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

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

################################################################################
# Variables
################################################################################

DEPENDENCIES = ["uart"]

# UART ID (mandatory)
CONF_UART_ID = "uart_id"

# Namespace for the generated code.
ivt_rego6xx_ctrl_component_ns = cg.esphome_ns.namespace("ivt_rego6xx_ctrl")

# The class of the component.
ivt_rego6xx_ctrl_component = ivt_rego6xx_ctrl_component_ns.class_(
    "IVTRego6xxCtrlComponent", cg.Component, uart.UARTDevice
)

# The configuration schema is automatically loaded by the ESPHome core and used to validate
# the provided configuration. See https://esphome.io/guides/contributing#config-validation
CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(ivt_rego6xx_ctrl_component),
        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTDevice)
    })
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
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
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

################################################################################
# Main
################################################################################
