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
 * @brief  IVT rego6xx controller sensor.
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
#include <Arduino.h>
#include "esphome/components/sensor/sensor.h"

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
 * IVT Rego6xx controller sensor for ESPHome.
 */
class IVTRego6xxSensor : public sensor::Sensor
{
public:

    /**
     * Constructs the IVT rego6xx controller component.
     */
    IVTRego6xxSensor(const char* sensorType) :
        m_sensorType(sensorType)
    {
    }

    /**
     * Destroys the IVT rego6xx controller component.
     */
    ~IVTRego6xxSensor()
    {
    }

    /**
     * Get the sensor type.
     *
     * @return The sensor type.
     */
    const char* getSensorType() const
    {
        return m_sensorType;
    }

private:

    const char* m_sensorType; /**< Sensor type */

    /** No default constructor. */
    IVTRego6xxSensor();
    /** No copy constructor. */
    IVTRego6xxSensor(const IVTRego6xxSensor& other) = delete;
    /** No assignment operator. */
    IVTRego6xxSensor& operator=(const IVTRego6xxSensor& other) = delete;
    /** No move constructor. */
    IVTRego6xxSensor(IVTRego6xxSensor&& other) = delete;
};

} /* namespace ivt_rego6xx_ctrl */
} /* namespace esphome */

/******************************************************************************
 * Functions
 *****************************************************************************/

/** @} */