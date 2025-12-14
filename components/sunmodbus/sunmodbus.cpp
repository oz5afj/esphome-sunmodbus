/* Copyright (C) 2025  Bo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "sunmodbus.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sunmodbus {

static const char *const TAG = "sunmodbus";

void SunModbus::setup() {
  ESP_LOGI(TAG, "SunModbus setup: slave=%u start=%u count=%u interval=%ums",
           this->slave_id_, this->start_address_, this->count_, this->update_interval_);
  this->last_update_ = millis();
}

void SunModbus::loop() {
  uint32_t now = millis();
  if (now - this->last_update_ < this->update_interval_) {
    return;
  }
  this->last_update_ = now;

  if (this->sensor_ == nullptr) {
    ESP_LOGW(TAG, "No sensor configured");
    return;
  }

  uint16_t buffer[125];

  if (!this->read_registers(this->start_address_, this->count_, buffer)) {
    ESP_LOGW(TAG, "Failed to read registers");
    return;
  }

  if (this->offset_ >= this->count_) {
    ESP_LOGW(TAG, "Offset %u out of range (count=%u)", this->offset_, this->count_);
    return;
  }

  uint16_t raw = buffer[this->offset_];
  int16_t raw_signed = static_cast<int16_t>(raw);

  float value = (this->type_ == TYPE_UINT16)
                    ? static_cast<float>(raw) * this->scale_
                    : static_cast<float>(raw_signed) * this->scale_;

  ESP_LOGI(TAG, "Publishing value: raw=%u scaled=%.3f", raw, value);
  this->sensor_->publish_state(value);
}

void SunModbus::add_sensor(sensor::Sensor *s, uint8_t offset, DataType type, float scale) {
  this->sensor_ = s;
  this->offset_ = offset;
  this->type_ = type;
  this->scale_ = scale;
}

bool SunModbus::read_registers(uint16_t start, uint16_t count, uint16_t *buffer) {
  ESP_LOGI(TAG, "Dummy read_registers(start=%u, count=%u)", start, count);
  for (uint16_t i = 0; i < count; i++) {
    buffer[i] = 100 + i;
  }
  return true;
}

}  // namespace sunmodbus
}  // namespace esphome
