#pragma once

#include "Config.h"

#include "Filter.hpp"
#include "DriverProtocol.hpp"
#include "Sensor.hpp"

// Base finger class with externalized features.
class Finger : public EncodedInput, public Calibrated {
 public:
  virtual int curlValue() const = 0;
  virtual int splayValue() const = 0;
 protected:
  Finger(EncodedInput::Type type, bool invert_curl, bool invert_splay)
    : type(type), invert_curl(invert_curl), invert_splay(invert_splay) {}

  EncodedInput::Type type;
  bool invert_curl;
  bool invert_splay;
};

#define ConstructorArgs EncodedInput::Type type, bool invert_curl, bool invert_splay, \
                        Sensor* k0, Sensor* k1, Sensor* k2, Sensor* splay
#define all_args type, invert_curl, invert_splay, k0, k1, k2, splay

// Declare unspecialized type, but don't define it so any unspecialized version won't compile (eg knuckle count 4).
template<bool enable_splay, size_t knuckle_count,
         size_t knuckle_offset=EncodedInput::KnuckleFingerOffset>
class ConfigurableFinger;

/*
 * ConfigurableFinger: 1 knuckle, no splay
 */
template<size_t _ /* knuckle_offset does not apply here */>
class ConfigurableFinger<false, 1, _> : public Finger {
 public:
  ConfigurableFinger(ConstructorArgs)
    : Finger(type, invert_curl, invert_splay), sensor(k0), value(0) {}

  void readInput() override {
    // Read the latest value.
    value = sensor->getValue();
  }

  inline int getEncodedSize() const override {
    return EncodedInput::CurlSize;
  }

  int encode(char* output) const override {
    return snprintf(output, getEncodedSize(), EncodedInput::CurlFormat, type, value);
  }

  int curlValue() const override {
    return value;
  }

  int splayValue() const override {
    // This finger type doesn't have splay so just return the center for anyone that asks.
    return ANALOG_MAX / 2;
  }

  // Allow others access to the finger's calibrator so they can
  // map other values on this range.
  int mapOntoCalibratedRange(int input, int min, int max) const {
    return input;
    //return sensor->filter(input);
  }

  void resetCalibration() override {
    sensor->resetCalibration();
  }

  void enableCalibration() override {
    sensor->enableCalibration();
  }

  void disableCalibration() override {
    sensor->disableCalibration();
  }

 protected:
  Sensor* sensor;
  int value;
};

/*
 * ConfigurableFinger: 2 knuckles, no splay
 */
template<size_t knuckle_offset>
class ConfigurableFinger<false, 2, knuckle_offset> : public Finger {
 public:
  ConfigurableFinger(ConstructorArgs) : Finger(type, invert_curl, invert_splay), sensors{k0, k1},
                                        values{0, 0, 0} {}

  void readInput() override {
    // Read from the two sensors that we have.
    for (size_t i = 0; i < 2; i++) {
      values[i] = sensors[i]->getValue();
    }

    // The third knuckle is based on the second knuckle.
    constexpr int min = ANALOG_MAX * KNUCKLE_DEPENDENCY_START;
    constexpr int max = ANALOG_MAX * KNUCKLE_DEPENDENCY_END;
    values[2] = accurateMap(constrain(values[1], min, max), min, max, 0, ANALOG_MAX);
  }

  inline int getEncodedSize() const override {
    return EncodedInput::KnuckleSize + EncodedInput::KnuckleSize + EncodedInput::KnuckleSize - 2;
  }

  int encode(char* output) const override {
    int offset = 0;
    for (size_t i = 0; i < 3; i++) {
      offset += snprintf(output + offset, EncodedInput::KnuckleSize,
                         EncodedInput::knuckleFormat(i + knuckle_offset), this->type, values[i]);
    }
    return offset;
  }

  int curlValue() const override {
    // Take the average of all knuckles
    return (values[0] + values[1] + values[2]) / 3.0f;
  }

  int splayValue() const override {
    // This finger type doesn't have splay so just return the center for anyone that asks.
    return ANALOG_MAX / 2;
  }

  // Allow others access to the finger's calibrator so they can
  // map other values on this range.
  int mapOntoCalibratedRange(int input, int min, int max) const {
    // TODO: figure out how to make this work.
    return 0;
    //return calibrator.filter(input, min, max);
  }

