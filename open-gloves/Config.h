/*
 * OpenGloves Firmware Configuration
 *
 *  This is the main configuration file. Tweak settings here to fit your hardware.
 *
 * Authors: Lucas_VRTech - LucidVR
 *         John Thomas  - Exia
 *         MinyStreem
 * lucidvrtech.com
 * github.com/JohnRThomas/opengloves-firmware/
 */

// Automatically set ANALOG_MAX depending on the microcontroller
#if defined(__AVR__)
#define ANALOG_MAX 1023
#elif defined(ESP32)
#define ANALOG_MAX 4095
#else
#error "This board doesn't have an auto ANALOG_MAX assignment, please set it manually by uncommenting ANALOG_MAX OVERRIDE!"
//ANALOG_MAX OVERRIDE:
// Uncomment and set as needed (only touch if you know what you are doing)
//#define ANALOG_MAX 4095
#endif

// Which communication protocol to use
#define COMM_USB          0
#define COMM_BLUETOOTH    1
#define COMM_WIFI         2
#define COMM_BLUETOOTH_LE 3
#define COMMUNICATION   COMM_BLUETOOTH_LE

// COMM settings
#define ENABLE_SYNCHRONOUS_COMM true // Experimental: If enabled, doesn't wait for FFB data before sending new input data.
#define SERIAL_BAUD_RATE        115200
#define BT_DEVICE_NAME          "OpenGlove-Left"
#define WIFI_SERIAL_SSID        "WIFI SSID here"
#define WIFI_SERIAL_PASSWORD    "password here"
#define WIFI_SERIAL_PORT        80
// https://www.bluetooth.com/specifications/assigned-numbers/
#define BLE_UUID                "8f8f832d-7705-4ba0-98c0-55b32e86f1e4"
#define BLE_RX_UUID             "f183924e-27a1-4633-8a20-29119a747867"
#define BLE_TX_UUID             "c88d81f1-b8f8-4f31-9585-b0be67e7273f"
#define COMM_DELAY              4 // How much time between data sends (ms)

// Button Settings
// If a button registers as pressed when not and vice versa (eg. using normally-closed switches),
// you can invert their behaviour here by setting their line to true.
// If unsure, set to false
#define INVERT_A        false
#define INVERT_B        false
#define INVERT_JOY      false // Does nothing if joystick is not enabled
#define INVERT_MENU     false
#define INVERT_CALIB    false
#define INVERT_TRIGGER  false // Does nothing if gesture is enabled
#define INVERT_GRAB     false // Does nothing if gesture is enabled
#define INVERT_PINCH    false // Does nothing if gesture is enabled

// Joystick configuration
#define ENABLE_JOYSTICK   true // Set to false if not using the joystick
#define INVERT_JOY_X      false
#define INVERT_JOY_Y      false
#define JOYSTICK_DEADZONE 0.1 //deadzone in the joystick to prevent drift. Value out of 1.0.

// Finger settings
#define ENABLE_THUMB        true  // If for some reason you don't want to track the thumb
#define ENABLE_SPLAY        false // Track the side to side motion of fingers
#define INVERT_CURL         false
#define INVERT_SPLAY        false

// Calibration Settings (See Calibration.hpp for more information)
#define CALIBRATION_LOOPS   -1 // How many loops should be calibrated. Set to -1 to always be calibrated.
#define CALIBRATION_CURL    MinMaxCalibrator<int, 0, ANALOG_MAX>
#define DRIVER_MAX_SPLAY    20  // The maximum deviation from the center point the driver supports.
#define SENSOR_MAX_SPLAY    270 // The maximum total range of rotation of the sensor.
#define CALIBRATION_SPLAY   CenterPointDeviationCalibrator<int, SENSOR_MAX_SPLAY, DRIVER_MAX_SPLAY, 0, ANALOG_MAX>

// Gesture enables, make false to use button override
#define TRIGGER_GESTURE true
#define GRAB_GESTURE    true
#define PINCH_GESTURE   (true && ENABLE_THUMB) // Cannot be enabled if there is no thumb

// Force Feedback and haptic settings
// Force feedback allows you to feel the solid objects you hold
// Haptics provide vibration.
#define ENABLE_FORCE_FEEDBACK false
#define ENABLE_HAPTICS        false

#define FORCE_FEEDBACK_FINGER_SCALING  false // Experimental: Determine servo range of motion based on calibration data.
#define FORCE_FEEDBACK_SMOOTH_STEPPING true // Use servo microsecond pulses instead of degrees for more servo steps.

#define FORCE_FEEDBACK_STYLE_SERVO       0
#define FORCE_FEEDBACK_STYLE_CLAMP       1
#define FORCE_FEEDBACK_STYLE_SERVO_CLAMP 2
#define FORCE_FEEDBACK_STYLE             FORCE_FEEDBACK_STYLE_SERVO

