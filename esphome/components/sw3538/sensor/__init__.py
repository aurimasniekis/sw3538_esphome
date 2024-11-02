import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, voltage_sampler
from esphome.const import (
    CONF_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_WATT,
    UNIT_CELSIUS,
)
from .. import sw3538_ns, SW3538Component, CONF_SW3538_ID

CONF_VIN_VOLTAGE = "vin_voltage"
CONF_VOUT_VOLTAGE = "vout_voltage"
CONF_USBC_CURRENT = "usbc_current"
CONF_USBA_CURRENT = "usba_current"
CONF_USBC_POWER = "usbc_power"
CONF_USBA_POWER = "usba_power"

AUTO_LOAD = ["voltage_sampler"]
DEPENDENCIES = ["sw3538"]

SW3538VinSensor = sw3538_ns.class_(
    "SW3538VinSensor", sensor.Sensor, cg.PollingComponent, voltage_sampler.VoltageSampler
)

SW3538VoutSensor = sw3538_ns.class_(
    "SW3538VoutSensor", sensor.Sensor, cg.PollingComponent, voltage_sampler.VoltageSampler
)

SW3538UsbCCurrentSensor = sw3538_ns.class_(
    "SW3538UsbCCurrentSensor", sensor.Sensor, cg.PollingComponent
)

SW3538UsbACurrentSensor = sw3538_ns.class_(
    "SW3538UsbACurrentSensor", sensor.Sensor, cg.PollingComponent
)

SW3538UsbCPowerSensor = sw3538_ns.class_(
    "SW3538UsbCPowerSensor", sensor.Sensor, cg.PollingComponent
)

SW3538UsbAPowerSensor = sw3538_ns.class_(
    "SW3538UsbAPowerSensor", sensor.Sensor, cg.PollingComponent
)

SW3538TempSensor = sw3538_ns.class_(
    "SW3538TempSensor", sensor.Sensor, cg.PollingComponent
)

SENSOR_TYPES = {
    CONF_VIN_VOLTAGE: sensor.sensor_schema(
        SW3538VinSensor,
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_VOUT_VOLTAGE: sensor.sensor_schema(
        SW3538VoutSensor,
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_USBC_CURRENT: sensor.sensor_schema(
        SW3538UsbCCurrentSensor,
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_CURRENT,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_USBA_CURRENT: sensor.sensor_schema(
        SW3538UsbACurrentSensor,
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_CURRENT,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_USBC_POWER: sensor.sensor_schema(
        SW3538UsbCPowerSensor,
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_POWER,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_USBA_POWER: sensor.sensor_schema(
        SW3538UsbAPowerSensor,
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_POWER,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_TEMPERATURE: sensor.sensor_schema(
        SW3538TempSensor,
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
}

CONFIG_SCHEMA = (cv.Schema(
    {
        cv.GenerateID(CONF_SW3538_ID): cv.use_id(SW3538Component),
    }
)

 .extend({cv.Optional(type_): schema.extend(cv.polling_component_schema("5s")) for type_, schema in SENSOR_TYPES.items()}))

async def to_code(config):
    paren = await cg.get_variable(config[CONF_SW3538_ID])

    for type_ in SENSOR_TYPES:
        if conf := config.get(type_):
            sens = await sensor.new_sensor(conf)
            await cg.register_parented(sens, paren)
            await cg.register_component(sens, conf)
