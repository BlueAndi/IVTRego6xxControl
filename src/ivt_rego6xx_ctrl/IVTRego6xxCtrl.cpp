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

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/**
 * Logger tag of this component.
 */
static const char* TAG = "ivt_rego6xx_ctrl.component";

/******************************************************************************
 * Public Methods
 *****************************************************************************/

void IVTRego6xxCtrl::setup()
{
    /* Start all timers responsible for reading from heatpump.
     * The exact order will be determined by the loop() method.
     */
    m_sensorTimer.start(SENSOR_READ_INITIAL);
    m_binarySensorTimer.start(SENSOR_READ_INITIAL);
}

void IVTRego6xxCtrl::loop()
{
    /* Pause? */
    if (true == m_pauseTimer.isTimerRunning())
    {
        /* Continue with any kind of next sensor? */
        if (true == m_pauseTimer.isTimeout())
        {
            m_pauseTimer.stop();
        }
    }

    if (false == m_pauseTimer.isTimerRunning())
    {
        /* Start or continue reading sensor, but only the bunch of binary sensors
         * are not complete read.
         */
        if ((true == m_sensorTimer.isTimerRunning()) &&
            (true == m_sensorTimer.isTimeout()) &&
            (MAX_BINARY_SENSORS == m_currentBinarySensorIndex))
        {
            readSensors();
        }
        /* Read binary sensors? */
        else if ((true == m_binarySensorTimer.isTimerRunning()) &&
                 (true == m_binarySensorTimer.isTimeout()))
        {
            readBinarySensors();
        }
        else
        {
            /* Nothing to do */
            ;
        }
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
    bool nextSensor = false;

    /* If no command is pending, request next sensor. */
    if (nullptr == m_rego6xxRsp)
    {
        /* If all sensors are read, start from the beginning. */
        if (m_sensorCount <= m_currentSensorIndex)
        {
            m_currentSensorIndex = 0U;
        }

        {
            IVTRego6xxSensor*       currentSensor = m_sensors[m_currentSensorIndex];
            Rego6xxCtrl::SysRegAddr sysRegAddr    = static_cast<Rego6xxCtrl::SysRegAddr>(currentSensor->getSysRegAddr());

            ESP_LOGI(TAG, "Request sensor '%s' at 0x%04X ...", currentSensor->get_name().c_str(), sysRegAddr);
            m_rego6xxRsp = m_ctrl.readSysReg(sysRegAddr);

            if (nullptr == m_rego6xxRsp)
            {
                ESP_LOGE(TAG, "Failed to read sensor '%s' at 0x%04X!", currentSensor->get_name().c_str(), sysRegAddr);
                nextSensor = true;
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

        nextSensor   = true;
    }
    else
    /* Wait for pending command response. */
    {
        /* Nothing to do */
        ;
    }

    if (true == nextSensor)
    {
        ++m_currentSensorIndex;

        /* Pause sending requests, after response. */
        m_pauseTimer.start(REGO6xx_REQ_PAUSE);

        if (m_sensorCount <= m_currentSensorIndex)
        {
            /* Start timer for next sensor read. */
            m_sensorTimer.start(SENSOR_READ_PERIOD);
        }
    }
}

void IVTRego6xxCtrl::readBinarySensors()
{
    bool nextSensor = false;

    /* If no command is pending, request next binary sensor. */
    if (nullptr == m_rego6xxBoolRsp)
    {
        /* If all sensors are read, start from the beginning. */
        if (m_binarySensorCount <= m_currentBinarySensorIndex)
        {
            m_currentBinarySensorIndex = 0U;
        }

        {
            IVTRego6xxBinarySensor*     currentBinarySensor = m_binarySensors[m_currentBinarySensorIndex];
            Rego6xxCtrl::FrontPanelAddr frontPanelAddr      = static_cast<Rego6xxCtrl::FrontPanelAddr>(currentBinarySensor->getSysRegAddr());

            ESP_LOGI(TAG, "Request binary sensor '%s' at 0x%04X ...", currentBinarySensor->get_name().c_str(), frontPanelAddr);
            m_rego6xxBoolRsp = m_ctrl.readFrontPanel(frontPanelAddr);

            if (nullptr == m_rego6xxBoolRsp)
            {
                ESP_LOGE(TAG, "Failed to read binary sensor '%s' at 0x%04X!", currentBinarySensor->get_name().c_str(), frontPanelAddr);
                nextSensor = true;
            }
        }
    }
    /* Command response received? */
    else if ((true == m_rego6xxBoolRsp->isUsed()) &&
             (false == m_rego6xxBoolRsp->isPending()))
    {
        /* The temperature is taken over only if the response is valid and there was no timeout. */
        if ((true == m_rego6xxBoolRsp->isValid()) &&
            (Rego6xxCtrl::DEV_ADDR_HOST == m_rego6xxBoolRsp->getDevAddr()))
        {
            IVTRego6xxBinarySensor* currentBinarySensor = m_binarySensors[m_currentBinarySensorIndex];
            bool                    state               = m_rego6xxBoolRsp->getValue();

            currentBinarySensor->publish_state(state);
        }
        else
        {
            /* Temperature skipped */
            ;
        }

        m_ctrl.release();
        m_rego6xxBoolRsp = nullptr;

        nextSensor       = true;
    }
    else
    /* Wait for pending command response. */
    {
        /* Nothing to do */
        ;
    }

    if (true == nextSensor)
    {
        ++m_currentBinarySensorIndex;

        /* Pause sending requests, after response. */
        m_pauseTimer.start(REGO6xx_REQ_PAUSE);

        if (m_binarySensorCount <= m_currentBinarySensorIndex)
        {
            /* Start timer for next sensor read. */
            m_binarySensorTimer.start(BINARY_SENSOR_READ_PERIOD);
        }
    }
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
