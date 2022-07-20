#include "Config.h"
#include "HardwareConfig.hpp"
#include "ICommunication.hpp"

#if COMMUNICATION == COMM_USB
  #include "SerialCommunication.hpp"
  ICommunication* comm = new SerialCommunication();
#elif COMMUNICATION == COMM_BLUETOOTH
  #include "SerialBTCommunication.hpp"
  ICommunication* comm = new BTSerialCommunication();
#elif COMMUNICATION == COMM_WIFI
  #include "SerialWIFICommunication.hpp"
  ICommunication* comm = new WIFISerialCommunication();
#endif

#define ALWAYS_CALIBRATING CALIBRATION_LOOPS == -1
int calibration_count = 0;

// These are composite lists of the hardware defined in the header above.
EncodedInput* inputs[MAX_INPUT_COUNT];
DecodedOuput* outputs[MAX_OUTPUT_COUNT];
Calibrated* calibrators[MAX_CALIBRATED_COUNT];

char* encoded_output_string;
size_t input_count = 0;
size_t output_count = 0;
size_t calibrated_count = 0;

// Common pattern for registering inputs and outputs
#define register(source, destination, new_count, existing) \
do {                                                       \
  for (size_t i = 0; i < new_count; existing++, i++) {     \
    destination[existing] = source[i];                     \
  }                                                        \
} while(false)

void setup() {
  // First thing to do is open the the communication channel.
  comm->start();

  // Register the inputs.
  register(buttons, inputs, BUTTON_COUNT, input_count);
  register(fingers, inputs, FINGER_COUNT, input_count);
  register(joysticks, inputs, JOYSTICK_COUNT, input_count);
  register(gestures, inputs, GESTURE_COUNT, input_count);

  // Register the calibrated inputs
  register(fingers, calibrators, FINGER_COUNT, calibrated_count);

  // Register the outputs.
  register(force_feedbacks, outputs, FORCE_FEEDBACK_COUNT, output_count);
  register(haptics, outputs, HAPTIC_COUNT, output_count);

  // Initialize I2C
  #if ENABLE_I2C_INTERFACE == true
    InitializeI2CInterface();
  #endif

  //Initialize PCA9685 Servo Board
  #if ENABLE_PCA9685_16CH_PWM_BOARD == true
    Initialize_PCA9685_Board();
  #endif
  Serial.print("Is glove using PCA9685 for haptic servos: ");
  Serial.println(USE_PCA9685_16CH_FOR_FFB);
      
  
  // Figure out needed size for the output string.
  int string_size = 0;
  for(size_t i = 0; i < input_count; i++) {
    string_size += inputs[i]->getEncodedSize();
  }

  // Add 1 for new line and 1 for the null terminator.
  encoded_output_string = new char[string_size + 1 + 1];

  // Setup all the inputs.
  for (size_t i = 0; i < input_count; i++) {
    inputs[i]->setupInput();
  }

  // Setup all the outputs.
  for (size_t i = 0; i < output_count; i++) {
    outputs[i]->setupOutput();
  }

  // Setup the StatusLED.
  led.setup();

  if (ALWAYS_CALIBRATING) {
    for (size_t i = 0; i < calibrated_count; i++) {
      calibrators[i]->enableCalibration();
    }
  }
}

void loop() {
  if (!comm->isOpen()){
    // Connection to Driver not ready, blink the LED to indicate no connection.
    led.setState(StatusLED::State::BLINK_STEADY);
  } else {
    // All is good, LED on to indicate a good connection.
    led.setState(StatusLED::State::ON);
  }

  // Notify the calibrators to turn on.
  if (calibration_button.isPressed()) {
    calibration_count = 0;
    for (size_t i = 0; i < calibrated_count; i++) {
      calibrators[i]->resetCalibration();
      calibrators[i]->enableCalibration();
    }
  }

  if (calibration_count < CALIBRATION_LOOPS || ALWAYS_CALIBRATING) {
    // Keep calibrating for one at least one more loop.
    calibration_count++;
  } else {
    // Calibration is done, notify the calibrators
    for (size_t i = 0; i < calibrated_count; i++) {
      calibrators[i]->disableCalibration();
    }
  }

  // Update all the inputs
  for (int i = 0; i < input_count; i++) {
    inputs[i]->readInput();
  }

  // Encode all of the inputs to a single string.
  encodeAll(encoded_output_string, inputs, input_count);

  // Send the string to the communication handler.
  comm->output(encoded_output_string);

  char received_bytes[100];
  if ((ENABLE_SYNCHRONOUS_COMM || comm->hasData()) &&
      comm->readData(received_bytes, 100)) {
    for (size_t i = 0; i < output_count; i++) {
      // Decode the update and write it to the output.
      outputs[i]->decodeToOuput(received_bytes);
    }
  }

  // Allow all the outputs to update their state.
  for (size_t i = 0; i < output_count; i++) {
    outputs[i]->updateOutput();
  }

  delay(COMM_DELAY);
}
