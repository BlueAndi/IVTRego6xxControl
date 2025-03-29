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
    m_timer.start(SENSOR_READ_INITIAL);
}

void IVTRego6xxCtrl::loop()
{
    /* Read sensors? */
    if ((true == m_timer.isTimerRunning()) &&
        (true == m_timer.isTimeout()))
    {
    }

    /* Process the heatpump Rego6xx controller. */
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

void IVTRego6xxCtrl::readSensors()
{
    /* If no command is pending, request next sensor. */
    if (nullptr == m_rego6xxRsp)
    {
        /* If the pause between two requests is finished, go ahead with next sensor. */
        if ((false == m_pauseTimer.isTimerRunning()) ||
            (true == m_pauseTimer.isTimeout()))
        {
            /* If all sensors are read, start from the beginning. */
            if (m_sensorCount <= m_currentSensorIndex)
            {
                m_currentSensorIndex = 0U;
                m_timer.start(SENSOR_READ_PERIOD);
            }
            else
            {
                IVTRego6xxSensor*       currentSensor = m_sensors[m_currentSensorIndex];
                Rego6xxCtrl::SysRegAddr sysRegAddr;
                bool                    isFound = getSysRegAddr(currentSensor->getSensorType(), sysRegAddr);

                if (false == isFound)
                {
                    ESP_LOGE(TAG, "Invalid sensor type '%s'!", currentSensor->getSensorType());
                }
                else
                {
                    m_rego6xxRsp = m_ctrl.readSysReg(sysRegAddr);

                    if (nullptr == m_rego6xxRsp)
                    {
                        ESP_LOGE(TAG, "Failed to read sensor '%s'!", currentSensor->getSensorType());
                    }
                }
            }
        }
    }
    /* Command response received? */
    else if ((true == m_rego6xxRsp->isUsed()) &&
             (false == m_rego6xxRsp->isPending()))
    {
        /* The temperature is taken over only if the response is valid and there was no timeout. */
        if ((true == m_rego6xxRsp->isValid()) &&
            (Rego6xxCtrl::DEV_ADDR_HOST == m_rego6xxRsp->getDevAddr()))
        {
            IVTRego6xxSensor* currentSensor = m_sensors[m_currentSensorIndex];
            float             temperature   = calculateTemperature(m_rego6xxRsp->getValue());

            currentSensor->publish_state(temperature);
        }
        else
        {
            /* Temperature skipped */
            ;
        }

        m_ctrl.release();
        m_rego6xxRsp = nullptr;

        /* Pause sending requests, after response. */
        m_timer.start(REGO6xx_REQ_PAUSE);
    }
    else
    /* Wait for pending command response. */
    {
        /* Nothing to do */
        ;
    }
}

bool IVTRego6xxCtrl::getSysRegAddr(const char* sensorType, Rego6xxCtrl::SysRegAddr& sysRegAddr)
{
    bool   isFound = false;
    size_t index   = 0U;

    for (index = 0U; index < (sizeof(NAME_TO_SYS_REG) / sizeof(NAME_TO_SYS_REG[0])); ++index)
    {
        if (0 == strcmp(sensorType, NAME_TO_SYS_REG[index].name))
        {
            sysRegAddr = NAME_TO_SYS_REG[index].sysRegAddr;
            isFound    = true;
            break;
        }
    }

    return isFound;
}

float IVTRego6xxCtrl::calculateTemperature(uint16_t rawTemperature)
{
    int8_t sign = 1;
    float  floorPart;
    float  fracPart;
    float  temperature;

    if (0U != (rawTemperature & 0x8000U))
    {
        rawTemperature = (0xFFFFU - rawTemperature) + 1U;
        sign           = -1;
    }

    floorPart   = sign * static_cast<int8_t>(rawTemperature / 10U);
    fracPart    = static_cast<uint8_t>(rawTemperature % 10U) / 10.0F;

    temperature = (0.0f > floorPart) ? (floorPart - fracPart) : (floorPart + fracPart);

    return temperature;
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/


} /* namespace ivt_rego6xx_ctrl */
} /* namespace esphome */
