#pragma once

#include "Filter.hpp"
#include "Config.h"
#include "Pin.hpp"

/**
 * Pure virtual interface for a sensor. These functions and those of the interface Calibrated MUST
 * be implemented, but we don't care about the implementation.
 */
class Sensor: public Calibrated {
 public:
  virtual int getValue() = 0;
};

/**
 * Calibrated sensor that inheriets a calibration type from the given calibrator.
 * If calibration is enabled, the value will update the calibration.
 */
template<typename FilterType>
class FilteredSensor : public FilterType, public Sensor {
 public:
  int getValue() override {
    int new_value = readInput();
    if (Calibrated::calibrate) {
      this->FilterType::update(new_value);
    }
    return this->FilterType::filter(new_value);
  }

  void resetCalibration() override {
    this->FilterType::reset();
  }

 protected:
  virtual int readInput() = 0;

 private:
  // Hoist these function from the base class as private.
  using FilterType::filter;
  using FilterType::reset;
  using FilterType::update;
};

/*
 * Very simple sensor that simely reads the voltage of the given pin.
 * If a resistive load is applied to the GPIO pin, the volage will change giving this sensor
 * different values.
 */
template<typename FilterType>
class ResistiveSensor : public FilteredSensor<FilterType> {
 public:
  ResistiveSensor(Pin* pin) : pin(pin) {}

  int readInput() override {
    return pin->analogRead();
  }

 private:
   Pin* pin;
};

// template<typename T, T output_min, T output_max>
// using HallEffectSensor =
//     ResistiveSensor<ExponentialToLinearFilter<3 /* the hall effect curve is cubic*/,

// TODO: Correct for exponential nature of hall effect
using HallEffectSensor = ResistiveSensor<DummyFilter<int>>;

template<int min, int max>
class HallEffectCurlMultiSensor : public Sensor {
 public:
  HallEffectCurlMultiSensor(Pin* left_pin,
                            Pin* right_pin)
    : left(left), right(right) {}

  void resetCalibration() override {
    left->resetCalibration();
    right->resetCalibration();
  }

  int getValue() override {
    int left_value = left->getValue();
    int right_value = right->getValue();
    int middle = (min + max) / 2;

    return max + ((middle - left_value) + (middle - right_value));
  }

 private:
  HallEffectSensor* left;
  HallEffectSensor* right;
};

template<int min, int max>
class HallEffectSplayMultiSensor : public Sensor {
 public:
  HallEffectSplayMultiSensor(Pin* left_pin,
                             Pin* right_pin)
    : left(left), right(right) {}

  void resetCalibration() override {
    left->resetCalibration();
    right->resetCalibration();
  }

  int getValue() override {
    int left_value = left->getValue();
    int right_value = right->getValue();
    int middle = (min + max) / 2;

    return middle - left_value + right_value;
  }

 private:
  HallEffectSensor* left;
  HallEffectSensor* right;
};