#define FORCE_FEEDBACK_CLAMP_UNLOCK       LOW
#define FORCE_FEEDBACK_CLAMP_LOCK         HIGH
#define FORCE_FEEDBACK_SERVO_CLAMP_UNLOCK 0
#define FORCE_FEEDBACK_SERVO_CLAMP_LOCK   20

#define FORCE_FEEDBACK_INVERT    false // Flips the direction of the FFB.
#define FORCE_FEEDBACK_MIN           0 // Value of 0 means no limit.
#define FORCE_FEEDBACK_MAX        1000 // Value of 1000 means maximum limit.
#define FORCE_FEEDBACK_RELEASE      50 // To prevent hardware damage, value passed the limit for when to release FFB. (Set to FORCE_FEEDBACK_MAX to disable)

// Counts of objects in the system used for looping
// Inputs
#define GESTURE_COUNT        (TRIGGER_GESTURE + GRAB_GESTURE + PINCH_GESTURE)
#define FINGER_COUNT         (ENABLE_THUMB ? 5 : 4)
#define JOYSTICK_COUNT       (ENABLE_JOYSTICK ? 2 : 0)
#define BUTTON_COUNT         (4 + ENABLE_JOYSTICK + !TRIGGER_GESTURE + !GRAB_GESTURE + !PINCH_GESTURE)
// Ouputs
#define HAPTIC_COUNT         (ENABLE_HAPTICS ? 1 : 0)
#define FORCE_FEEDBACK_COUNT (ENABLE_FORCE_FEEDBACK ? FINGER_COUNT : 0)
// Used for array allocations.
#define MAX_INPUT_COUNT      (BUTTON_COUNT+FINGER_COUNT+JOYSTICK_COUNT+GESTURE_COUNT)
#define MAX_CALIBRATED_COUNT FINGER_COUNT
#define MAX_OUTPUT_COUNT     (HAPTIC_COUNT + FORCE_FEEDBACK_COUNT)

//PINS CONFIGURATION
#if defined(__AVR__)
  //(This configuration is for Arduino Nano so make sure to change if you're on another board)
  #define PIN_PINKY           A0
  #define PIN_RING            A1
  #define PIN_MIDDLE          A2
  #define PIN_INDEX           A3
  #define PIN_THUMB           A4
  #define PIN_JOY_X           A6
  #define PIN_JOY_Y           A7
  #define PIN_JOY_BTN         7
  #define PIN_A_BTN           8
  #define PIN_B_BTN           9
  #define PIN_MENU_BTN        8
  #define PIN_TRIG_BTN        10 //unused if gesture set
  #define PIN_GRAB_BTN        11 //unused if gesture set
  #define PIN_PNCH_BTN        12 //unused if gesture set
  #define PIN_CALIB           13 //button for recalibration
  #define PIN_LED             LED_BUILTIN
  #define PIN_PINKY_FFB       2 //used for force feedback
  #define PIN_RING_FFB        3 //^
  #define PIN_MIDDLE_FFB      4 //^
  #define PIN_INDEX_FFB       5 //^
  #define PIN_THUMB_FFB       6 //^
  #define PIN_HAPTIC          1
  #define PIN_PINKY_SPLAY     1
  #define PIN_RING_SPLAY      1
  #define PIN_MIDDLE_SPLAY    1
  #define PIN_INDEX_SPLAY     1
  #define PIN_THUMB_SPLAY     1
#elif defined(ESP32)
  //(This configuration is for ESP32 DOIT V1 so make sure to change if you're on another board)
  #define PIN_PINKY           36
  #define PIN_RING            39
  #define PIN_MIDDLE          34
  #define PIN_INDEX           35
  #define PIN_THUMB           32
  #define PIN_JOY_X           33
  #define PIN_JOY_Y           25
  #define PIN_JOY_BTN         26
  #define PIN_A_BTN           27
  #define PIN_B_BTN           14
  #define PIN_MENU_BTN        27
  #define PIN_TRIG_BTN        12 //unused if gesture set
  #define PIN_GRAB_BTN        13 //unused if gesture set
  #define PIN_PNCH_BTN        23 //unused if gesture set
  #define PIN_CALIB           12 //button for recalibration
  #define PIN_LED             2
  #define PIN_PINKY_FFB       5  //used for force feedback
  #define PIN_RING_FFB        18 //^
  #define PIN_MIDDLE_FFB      19 //^
  #define PIN_INDEX_FFB       21 //^
  #define PIN_THUMB_FFB       17 //^
  #define PIN_HAPTIC          1
  #define PIN_PINKY_SPLAY     1
  #define PIN_RING_SPLAY      1
  #define PIN_MIDDLE_SPLAY    1
  #define PIN_INDEX_SPLAY     1
  #define PIN_THUMB_SPLAY     1
#endif

// You must install RunningMedian library to use this feature
// https://www.arduino.cc/reference/en/libraries/runningmedian/
#define ENABLE_MEDIAN_FILTER false //use the median of the previous values, helps reduce noise
#define MEDIAN_SAMPLES 20
