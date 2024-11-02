#include "sw3538.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace sw3538 {

static const char *const TAG = "sw3538";


void SW3538Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SW3538...");

  if (!this->write_byte(0x13, 0x02)) {
    this->mark_failed();
    return;
  }
}

void SW3538Component::dump_config() {
  ESP_LOGCONFIG(TAG, "SW3538:");
  LOG_I2C_DEVICE(this);

  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with SW3538 failed!");
    return;
  }
}

float SW3538Component::read_vin() {
  uint16_t raw_vin_data;

  uint8_t high_byte, low_byte;
  if (!this->read_byte(0x30, &high_byte) || !this->read_byte(0x32, &low_byte)) {
    this->status_set_warning();
    return NAN;
  }

  raw_vin_data = (high_byte << 4) | (low_byte >> 4);

  this->status_clear_warning();

  return raw_vin_data * 0.01f;  // 10mV/step
}

float SW3538Component::read_vout() {
  uint16_t raw_vout_data;
  uint8_t high_byte, low_byte;
  if (!this->read_byte(0x31, &high_byte) || !this->read_byte(0x32, &low_byte)) {
    this->status_set_warning();
    return NAN;
  }

  raw_vout_data = (high_byte << 4) | (low_byte & 0x0F);

  this->status_clear_warning();

  return raw_vout_data * 0.006f;  // 6mV/step
}

float SW3538Component::read_usbc_current() {
  uint16_t raw_iout1_data;
  uint8_t high_byte, low_byte;
  if (!this->read_byte(0x33, &high_byte) || !this->read_byte(0x35, &low_byte)) {
    this->status_set_warning();
    return NAN;
  }

  raw_iout1_data = (high_byte << 4) | (low_byte >> 4);

  this->status_clear_warning();

  return raw_iout1_data * 0.0025f;  // 2.5mA/step
}

float SW3538Component::read_usba_current() {
  uint16_t raw_iout2_data;
  uint8_t high_byte, low_byte;
  if (!this->read_byte(0x34, &high_byte) || !this->read_byte(0x35, &low_byte)) {
    this->status_set_warning();
    return NAN;
  }

  raw_iout2_data = (high_byte << 4) | (low_byte & 0x0F);

  this->status_clear_warning();

  return raw_iout2_data * 0.0025f;  // 2.5mA/step
}

float SW3538Component::read_temperature() {
  uint16_t raw_temp_data;
  uint8_t high_byte, low_byte;
  if (!this->read_byte(0x37, &high_byte) || !this->read_byte(0x38, &low_byte)) {
    this->status_set_warning();
    return NAN;
  }

  raw_temp_data = (high_byte << 4) | (low_byte & 0x0F);

  float temp = (1.0 / ((1.0 / 298.15) + (1.0 / 3435.0) * log((((raw_temp_data * 0.5e-3) / 100e-6) - 2000) / 10000.0))) - 273.15;

  this->status_clear_warning();

  return temp;
}

uint8_t SW3538Component::read_version() {
  uint8_t value;
  if (!this->read_byte(0x01, &value)) {
    this->status_set_warning();

    return 0;
  }

  this->status_clear_warning();

  return value & 0x07;
}

} // namespace sw3538
} // namespace esphome