#pragma once

#include "Config.h"

#include "DriverProtocol.hpp"
#include "Pin.hpp"

class Button : public EncodedInput {
 public:
  Button(EncodedInput::Type type, Pin* pin, bool invert) :
    type(type), pin(pin), on_state(invert ? HIGH : LOW), value(false) {}

  void setupInput() override {
  }

  virtual void readInput() {
    value = (pin->digitalRead() == on_state);
  }

  inline int getEncodedSize() const override {
    // Encode string size = single char
    return 1;
  }

  int encode(char* output) const override {
    if (value) output[0] = type;
    return value ? 1 : 0;
  }

  bool isPressed() const {
    return value;
  }

 private:
  const EncodedInput::Type type;
  const Pin* pin;
  const bool on_state;
  bool value;
};
