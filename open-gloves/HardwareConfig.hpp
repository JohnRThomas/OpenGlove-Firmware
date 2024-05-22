#pragma once
#include "Config.h"

#include "DriverProtocol.hpp"

#include "Button.hpp"
#include "Filter.hpp"
#include "Finger.hpp"
#include "ForceFeedback.hpp"
#include "Gesture.hpp"
#include "Haptics.hpp"
#include "JoyStick.hpp"
#include "LED.hpp"
#include "Pin.hpp"
#include "Sensor.hpp"

StatusLED led(PIN_LED);

struct {
  void setup() {
    pinMode(MUX_SEL_0, OUTPUT);
    pinMode(MUX_SEL_1, OUTPUT);
    pinMode(MUX_SEL_2, OUTPUT);
    pinMode(MUX_SEL_3, OUTPUT);
  }
} multiplexer;

// This button is referenced directly by the FW, so we need a pointer to it outside
// the list of buttons.
Button calibration_button(EncodedInput::Type::CALIBRATE, new PIN_CALIB, INVERT_CALIB);

Button* buttons[BUTTON_COUNT] = {
  new Button(EncodedInput::Type::A_BTN, new PIN_A_BTN, INVERT_A),
  new Button(EncodedInput::Type::B_BTN, new PIN_B_BTN, INVERT_B),
  new Button(EncodedInput::Type::MENU, new PIN_MENU_BTN, INVERT_MENU),
  &calibration_button,
  #if ENABLE_JOYSTICK
    new Button(EncodedInput::Type::JOY_BTN, new PIN_JOY_BTN, INVERT_JOY),
  #endif
  #if !TRIGGER_GESTURE
    new Button(EncodedInput::Type::TRIGGER, new PIN_TRIG_BTN, INVERT_TRIGGER),
  #endif
  #if !GRAB_GESTURE
    new Button(EncodedInput::Type::GRAB, new PIN_GRAB_BTN, INVERT_GRAB),
  #endif
  #if !PINCH_GESTURE
    new Button(EncodedInput::Type::PINCH, new PIN_PNCH_BTN, INVERT_PINCH),
  #endif

};

