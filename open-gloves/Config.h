/*
 * OpenGloves Firmware Configuration
 *
 *  This is the main configuration file. Tweak settings here to fit your hardware.
 *
 * Authors: Lucas_VRTech - LucidVR
 *         John Thomas  - Exia
 *         MinyStreem
 * lucidvrtech.com
 * github.com/JohnRThomas/OpenGloves-Firmware/
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
#define COMM_USB        0
#define COMM_BLUETOOTH  1
#define COMM_WIFI       2
#define COMMUNICATION   COMM_USB

// COMM settings
#define ENABLE_SYNCHRONOUS_COMM true // Experimental: If enabled, doesn't wait for FFB data before sending new input data.
#define SERIAL_BAUD_RATE        115200
#define BT_DEVICE_NAME          "OpenGlove-Left"
#define WIFI_SERIAL_SSID        "WIFI SSID here"
#define WIFI_SERIAL_PASSWORD    "password here"
#define WIFI_SERIAL_PORT        80
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
#define ENABLE_SPLAY        true // Track the side to side motion of fingers
#define INVERT_CURL         false
#define INVERT_SPLAY        false
#define KNUCKLE_COUNT       1 // How many knuckles each finger has
#define THUMB_KNUCKLE_COUNT KNUCKLE_COUNT // How many knuckles the thumb has
// Used for 2 knuckle mode to estimate the 3rd knuckle
#define KNUCKLE_DEPENDENCY_START 0.10f // Percentage (10%) of K1 motion before K2 starts moving
#define KNUCKLE_DEPENDENCY_END   0.50f // Percentage (50%) of K1 motion where K2 finishes moving

// Calibration Settings (See Filter.hpp for more information)
#define CALIBRATION_LOOPS   -1 // How many loops should be calibrated. Set to -1 to always be calibrated.
#define DRIVER_MAX_SPLAY    20  // The maximum deviation from the center point the driver supports.
#define SENSOR_MAX_SPLAY    270 // The maximum total range of rotation of the sensor.

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

// Settings for the mutiplexer
#define ENABLE_MULTIPLEXER false

// Counts of objects in the system used for looping
// Don't touch this unless you are changing the way the code works.
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

// You must install RunningMedian library to use this feature
// https://www.arduino.cc/reference/en/libraries/runningmedian/
// TODO: Allow calibration/filtering to be configurable here.
#define ENABLE_MEDIAN_FILTER false //use the median of the previous values, helps reduce noise
#define MEDIAN_SAMPLES 20

// PINS CONFIGURATION
// For ESP32, avoid GPIO6 - GPIO11, they are used intenally for FLASH access and may break things if used.
// Analog Inputs
#define PIN_THUMB_K0     DirectPin<32>()  // First knuckle or whole finger if only one pot is attatched
#define PIN_THUMB_K1     MultiplexedPin<M0>()
#define PIN_THUMB_K2     MultiplexedPin<M1>()
#define PIN_THUMB_SPLAY  MultiplexedPin<M2>()

#define PIN_INDEX_K0     DirectPin<35>()  // First knuckle or whole finger if only one pot is attatched
#define PIN_INDEX_K1     MultiplexedPin<M3>()
#define PIN_INDEX_K2     MultiplexedPin<M4>()
#define PIN_INDEX_SPLAY  MultiplexedPin<M5>()

#define PIN_MIDDLE_K0    DirectPin<34>()  // First knuckle or whole finger if only one pot is attatched
#define PIN_MIDDLE_K1    MultiplexedPin<M6>()
#define PIN_MIDDLE_K2    MultiplexedPin<M7>()
#define PIN_MIDDLE_SPLAY MultiplexedPin<M8>()

#define PIN_RING_K0      DirectPin<39>()  // First knuckle or whole finger if only one pot is attatched
#define PIN_RING_K1      MultiplexedPin<M9>()
#define PIN_RING_K2      MultiplexedPin<M10>()
#define PIN_RING_SPLAY   MultiplexedPin<M11>()

#define PIN_PINKY_K0     DirectPin<36>()  // First knuckle or whole finger if only one pot is attatched
#define PIN_PINKY_K1     MultiplexedPin<M12>()
#define PIN_PINKY_K2     MultiplexedPin<M13>()
#define PIN_PINKY_SPLAY  MultiplexedPin<M14>()

#define PIN_JOY_X        DirectPin<33>()
#define PIN_JOY_Y        DirectPin<25>()

#define MUX_INPUT_A      33 // Must be a direct pin!
#define MUX_INPUT_B      25 // Must be a direct pin!

// Digital Inputs
#define PIN_JOY_BTN      DirectPin<23>()
#define PIN_A_BTN        DirectPin<22>()
#define PIN_B_BTN        DirectPin<1>()
#define PIN_MENU_BTN     DirectPin<3>()
#define PIN_CALIB        DirectPin<12>() //button for recalibration
#define PIN_TRIG_BTN     DirectPin<-1>() //unused if gesture is enabled
#define PIN_GRAB_BTN     DirectPin<-1>() //unused if gesture is enabled
#define PIN_PNCH_BTN     DirectPin<-1>() //unused if gesture is enabled

// Ouput pins
// These pins do not support multiplexing since pins are shared, any set output would only last
// until the multiplex changes it's connection.
#define MUX_SEL_0        26
#define MUX_SEL_1        27
#define MUX_SEL_2        14
#define MUX_SEL_3        12
#define PIN_LED          2
#define PIN_THUMB_FFB    21
#define PIN_INDEX_FFB    19
#define PIN_MIDDLE_FFB   18
#define PIN_RING_FFB     5
#define PIN_PINKY_FFB    17
#define PIN_HAPTIC       16
