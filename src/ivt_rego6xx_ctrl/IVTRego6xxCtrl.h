/* MIT License
 *
 * Copyright (c) 2025 Andreas Merkle <web@blue-andi.de>
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
 *
 * @addtogroup APP_LAYER
 *
 * @{
 */

#pragma once

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "Rego6xxCtrl.h"
#include "StreamUartDevAdapter.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** ESPHome namspace */
namespace esphome
{

/** IVT rego6xx controller namespace */
namespace ivt_rego6xx_ctrl
{

class TemperatureSensor
{
public:

    TemperatureSensor(const char* name, Rego6xxCtrl::SysRegAddr sysRegAddr) :
        m_name(name),
        m_sysRegAddr(sysRegAddr),
        m_value(0.0F)
    {
    }

    /**
     * Get temperature value in °C.
     * 
     * @return Temperature value in °C
     */
    float getValue() const
    {
        return m_value;
    }

    /**
     * Set temperature value in °C.
     * 
     * @param[in] value Temperature value in °C
     */
    void setValue(float value)
    {
        m_value = value;
    }

private:

    String                  m_name;        /**< Name of the sensor. */
    Rego6xxCtrl::SysRegAddr m_sysRegAddr;  /**< System register address. */
    float                   m_value;       /**< Temperature value in °C. */
};

/**
 * IVT Rego6xx controller component for ESPHome.
 */
class IVTRego6xxCtrl : public uart::UARTDevice, public Component
{
public:

    /**
     * Constructs the IVT rego6xx controller component.
     */
    IVTRego6xxCtrl() :
        m_adapter(),
        m_ctrl(m_adapter),
        m_temperatureSensorGT1("gt1", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT1),
        m_temperatureSensorGT2("gt2", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT2),
        m_temperatureSensorGT3("gt3", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT3),
        m_temperatureSensorGT4("gt4", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT4),
        m_temperatureSensorGT5("gt5", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT5),
        m_temperatureSensorGT6("gt6", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT6),
        m_temperatureSensorGT8("gt8", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT8),
        m_temperatureSensorGT9("gt9", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT9),
        m_temperatureSensorGT10("gt10", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT10),
        m_temperatureSensorGT11("gt11", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT11),
        m_temperatureSensorGT3X("gt3x", Rego6xxCtrl::SysRegAddr::SYSREG_ADDR_GT3X),
        m_temperatureSensors{
            &m_temperatureSensorGT1,
            &m_temperatureSensorGT2,
            &m_temperatureSensorGT3,
            &m_temperatureSensorGT4,
            &m_temperatureSensorGT5,
            &m_temperatureSensorGT6,
            &m_temperatureSensorGT8,
            &m_temperatureSensorGT9,
            &m_temperatureSensorGT10,
            &m_temperatureSensorGT11,
            &m_temperatureSensorGT3X
        }
    {
        m_adapter.setUartDevice(this);
    }

    /**
     * Destroys the IVT rego6xx controller component.
     */
    ~IVTRego6xxCtrl()
    {
    }

    /**
     * Initialize the IVT rego6xx controller component.
     */
    void setup() override;

    /**
     * Handle the loop.
     */
    void loop() override;

    /**
     * Dump the configuration of the component.
     */
    void dump_config() override;

private:

    StreamUartDevAdapter    m_adapter;              /**< Stream to UART device adapter. */
    Rego6xxCtrl             m_ctrl;                 /**< IVT rego6xx controller. */
    TemperatureSensor       m_temperatureSensorGT1; /**< Temperature sensor GT1. */
    TemperatureSensor       m_temperatureSensorGT2; /**< Temperature sensor GT2. */
    TemperatureSensor       m_temperatureSensorGT3; /**< Temperature sensor GT3. */
    TemperatureSensor       m_temperatureSensorGT4; /**< Temperature sensor GT4. */
    TemperatureSensor       m_temperatureSensorGT5; /**< Temperature sensor GT5. */
    TemperatureSensor       m_temperatureSensorGT6; /**< Temperature sensor GT6. */
    TemperatureSensor       m_temperatureSensorGT8; /**< Temperature sensor GT8. */
    TemperatureSensor       m_temperatureSensorGT9; /**< Temperature sensor GT9. */
    TemperatureSensor       m_temperatureSensorGT10; /**< Temperature sensor GT10. */
    TemperatureSensor       m_temperatureSensorGT11; /**< Temperature sensor GT11. */
    TemperatureSensor       m_temperatureSensorGT3X; /**< Temperature sensor GT3X. */
    TemperatureSensor*      m_temperatureSensors[]; /**< Temperature sensors. */
};

}  /* namespace ivt_rego6xx_ctrl */
}  /* namespace esphome */

/******************************************************************************
 * Functions
 *****************************************************************************/

 /** @} */