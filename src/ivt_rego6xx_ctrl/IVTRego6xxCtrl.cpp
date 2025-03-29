/* MIT License
 *
 * Copyright (c) 2019 - 2025 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief  IVT rego6xx controller component.
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "IVTRego6xxCtrl.h"
#include "esphome/core/log.h"

/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

namespace esphome
{
namespace ivt_rego6xx_ctrl
{

/******************************************************************************
 * Types and classes
 *****************************************************************************/

/** This type defines a mapping between a string and a system register. */
typedef struct
{
    const char*             name;       /**< Sensor name. */
    Rego6xxCtrl::SysRegAddr sysRegAddr; /**< Sensor system register address. */

} NameToSysReg;

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/**
 * Logger tag of this component.
 */
static const char* TAG                      = "ivt_rego6xx_ctrl.component";

/**
 * Mapping table between a string and a system register.
 */
static const NameToSysReg NAME_TO_SYS_REG[] = {
    { "gt1", Rego6xxCtrl::SYSREG_ADDR_GT1 },
    { "gt2", Rego6xxCtrl::SYSREG_ADDR_GT2 },
    { "gt3", Rego6xxCtrl::SYSREG_ADDR_GT3 },
    { "gt4", Rego6xxCtrl::SYSREG_ADDR_GT4 },
    { "gt5", Rego6xxCtrl::SYSREG_ADDR_GT5 },
    { "gt6", Rego6xxCtrl::SYSREG_ADDR_GT6 },
    { "gt8", Rego6xxCtrl::SYSREG_ADDR_GT8 },
    { "gt9", Rego6xxCtrl::SYSREG_ADDR_GT9 },
    { "gt10", Rego6xxCtrl::SYSREG_ADDR_GT10 },
    { "gt11", Rego6xxCtrl::SYSREG_ADDR_GT11 },
    { "gt3x", Rego6xxCtrl::SYSREG_ADDR_GT3X }
};

/******************************************************************************
 * Public Methods
 *****************************************************************************/

void IVTRego6xxCtrl::setup()
{
    /* Nothing to do. */
}

void IVTRego6xxCtrl::loop()
{
    m_ctrl.process();
}

void IVTRego6xxCtrl::dump_config()
{
    ESP_LOGCONFIG(TAG, "IVT rego6xx controller component");
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/


} /* namespace ivt_rego6xx_ctrl */
} /* namespace esphome */
