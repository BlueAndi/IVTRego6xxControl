# MIT License
#
# Copyright (c) 2025 Andreas Merkle (web@blue-andi.de)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

################################################################################
# Imports
################################################################################

import esphome.codegen as cg # Code generation API
import esphome.config_validation as cv # Configuration validation API
from esphome.components import sensor # Sensor component
from esphome.const import CONF_ID, CONF_UNIT_OF_MEASUREMENT, CONF_STATE_CLASS

################################################################################
# Variables
################################################################################

DEPENDENCIES = ["ivt_rego6xx_ctrl"]

# Namespace for the generated code.
ivt_rego6xx_ctrl_ns = cg.esphome_ns.namespace("ivt_rego6xx_ctrl")

# The class of the component.
ivt_rego6xx_sensor = ivt_rego6xx_ctrl_ns.class_(
    "IVTRego6xxSensor", sensor.Sensor
)

# Sensor variables
CONF_IVT_REGO6XX_CTRL_ID = "ivt_rego6xx_ctrl_id"
CONF_IVT_REGO6XX_VARIABLE = "ivt_rego6xx_variable"

# The configuration schema is automatically loaded by the ESPHome core and used to validate
# the provided configuration. See https://esphome.io/guides/contributing#config-validation
CONFIG_SCHEMA = sensor.SENSOR_SCHEMA.extend(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(ivt_rego6xx_sensor),

        # Mandatory variables
        cv.Required(CONF_IVT_REGO6XX_CTRL_ID): cv.use_id(ivt_rego6xx_ctrl_ns.IVTRego6xxCtrl),
        cv.Required(CONF_IVT_REGO6XX_VARIABLE): cv.string
    })
)

################################################################################
# Functions
################################################################################

async def to_code(config: dict) -> None:
    """
    Generate code for the IVT Rego 6xx control component.
    This runs only after the user input has been successfully validated.
    See https://esphome.io/guides/contributing#code-generation

    Args:
        config (dict): Configuration
    """
    # Create a new variable for the sensor.
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_IVT_REGO6XX_VARIABLE])
    await sensor.register_sensor(var, config)

    # Add the optional variables.
    if CONF_UNIT_OF_MEASUREMENT in config:
        cg.add(var.set_unit_of_measurement(config[CONF_UNIT_OF_MEASUREMENT]))

    if CONF_STATE_CLASS in config:
        cg.add(var.set_state_class(config[CONF_STATE_CLASS]))

    # Register sensor at the IVT Rego6xx control component.
    ivt_rego6xx_ctrl = await cg.get_variable(config[CONF_IVT_REGO6XX_CTRL_ID])
    cg.add(ivt_rego6xx_ctrl.registerSensor(var))

################################################################################
# Main
################################################################################
