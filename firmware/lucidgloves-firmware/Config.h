/*
 * LucidGloves Firmware Version 5
 * Author: Lucas_VRTech - LucidVR
 *         John Thomas  - Exia
 * lucidvrtech.com
 * github.com/JohnRThomas/FlexiaGloves/
 */

// This is the main configuration file. Tweak settings here to fit your hardware.

// Which communication protocol to use
#define COMM_SERIAL 0
#define COMM_BTSERIAL 1
#define COMMUNICATION COMM_SERIAL

// Serial over USB
#define SERIAL_BAUD_RATE 115200

// Serial over Bluetooth
#define BTSERIAL_DEVICE_NAME "lucidgloves-left"

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
#define ENABLE_JOYSTICK  true // make false if not using the joystick
#define INVERT_JOY_X     false
#define INVERT_JOY_Y     false
#define JOYSTICK_DEADZONE 0.1 //deadzone in the joystick to prevent drift. Value out of 1.0.

// Finger settings
#define ENABLE_THUMB   true  // If for some reason you don't want to track the thumb
#define ENABLE_SPLAY   false // Track the side to side motion of fingers
#define INVERT_FLEXION false
#define INVERT_SPLAY   false

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
#define FORCE_FEEDBACK_SMOOTH_STEPPING false // Experimental: Use servo microsecond pulses instead of degrees.

#define FORCE_FEEDBACK_STYLE_SERVO       0
#define FORCE_FEEDBACK_STYLE_CLAMP       1
#define FORCE_FEEDBACK_STYLE_SERVO_CLAMP 2
#define FORCE_FEEDBACK_STYLE             FORCE_FEEDBACK_STYLE_SERVO

#define FORCE_FEEDBACK_CLAMP_UNLOCK       LOW
#define FORCE_FEEDBACK_CLAMP_LOCK         HIGH
#define FORCE_FEEDBACK_SERVO_CLAMP_UNLOCK 0
#define FORCE_FEEDBACK_SERVO_CLAMP_LOCK   20

#define FORCE_FEEDBACK_MIN      0 // Value of 0 means no limit.
#define FORCE_FEEDBACK_MAX   1000 // Value of 1000 means maximum limit.
#define FORCE_FEEDBACK_RELEASE 50 // To prevent hardware damage, value passed the limit for when to release FFB. (Set to FORCE_FEEDBACK_MAX to disable)

// Splay Scaling
// Unit invariant as long as you use the same unit between the two variables
#define POT_MAX_ANGLE    270
#define DRIVER_MAX_ANGLE 40
#define ANALOG_MID (ANALOG_MAX / 2) //expected resting splay value (can be chanched if your pots are not in the midle of their range when fingers are in resting position)

// Counts of objects in the system used for looping
// Inputs
#define GESTURE_COUNT        (TRIGGER_GESTURE + GRAB_GESTURE + PINCH_GESTURE)
#define FINGER_COUNT         (ENABLE_THUMB ? 5 : 4)
#define JOYSTICK_COUNT       (ENABLE_JOYSTICK ? 2 : 0)
#define BUTTON_COUNT         (4 + ENABLE_JOYSTICK + !TRIGGER_GESTURE + !GRAB_GESTURE + !PINCH_GESTURE)
// Ouputs
#define HAPTIC_COUNT         (ENABLE_HAPTICS ? 1 : 0)
#define FORCE_FEEDBACK_COUNT (ENABLE_FORCE_FEEDBACK ? FINGER_COUNT : 0)
// Used for array allocations. These may need to be increased if more inputs are added.
#define MAX_INPUT_COUNT      32
#define MAX_CALIBRATED_COUNT 8
#define MAX_OUTPUT_COUNT     8

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

// Advanced Config. Don't touch this unless you know what you are doing. Only for the pros XD
#define LOOP_TIME          4 //How much time between data sends (ms), set to 0 for a good time :)
#define CALIBRATION_LOOPS -1 //How many loops should be calibrated. Set to -1 to always be calibrated.

//Automatically set ANALOG_MAX depending on the microcontroller
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

//Filtering and clamping analog inputs
#define CLAMP_ANALOG_MAP true //clamp the mapped analog values from 0 to ANALOG_MAX

// Enable and set min and max to match your sensor's expected raw value range
// This discards any spurious values outside of the useful range
#define CLAMP_FLEXION false       // Clamp the raw flexion values
#define CLAMP_MIN     0           // Minimum value from the flexion sensors
#define CLAMP_MAX     ANALOG_MAX  // Maximum value from the flexion sensors

// You must install RunningMedian library to use this feature
// https://www.arduino.cc/reference/en/libraries/runningmedian/
#define ENABLE_MEDIAN_FILTER false //use the median of the previous values, helps reduce noise
#define MEDIAN_SAMPLES 20
