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

import esphome.codegen as cg  # Code generation API
import esphome.config_validation as cv  # Configuration validation API
from esphome.components import number  # Number
from esphome.const import CONF_ID, CONF_UNIT_OF_MEASUREMENT, CONF_STATE_CLASS
from .. import ivt_rego6xx_ctrl_ns  # IVT Rego6xx control component namespace

################################################################################
# Variables
################################################################################

DEPENDENCIES = ["ivt_rego6xx_ctrl"]

# The class of the number.
ivt_rego6xx_number = ivt_rego6xx_ctrl_ns.class_(
    "IVTRego6xxNumber", number.Number
)

# Number variables
CONF_IVT_REGO6XX_CTRL_ID = "ivt_rego6xx_ctrl_id"
CONF_IVT_REGO6XX_CMD_READ = "ivt_rego6xx_ctrl_cmd_read"
CONF_IVT_REGO6XX_CMD_WRITE = "ivt_rego6xx_ctrl_cmd_write"
CONF_IVT_REGO6XX_ADDR = "ivt_rego6xx_ctrl_addr"
CONF_IVT_REGO6XX_MIN_VALUE = "ivt_rego6xx_ctrl_min_value"
CONF_IVT_REGO6XX_MAX_VALUE = "ivt_rego6xx_ctrl_max_value"
CONF_IVT_REGO6XX_STEP = "ivt_rego6xx_ctrl_step"

TEMPERATURE_RANGE_MIN = -100  # °C
TEMPERATURE_RANGE_MAX = 100  # °C
TEMPERATURE_STEP_MIN = 0.01  # K
TEMPERATURE_STEP_MAX = 1.00  # K

# The configuration schema is automatically loaded by the ESPHome core and used to validate
# the provided configuration. See https://esphome.io/guides/contributing#config-validation
CONFIG_SCHEMA = number.NUMBER_SCHEMA.extend(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(ivt_rego6xx_number),

        # Optional variables with default values
        cv.Optional(CONF_IVT_REGO6XX_MIN_VALUE, default=-40): cv.float_range(TEMPERATURE_RANGE_MIN, TEMPERATURE_RANGE_MAX),
        cv.Optional(CONF_IVT_REGO6XX_MAX_VALUE, default=50): cv.float_range(TEMPERATURE_RANGE_MIN, TEMPERATURE_RANGE_MAX),
        cv.Optional(CONF_IVT_REGO6XX_STEP, default=0.1): cv.float_range(TEMPERATURE_STEP_MIN, TEMPERATURE_STEP_MAX),

        # Mandatory variables
        cv.Required(CONF_IVT_REGO6XX_CTRL_ID): cv.use_id(ivt_rego6xx_ctrl_ns.IVTRego6xxCtrl),
        cv.Required(CONF_IVT_REGO6XX_CMD_READ): cv.hex_int_range(0x00, 0x7F),
        cv.Required(CONF_IVT_REGO6XX_CMD_WRITE): cv.hex_int_range(0x00, 0x7F),
        cv.Required(CONF_IVT_REGO6XX_ADDR): cv.hex_int_range(0x0000, 0x0300),
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
    # Create a new variable for the number.
    var = cg.new_Pvariable(config[CONF_ID],
                           config[CONF_IVT_REGO6XX_CMD_READ],
                           config[CONF_IVT_REGO6XX_CMD_WRITE],
                           config[CONF_IVT_REGO6XX_ADDR])
    await number.register_number(var,
                                 config,
                                 min_value=config[CONF_IVT_REGO6XX_MIN_VALUE],
                                 max_value=config[CONF_IVT_REGO6XX_MAX_VALUE],
                                 step=config[CONF_IVT_REGO6XX_STEP])

    # Add the optional variables.
    if CONF_UNIT_OF_MEASUREMENT in config:
        cg.add(var.set_unit_of_measurement(config[CONF_UNIT_OF_MEASUREMENT]))

    if CONF_STATE_CLASS in config:
        cg.add(var.set_state_class(config[CONF_STATE_CLASS]))

    # Register number at the IVT Rego6xx control component.
    ivt_rego6xx_ctrl = await cg.get_variable(config[CONF_IVT_REGO6XX_CTRL_ID])
    cg.add(ivt_rego6xx_ctrl.registerNumber(var))

################################################################################
# Main
################################################################################