  void resetCalibration() override {
    for (size_t i = 0; i < 2; i++) {
      sensors[i]->resetCalibration();
    }
  }

  void enableCalibration() override {
    for (size_t i = 0; i < 2; i++) {
      sensors[i]->enableCalibration();
    }
  }

  void disableCalibration() override {
    for (size_t i = 0; i < 2; i++) {
      sensors[i]->disableCalibration();
    }
  }

 protected:
  Sensor* sensors[2];
  int values[3];
};

/*
 * ConfigurableFinger: 3 knuckle, no splay
 */
template<size_t knuckle_offset>
class ConfigurableFinger<false, 3, knuckle_offset> : public Finger {
   public:
  ConfigurableFinger(ConstructorArgs) : Finger(type, invert_curl, invert_splay), sensors{k0, k1, k2},
                                        values{0, 0, 0} {}

  void readInput() override {
    for (size_t i = 0; i < 3; i++) {
      values[i] = sensors[i]->getValue();
    }
  }

  inline int getEncodedSize() const override {
    return EncodedInput::KnuckleSize + EncodedInput::KnuckleSize + EncodedInput::KnuckleSize - 2;
  }

  int encode(char* output) const override {
    int offset = 0;
    for (size_t i = 0; i < 3; i++) {
      offset += snprintf(output + offset, EncodedInput::KnuckleSize,
                         EncodedInput::knuckleFormat(i + knuckle_offset), this->type, values[i]);
    }
    return offset;
  }

  int curlValue() const override {
    // Take the average of all knuckles
    return (values[0] + values[1] + values[2]) / 3.0f;
  }

  int splayValue() const override {
    // This finger type doesn't have splay so just return the center for anyone that asks.
    return ANALOG_MAX / 2;
  }

  // Allow others access to the finger's calibrator so they can
  // map other values on this range.
  int mapOntoCalibratedRange(int input, int min, int max) const {
    // TODO: figure out how to make this work.
    return input;
    //return calibrator.filter(input, min, max);
  }

  void resetCalibration() override {
    for (size_t i = 0; i < 3; i++) {
      sensors[i]->resetCalibration();
    }
  }

  void enableCalibration() override {
    for (size_t i = 0; i < 3; i++) {
      sensors[i]->enableCalibration();
    }
  }

  void disableCalibration() override {
    for (size_t i = 0; i < 3; i++) {
      sensors[i]->disableCalibration();
    }
  }

 protected:
  Sensor* sensors[3];
  int values[3];
};

/*
 * SplaySupport: adds splay to any Finger type.
 * If you are adding a custom finger, see the partially specialized class that adds splay support
 * to all ConfigurableFingers and use that as an example.
 */
template<typename BaseFinger>
class SplaySupport : public BaseFinger {
 public:
  SplaySupport(ConstructorArgs) : BaseFinger(all_args), splay_sensor(splay), splay_value(0) {}

  void readInput() override {
    BaseFinger::readInput();
    splay_value = splay_sensor->getValue();
  }

  inline int getEncodedSize() const override {
    return BaseFinger::getEncodedSize() + EncodedInput::SplaySize - 1;
  }

  int encode(char* output) const override {
    int offset = BaseFinger::encode(output);
    offset += snprintf(output + offset, EncodedInput::SplaySize, EncodedInput::SplayFormat,
                       this->type, splay_value);
    return offset;
  }

  virtual int splayValue() const {
    return splay_value;
  }

  void resetCalibration() override {
    BaseFinger::resetCalibration();
    splay_sensor->resetCalibration();
  }

  void enableCalibration() override {
    for (size_t i = 0; i < 3; i++) {
      splay_sensor->enableCalibration();
    }
  }

  void disableCalibration() override {
    for (size_t i = 0; i < 3; i++) {
      splay_sensor->disableCalibration();
    }
  }

 protected:
  Sensor* splay_sensor;
  int splay_value;
};

/*
 * This is a partially specialized class that allows ANY knuckle count, but always has splay enabled.
 */
template<size_t knuckle_count, size_t knuckle_offset>
class ConfigurableFinger<true, knuckle_count, knuckle_offset> :
  public SplaySupport<ConfigurableFinger<false, knuckle_count, knuckle_offset>> {
 public:
  ConfigurableFinger(ConstructorArgs) : SplaySupport<ConfigurableFinger<false, knuckle_count,
                                                                        knuckle_offset>
                                                    >(all_args) {}
};