#define FINGER_PARAMS(F)                                                               \
  EncodedInput::Type::F, INVERT_CURL, INVERT_SPLAY,                                    \
  new HallEffectCurlMultiSensor<0, ANALOG_MAX>(new PIN_##F##_K0, new PIN_##F##_SPLAY), \
  new ResistiveSensor<MinMaxFilter<int, 0, ANALOG_MAX>>(new PIN_##F##_K1),             \
  nullptr, /* No Knuckle 2 on the other fingers */                                     \
  new HallEffectSplayMultiSensor<0, ANALOG_MAX>(new PIN_##F##_K0, new PIN_##F##_SPLAY)

#define INDEX_FINGER_PARAMS                                                            \
  EncodedInput::Type::INDEX, INVERT_CURL, INVERT_SPLAY,                                \
  new HallEffectCurlMultiSensor<0, ANALOG_MAX>(new PIN_INDEX_K0, new PIN_INDEX_SPLAY), \
  new ResistiveSensor<MinMaxFilter<int, 0, ANALOG_MAX>>(new PIN_INDEX_K1),             \
  new ResistiveSensor<MinMaxFilter<int, 0, ANALOG_MAX>>(new PIN_INDEX_K2),             \
  new HallEffectSplayMultiSensor<0, ANALOG_MAX>(new PIN_INDEX_K0, new PIN_INDEX_SPLAY)


#if ENABLE_THUMB
  ConfigurableFinger<ENABLE_SPLAY, 2, EncodedInput::KnuckleThumbOffset>
      finger_thumb(FINGER_PARAMS(THUMB));
#endif

ConfigurableFinger<ENABLE_SPLAY, 3> finger_index(INDEX_FINGER_PARAMS);
ConfigurableFinger<ENABLE_SPLAY, 2> finger_middle(FINGER_PARAMS(MIDDLE));
ConfigurableFinger<ENABLE_SPLAY, 2> finger_ring(FINGER_PARAMS(RING));
ConfigurableFinger<ENABLE_SPLAY, 2> finger_pinky(FINGER_PARAMS(PINKY));

Finger* fingers[FINGER_COUNT] = {
  #if ENABLE_THUMB
    &finger_thumb,
  #endif
  &finger_index, &finger_middle, &finger_ring, &finger_pinky
};

JoyStickAxis* joysticks[JOYSTICK_COUNT] = {
  #if ENABLE_JOYSTICK
    new JoyStickAxis(EncodedInput::Type::JOY_X, new PIN_JOY_X, JOYSTICK_DEADZONE, INVERT_JOY_X),
    new JoyStickAxis(EncodedInput::Type::JOY_Y, new PIN_JOY_Y, JOYSTICK_DEADZONE, INVERT_JOY_Y)
  #endif
};

Gesture* gestures[GESTURE_COUNT] = {
  #if TRIGGER_GESTURE
    new TriggerGesture(&finger_index),
  #endif
  #if GRAB_GESTURE
    new GrabGesture(&finger_index, &finger_middle, &finger_ring, &finger_pinky),
  #endif
  #if PINCH_GESTURE
    new PinchGesture(&finger_thumb, &finger_index)
  #endif
};

HapticMotor* haptics[HAPTIC_COUNT] = {
  #if ENABLE_HAPTICS
    new HapticMotor(DecodedOuput::Type::HAPTIC_FREQ,
                    DecodedOuput::Type::HAPTIC_DURATION,
                    DecodedOuput::Type::HAPTIC_AMPLITUDE, PIN_HAPTIC),
  #endif
};

ForceFeedback* force_feedbacks[FORCE_FEEDBACK_COUNT] {
  #if ENABLE_FORCE_FEEDBACK
    #if FORCE_FEEDBACK_STYLE == FORCE_FEEDBACK_STYLE_SERVO
      #if ENABLE_THUMB
        new ServoForceFeedback(DecodedOuput::Type::FFB_THUMB, &finger_thumb, PIN_THUMB_FFB, FORCE_FEEDBACK_INVERT),
      #endif
      new ServoForceFeedback(DecodedOuput::Type::FFB_INDEX, &finger_index, PIN_INDEX_FFB, FORCE_FEEDBACK_INVERT),
      new ServoForceFeedback(DecodedOuput::Type::FFB_MIDDLE, &finger_middle, PIN_MIDDLE_FFB, FORCE_FEEDBACK_INVERT),
      new ServoForceFeedback(DecodedOuput::Type::FFB_RING, &finger_ring, PIN_RING_FFB, FORCE_FEEDBACK_INVERT),
      new ServoForceFeedback(DecodedOuput::Type::FFB_PINKY, &finger_pinky, PIN_PINKY_FFB, FORCE_FEEDBACK_INVERT)
    #elif FORCE_FEEDBACK_STYLE == FORCE_FEEDBACK_STYLE_CLAMP
      #if ENABLE_THUMB
        new DigitalClampForceFeedback(DecodedOuput::Type::FFB_THUMB, &finger_thumb, PIN_THUMB_FFB),
      #endif
      new DigitalClampForceFeedback(DecodedOuput::Type::FFB_INDEX, &finger_index, PIN_INDEX_FFB),
      new DigitalClampForceFeedback(DecodedOuput::Type::FFB_MIDDLE, &finger_middle, PIN_MIDDLE_FFB),
      new DigitalClampForceFeedback(DecodedOuput::Type::FFB_RING, &finger_ring, PIN_RING_FFB),
      new DigitalClampForceFeedback(DecodedOuput::Type::FFB_PINKY, &finger_pinky, PIN_PINKY_FFB)
    #elif FORCE_FEEDBACK_STYLE == FORCE_FEEDBACK_STYLE_SERVO_CLAMP
      #if ENABLE_THUMB
        new ServoClampForceFeedback(DecodedOuput::Type::FFB_THUMB, &finger_thumb, PIN_THUMB_FFB),
      #endif
      new ServoClampForceFeedback(DecodedOuput::Type::FFB_INDEX, &finger_index, PIN_INDEX_FFB),
      new ServoClampForceFeedback(DecodedOuput::Type::FFB_MIDDLE, &finger_middle, PIN_MIDDLE_FFB),
      new ServoClampForceFeedback(DecodedOuput::Type::FFB_RING, &finger_ring, PIN_RING_FFB),
      new ServoClampForceFeedback(DecodedOuput::Type::FFB_PINKY, &finger_pinky, PIN_PINKY_FFB)
    #endif
  #endif
};
