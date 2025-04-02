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
 * @brief  IVT rego6xx controller number.
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
#include "esphome/components/number/number.h"

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
 * IVT Rego6xx number for ESPHome.
 */
class IVTRego6xxNumber : public number::Number, public number::NumberTraits
{
public:

    /**
     * Constructs the IVT rego6xx number.
     *
     * @param[in] readCmdId     Command id to send to the heatpump to read value.
     * @param[in] writeCmdId    Command id to send to the heatpump to write value.
     * @param[in] addr          Address to read by the command.
     */
    IVTRego6xxNumber(uint8_t readCmdId, uint8_t writeCmdId, uint16_t addr) :
        m_readCmdId(readCmdId),
        m_writeCmdId(writeCmdId),
        m_addr(addr),
        m_value(0.0F),
        m_isUpdateRequested(false)
    {
    }

    /**
     * Destroys the IVT rego6xx number.
     */
    ~IVTRego6xxNumber()
    {
    }

    /**
     * Get the command id to send to the heatpump to read a value.
     *
     * @return The command id to send to the heatpump.
     */
    uint8_t getReadCmdId() const
    {
        return m_readCmdId;
    }

    /**
     * Get the command id to send to the heatpump to write a value.
     *
     * @return The command id to send to the heatpump.
     */
    uint8_t getWriteCmdId() const
    {
        return m_writeCmdId;
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

    /**
     * Is number update requested?
     *
     * @return If number update is requested, it will return true otherwise false.
     */
    bool isUpdateRequested() const
    {
        return m_isUpdateRequested;
    }

    /**
     * Get the value to write by the command.
     *
     * @return The value to write by the command.
     */
    float getValue()
    {
        m_isUpdateRequested = false;

        return m_value;
    }

private:

    uint8_t  m_readCmdId;         /**< Command id to send to the heatpump to read value. */
    uint8_t  m_writeCmdId;        /**< Command id to send to the heatpump to write value. */
    uint16_t m_addr;              /**< Address to read by the command. */
    float    m_value;             /**< Value to write by the command. */
    bool     m_isUpdateRequested; /**< Flag to indicate whether a number update is requested. */

    /** No default constructor. */
    IVTRego6xxNumber();
    /** No copy constructor. */
    IVTRego6xxNumber(const IVTRego6xxNumber& other)            = delete;
    /** No assignment operator. */
    IVTRego6xxNumber& operator=(const IVTRego6xxNumber& other) = delete;
    /** No move constructor. */
    IVTRego6xxNumber(IVTRego6xxNumber&& other)                 = delete;

    /**
     * Set the value of the number.
     *
     * @param[in] value The value as validated by the NumberCall.
     */
    void control(float value) override
    {
        m_value             = value;
        m_isUpdateRequested = true;
    }
};

} /* namespace ivt_rego6xx_ctrl */
} /* namespace esphome */

/******************************************************************************
 * Functions
 *****************************************************************************/

/** @} */