#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/voltage_sampler/voltage_sampler.h"

#include "../sw3538.h"

namespace esphome {
namespace sw3538 {

static const char *const TAG = "sw3538";

class SW3538Sensor : public sensor::Sensor,
                      public PollingComponent,
                      public Parented<SW3538Component> {
public:
  virtual float sample() = 0;
  void update() override {
    float v = this->sample();
    if (!std::isnan(v)) {
      this->publish_state(v);
    }
  }
};

class SW3538VinSensor : public SW3538Sensor,
                      public voltage_sampler::VoltageSampler {
public:
  float sample() override {
    return this->parent_->read_vin();
  }

  void dump_config() override {
    LOG_SENSOR("  ", "SW3538 VIN", this);
  }
};

class SW3538VoutSensor : public SW3538Sensor,
                      public voltage_sampler::VoltageSampler {
public:
  float sample() override {
    return this->parent_->read_vout();
  }

  void dump_config() override {
    LOG_SENSOR("  ", "SW3538 VOUT", this);
  }
};

class SW3538UsbCCurrentSensor : public SW3538Sensor {
  public:
    float sample() {
      return this->parent_->read_usbc_current();
    }

  void dump_config() override {
    LOG_SENSOR("  ", "SW3538 USB-C Current", this);
  }

 };

class SW3538UsbACurrentSensor : public SW3538Sensor {
  public:
    float sample() {
      return this->parent_->read_usba_current();
    }

  void dump_config() override {
    LOG_SENSOR("  ", "SW3538 USB-A Current", this);
  }

 };

class SW3538UsbCPowerSensor : public SW3538Sensor {
  public:
    float sample() {
      return this->parent_->read_usbc_current() * this->parent_->read_vout();
    }

  void dump_config() override {
    LOG_SENSOR("  ", "SW3538 USB-C Power", this);
  }

 };

class SW3538UsbAPowerSensor : public SW3538Sensor {
  public:
    float sample() {
      return this->parent_->read_usba_current() * this->parent_->read_vout();
    }

  void dump_config() override {
    LOG_SENSOR("  ", "SW3538 USB-A Power", this);
  }

 };

class SW3538TempSensor : public SW3538Sensor {
  public:
    float sample() {
      return this->parent_->read_temperature();
    }

  void dump_config() override {
    LOG_SENSOR("  ", "SW3538 Board Temperature", this);
  }

 };

} // namespace sw3538
} // namespace esphome