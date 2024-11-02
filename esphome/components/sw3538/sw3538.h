#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

#include <cinttypes>

namespace esphome {
namespace sw3538 {

class SW3538Component : public Component, public i2c::I2CDevice {
  public:
    void setup() override;
    void dump_config() override;

    float get_setup_priority() const override { return setup_priority::DATA; };

    float read_vin();
    float read_vout();
    float read_usbc_current();
    float read_usba_current();
    float read_temperature();
    uint8_t read_version();

  protected:

};

}
}