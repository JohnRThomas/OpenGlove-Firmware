#pragma once

constexpr float accurateMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Same as the above, but both mins are 0.
constexpr float simpleAccurateMap(float x, float in_max, float out_max) {
  return x * out_max / in_max;
}

class Calibrated {
 public:
  virtual void resetCalibration() = 0;

  virtual void enableCalibration() {
    calibrate = true;
  }

  virtual void disableCalibration() {
    calibrate = false;
  }

 protected:
  bool calibrate;
};

template<typename T>
struct Filter {
  virtual void reset() = 0;
  virtual void update(T /*input*/) = 0;
  virtual T filter(T input) const = 0;
};

template<typename T, T output_min, T output_max>
class MinMaxFilter : public Filter<T> {
 public:
  MinMaxFilter() : value_min(output_max), value_max(output_min) {}

  void reset() override {
    value_min = output_max;
    value_max = output_min;
  }

  void update(T input) override {
    // Update the min and the max.
    if (input < value_min) value_min = input;
    if (input > value_max) value_max = input;
  }

  T filter(T input) const override {
    // This means we haven't had any calibration data yet.
    // Return a neutral value right in the middle of the output range.
    if (value_min > value_max) return (output_min + output_max) / 2.0f;

    // Map the input range to the output range.
    T output = accurateMap(input, value_min, value_max, output_min, output_max);

    // Lock the range to the output.
    return constrain(output, output_min, output_max);
  }

 private:
  T value_min;
  T value_max;
};

template<typename T, T sensor_max, T driver_max_deviation, T output_min, T output_max>
class CenterPointDeviationFilter : public Filter<T> {
 public:
  CenterPointDeviationFilter() : range_min(sensor_max), range_max(0) {}

  void reset() override {
    range_min = sensor_max;
    range_max = 0;
  }

  void update(T input) override {
    // Update the min and the max.
    if (input < range_min) range_min = accurateMap(input, output_min, output_max, 0, sensor_max);
    if (input > range_max) range_max = accurateMap(input, output_min, output_max, 0, sensor_max);
  }

  T filter(T input) const {
    // Find the center point of the sensor so we know how much we have deviated from it.
    T center = (range_min + range_max) / 2.0f;

    // Map the input to the sensor range of motion.
    T output = accurateMap(input, output_min, output_max, 0, sensor_max);

    // Find the deviation from the center and constrain it to the maximum that the driver supports.
    output = constrain(output - center, -driver_max_deviation, driver_max_deviation);

    // Finally map the deviation from the center back to the output range.
    return map(output, -driver_max_deviation, driver_max_deviation, output_min, output_max);
  }

 private:
  T range_min;
  T range_max;
};

template<typename T, T sensor_max, T driver_max_deviation, T output_min, T output_max>
class FixedCenterPointDeviationFilter : public Filter<T> {
 public:
   void reset() override {}
   void update(T /*input*/) override {}

  T filter(T input) const override {
    // Find the center point of the sensor so we know how much we have deviated from it.
    T center = sensor_max / 2.0f;

    // Map the input to the sensor range of motion.
    T output = accurateMap(input, output_min, output_max, 0, sensor_max);

    // Find the deviation from the center and constrain it to the maximum that the driver supports.
    output = constrain(output - center, -driver_max_deviation, driver_max_deviation);

    // Finally map the deviation from the center back to the output range.
    return map(output, -driver_max_deviation, driver_max_deviation, output_min, output_max);
  }
};

template<typename T, T exponent, T output_min, T output_max>
class ExponentialToLinearFilter : public MinMaxFilter<T, output_min, output_max> {
  void reset() override {}
  void update(T /*input*/) override {}

  T filter(T input) const {
    // TODO: Correct for the exponential input based on the configured exponent
    return this->MinMaxFilter<T, output_min, output_max>::filter(input);
  }
};

template<typename T>
struct DummyFilter : public Filter<T> {
  void reset() override {}
  void update(T /*input*/) override {}

  inline T filter(T input) const override {
    return input;
  }
};