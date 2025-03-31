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

#include "SimpleTimer.hpp"
#include "StreamUartDevAdapter.h"
#include "sensor/IVTRego6xxSensor.h"
#include "binary_sensor/IVTRego6xxBinarySensor.h"
#include "text_sensor/IVTRego6xxTextSensor.h"

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
        m_pauseTimer(),
        m_rego6xxRsp(nullptr),
        m_displayRsp(nullptr),

        m_sensorTimer(),
        m_sensorCount(0U),
        m_sensors{ nullptr },
        m_currentSensorIndex(MAX_SENSORS),

        m_binarySensorTimer(),
        m_binarySensorCount(0U),
        m_binarySensors{ nullptr },
        m_currentBinarySensorIndex(MAX_BINARY_SENSORS),

        m_textSensorTimer(),
        m_textSensorCount(0U),
        m_textSensors{ nullptr },
        m_currentTextSensorIndex(MAX_TEXT_SENSORS)
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

    /**
     * Register a sensor.
     * This will be called during setup() by the code generated by ESPHome.
     *
     * @param[in] sensor    The sensor to register.
     */
    void registerSensor(IVTRego6xxSensor* sensor)
    {
        if ((nullptr != sensor) && (m_sensorCount < MAX_SENSORS))
        {
            m_sensors[m_sensorCount] = sensor;

            ++m_sensorCount;
        }
    }

    /**
     * Register a binary sensor.
     * This will be called during setup() by the code generated by ESPHome.
     *
     * @param[in] binarySensor  The binary sensor to register.
     */
    void registerBinarySensor(IVTRego6xxBinarySensor* binarySensor)
    {
        if ((nullptr != binarySensor) && (m_binarySensorCount < MAX_BINARY_SENSORS))
        {
            m_binarySensors[m_binarySensorCount] = binarySensor;

            ++m_binarySensorCount;
        }
    }

    /**
     * Register a text sensor.
     * This will be called during setup() by the code generated by ESPHome.
     *
     * @param[in] textSensor    The text sensor to register.
     */
    void registerTextSensor(IVTRego6xxTextSensor* textSensor)
    {
        if ((nullptr != textSensor) && (m_textSensorCount < MAX_TEXT_SENSORS))
        {
            m_textSensors[m_textSensorCount] = textSensor;

            ++m_textSensorCount;
        }
    }

private:

    /** Maximum number of sensors. */
    static const size_t MAX_SENSORS                 = 11U;

    /** Maximum number of binary sensors. */
    static const size_t MAX_BINARY_SENSORS          = 5U;

    /** Maximum number of text sensors. */
    static const size_t MAX_TEXT_SENSORS            = 4U;

    /** Duration in ms after the first time all kind of sensors are read. */
    static const uint32_t SENSOR_READ_INITIAL       = SIMPLE_TIMER_SECONDS(2U);

    /** Period in ms for reading sensors from heatpump. */
    static const uint32_t SENSOR_READ_PERIOD        = SIMPLE_TIMER_MINUTES(2U);

    /** Period in ms for reading binary sensors from heatpump. */
    static const uint32_t BINARY_SENSOR_READ_PERIOD = SIMPLE_TIMER_SECONDS(30U);

    /** Period in ms for reading text sensors from heatpump. */
    static const uint32_t TEXT_SENSOR_READ_PERIOD   = SIMPLE_TIMER_SECONDS(30U);

    /** Pause between every request to the heatpump controller in ms. */
    static const uint32_t    REGO6xx_REQ_PAUSE      = SIMPLE_TIMER_SECONDS(1U);

    StreamUartDevAdapter     m_adapter;    /**< Stream to UART device adapter. */
    Rego6xxCtrl              m_ctrl;       /**< IVT rego6xx controller. */
    SimpleTimer              m_pauseTimer; /**< Timer used to pause between each heatpump request. This shall avoid problems with the Rego6xx controller. */
    const Rego6xxStdRsp*     m_rego6xxRsp; /**< Pending Rego6xx response, used to read sensors and binary sensors. */
    const Rego6xxDisplayRsp* m_displayRsp; /**< Pending Rego6xx display response, used to read text sensors. */

    SimpleTimer              m_sensorTimer;          /**< Timer used to read cyclic all registered sensors values from the heatpump. */
    size_t                   m_sensorCount;          /**< Number of registered sensors. */
    IVTRego6xxSensor*        m_sensors[MAX_SENSORS]; /**< List of registered sensors. */
    size_t                   m_currentSensorIndex;   /**< Index of the current sensor to read. */

    SimpleTimer              m_binarySensorTimer;                 /**< Timer used to read cyclic all registered binary sensors values from the heatpump. */
    size_t                   m_binarySensorCount;                 /**< Number of registered binary sensors. */
    IVTRego6xxBinarySensor*  m_binarySensors[MAX_BINARY_SENSORS]; /**< List of registered binary sensors. */
    size_t                   m_currentBinarySensorIndex;          /**< Index of the current binary sensor to read. */

    SimpleTimer              m_textSensorTimer;                 /**< Timer used to read cyclic all registered text sensors values from the heatpump. */
    size_t                   m_textSensorCount;                 /**< Number of registered text sensors. */
    IVTRego6xxTextSensor*    m_textSensors[MAX_BINARY_SENSORS]; /**< List of registered text sensors. */
    size_t                   m_currentTextSensorIndex;          /**< Index of the current text sensor to read. */

    /**
     * Read all registered sensors.
     * This will be called periodically.
     */
    void readSensors();

    /**
     * Read all registered binary sensors.
     * This will be called periodically.
     */
    void readBinarySensors();

    /**
     * Read all registered text sensors.
     * This will be called periodically.
     */
    void readTextSensors();
};

} /* namespace ivt_rego6xx_ctrl */
} /* namespace esphome */

/******************************************************************************
 * Functions
 *****************************************************************************/

/** @} */