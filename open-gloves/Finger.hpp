#pragma once

#include "Config.h"

#include "Calibration.hpp"
#include "DriverProtocol.hpp"

#if ENABLE_MEDIAN_FILTER
  #include <RunningMedian.h>
#endif

class Finger : public EncodedInput, public Calibrated {
 public:
  Finger(EncodedInput::Type enc_type, int pin) :
    type(enc_type), pin(pin), value(0),
    median(MEDIAN_SAMPLES) {}

  void readInput() override {
    // Read the latest value.
    int new_value = analogRead(pin);

    // Apply configured modifiers.
    #if INVERT_CURL
      new_value = ANALOG_MAX - new_value;
    #endif

    #if ENABLE_MEDIAN_FILTER
      median.add(new_value);
      new_value = median.getMedian();
    #endif

    // Update the calibration
    if (calibrate) {
      calibrator.update(new_value);
    }

    // set the value to the calibrated value.
    value = calibrator.calibrate(new_value);
  }

  inline int getEncodedSize() const override {
    // Encode string size = AXXXX + '\0'
    return 6;
  }

  int encode(char* output) const override {
    return snprintf(output, getEncodedSize(), "%c%d", type, value);
  }

  void resetCalibration() override {
    calibrator.reset();
  }

  virtual int flexionValue() const {
    return value;
  }

  // Allow others access to the finger's calibrator so they can
  // map other values on this range.
  int mapOntoCalibratedRange(int input, int min, int max) const {
    return calibrator.calibrate(input);
  }

 protected:
  EncodedInput::Type type;
  int pin;
  int value;

  #if ENABLE_MEDIAN_FILTER
    RunningMedian median;
  #else
    int median;
  #endif

  CALIBRATION_CURL calibrator;
};

class SplayFinger : public Finger {
 public:
  SplayFinger(EncodedInput::Type enc_type, int pin, int splay_pin) :
    Finger(enc_type, pin), splay_pin(splay_pin), splay_value(0) {}

  void readInput() override {
    Finger::readInput();
    int new_splay_value = analogRead(splay_pin);
    // Update the calibration
    if (calibrate) {
      splay_calibrator.update(new_splay_value);
    }

    // set the value to the calibrated value.
    splay_value = splay_calibrator.calibrate(new_splay_value);
  }

  inline int getEncodedSize() const override {
    // Encoded string size = AXXXX(AB)XXXX + '\0'
    return 14;
  }

  int encode(char* output) const override {
    return snprintf(output, getEncodedSize(), "%c%d(%cB)%d", type, value, type, splay_value);
  }

  virtual int splayValue() const {
    return splay_value;
  }

 protected:
  int splay_pin;
  int splay_value;
  CALIBRATION_SPLAY splay_calibrator;
};
