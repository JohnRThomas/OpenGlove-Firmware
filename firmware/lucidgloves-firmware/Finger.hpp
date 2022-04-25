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
    median(MEDIAN_SAMPLES), calibrator(0, ANALOG_MAX, CLAMP_ANALOG_MAP) {}

  void readInput() override {
    // Read the latest value.
    int new_value = analogRead(pin);

    // Apply configured modifiers.
    #if INVERT_FLEXION
      new_value = ANALOG_MAX - new_value;
    #endif

    #if ENABLE_MEDIAN_FILTER
      median.add(new_value);
      new_value = median.getMedian();
    #endif

    #if CLAMP_FLEXION
      new_value = constrain(new_value, CLAMP_MIN, CLAMP_MAX);
    #endif

    // Update the calibration
    if (calibrate) {
      calibrator.update(new_value);
    }

    // set the value to the calibrated value.
    value = calibrator.calibrate(new_value, 0, ANALOG_MAX);
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
    return calibrator.calibrate(input, min, max);
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

  MinMaxCalibrator<int> calibrator;
};

class SplayFinger : public Finger {
 public:
  SplayFinger(EncodedInput::Type enc_type, int pin, int splay_pin) :
    Finger(enc_type, pin), splay_pin(splay_pin), splay_value(0),
    splay_calibrator(0, ANALOG_MAX, CLAMP_ANALOG_MAP) {}

  void readInput() override {
    Finger::readInput();
    int new_splay_value = analogRead(splay_pin);
    
    // map splay value to match max angle in driver
    splay_value = map(new_splay_value, ANALOG_MID - DRIVER_MAX_SPLAY_ANGLE, ANALOG_MID + DRIVER_MAX_SPLAY_ANGLE, ANALOG_MID - POT_MAX_SPLAY_ANGLE, ANALOG_MID + POT_MAX_SPLAY_ANGLE);

    // constrain splay value to 0 -> 4095 range.
    splay_value = constrain(splay_value, 0, 4095);
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
  MinMaxCalibrator<int> splay_calibrator;
};
