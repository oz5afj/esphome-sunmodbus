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

#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace sunmodbus {

enum DataType {
  TYPE_UINT16,
  TYPE_INT16,
};

class SunModbus : public Component, public uart::UARTDevice {
 public:
  void set_slave_id(uint8_t id) { slave_id_ = id; }
  void set_start_address(uint16_t addr) { start_address_ = addr; }
  void set_count(uint16_t c) { count_ = c; }
  void set_update_interval(uint32_t ms) { update_interval_ = ms; }

  void add_sensor(sensor::Sensor *s, uint8_t offset, DataType type, float scale);

  void setup() override;
  void loop() override;

 protected:
  bool read_registers(uint16_t start, uint16_t count, uint16_t *buffer);

  uint8_t slave_id_{1};
  uint16_t start_address_{0};
  uint16_t count_{1};
  uint32_t update_interval_{1000};

  sensor::Sensor *sensor_{nullptr};
  uint8_t offset_{0};
  DataType type_{TYPE_UINT16};
  float scale_{1.0f};

  uint32_t last_update_{0};
};

}  // namespace sunmodbus
}  // namespace esphome
