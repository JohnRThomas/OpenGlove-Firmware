#pragma once

#include "Config.h"

#include "DriverProtocol.hpp"
#include "Pin.hpp"

class JoyStickAxis : public EncodedInput {
 public:
  JoyStickAxis(EncodedInput::Type type, Pin* pin, float dead_zone, bool invert) :
    type(type), pin(pin), dead_zone(dead_zone), invert(invert), value(ANALOG_MAX/2) {}

  void readInput() override {
    // Read the latest value.
    int new_value = pin->analogRead();

    // Apply the deadzone to the value.
    new_value = filterDeadZone(new_value);

    // Invert if required.
    if (invert) {
      new_value = ANALOG_MAX - new_value;
    }

    // Update the value.
    value = new_value;
  }

  inline int getEncodedSize() const override {
    // Encode string size = AXXXX + '\0'
    return 6;
  }

  int encode(char* output) const override {
    return snprintf(output, getEncodedSize(), "%c%d", type, value);
  }

  int getValue() const {
    return value;
  }

 private:
  int filterDeadZone(int in) {
    // This function clamps the input to the center of the range if
    // the value is within the threshold. This is to eliminate at-rest
    // noise of the joystick.
    int center = ANALOG_MAX/2;
    return abs(center - in) < dead_zone * ANALOG_MAX ? center : in;
  }

  EncodedInput::Type type;
  Pin* pin;
  float dead_zone;
  bool invert;
  int value;
};
