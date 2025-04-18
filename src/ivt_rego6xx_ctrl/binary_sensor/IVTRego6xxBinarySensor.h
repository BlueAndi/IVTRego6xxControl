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
#include "esphome/components/binary_sensor/binary_sensor.h"

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
 * IVT Rego6xx binary sensor for ESPHome.
 */
class IVTRego6xxBinarySensor : public binary_sensor::BinarySensor
{
public:

    /**
     * Constructs the IVT rego6xx sensor.
     *
     * @param[in] cmdId Command id to send to the heatpump.
     * @param[in] addr  Address to read by the command.
     */
    IVTRego6xxBinarySensor(uint8_t cmdId, uint16_t addr) :
        m_cmdId(cmdId),
        m_addr(addr)
    {
    }

    /**
     * Destroys the IVT rego6xx sensor.
     */
    ~IVTRego6xxBinarySensor()
    {
    }

    /**
     * Get the command id to send to the heatpump.
     *
     * @return The command id to send to the heatpump.
     */
    uint8_t getCmdId() const
    {
        return m_cmdId;
    }

    /**
     * Get the address to read by the command.
     *
     * @return The address to read by the command.
     */
    uint16_t getAddr() const
    {
        return m_addr;
    }

private:

    uint8_t  m_cmdId; /**< Command id to send to the heatpump. */
    uint16_t m_addr;  /**< Address to read by the command. */

    /** No default constructor. */
    IVTRego6xxBinarySensor();
    /** No copy constructor. */
    IVTRego6xxBinarySensor(const IVTRego6xxBinarySensor& other)            = delete;
    /** No assignment operator. */
    IVTRego6xxBinarySensor& operator=(const IVTRego6xxBinarySensor& other) = delete;
    /** No move constructor. */
    IVTRego6xxBinarySensor(IVTRego6xxBinarySensor&& other)                 = delete;
};

} /* namespace ivt_rego6xx_ctrl */
} /* namespace esphome */

/******************************************************************************
 * Functions
 *****************************************************************************/

/** @} */