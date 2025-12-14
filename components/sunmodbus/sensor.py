# Copyright (C) 2025  Bo
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# Copyright (C) 2025  Bo
#
# GPL header osv…

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID, CONF_UART_ID

sunmodbus_ns = cg.esphome_ns.namespace("sunmodbus")
SunModbus = sunmodbus_ns.class_("SunModbus", cg.Component, uart.UARTDevice)

DataType = sunmodbus_ns.enum("DataType")

DEPENDENCIES = ["uart"]

CONF_SLAVE_ID = "slave_id"
CONF_START_ADDRESS = "start_address"
CONF_COUNT = "count"
CONF_OFFSET = "offset"
CONF_SCALE = "scale"
CONF_TYPE = "type"
CONF_UPDATE_INTERVAL = "update_interval"

DATA_TYPE_ENUM = cv.enum({
    "uint16": DataType.TYPE_UINT16,
    "int16": DataType.TYPE_INT16,
}, upper=False)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SunModbus),

    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required(CONF_SLAVE_ID): cv.int_range(min=1, max=247),
    cv.Required(CONF_START_ADDRESS): cv.int_range(min=0, max=65535),
    cv.Required(CONF_COUNT): cv.int_range(min=1, max=125),
    cv.Required(CONF_OFFSET): cv.int_range(min=0, max=255),
    cv.Required(CONF_SCALE): cv.float_,
    cv.Required(CONF_TYPE): DATA_TYPE_ENUM,
    cv.Optional(CONF_UPDATE_INTERVAL, default="1s"): cv.positive_time_period_milliseconds,

    # Sensor definition
    cv.Required("name"): cv.string,
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    uart_comp = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart(uart_comp))
    cg.add(var.set_slave_id(config[CONF_SLAVE_ID]))
    cg.add(var.set_start_address(config[CONF_START_ADDRESS]))
    cg.add(var.set_count(config[CONF_COUNT]))
    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))

    sens = await sensor.new_sensor(config)
    cg.add(var.add_sensor(
        sens,
        config[CONF_OFFSET],
        config[CONF_TYPE],
        config[CONF_SCALE],
    ))
