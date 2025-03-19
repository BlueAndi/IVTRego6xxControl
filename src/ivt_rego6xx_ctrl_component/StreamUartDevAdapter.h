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
 * @brief  Stream to UART device adapter
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

#include <Stream.h>
#include "esphome/components/uart/uart.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 * Stream to UART device adapter.
 */
class StreamUartDevAdapter : public Stream
{
public:

    /**
     * Constructs the adapter.
     */
    StreamUartDevAdapter() :
        m_uartDev(nullptr)
    {
    }

    /**
     * Destroys the adapter.
     */
    ~StreamUartDevAdapter()
    {
    }

    /**
     * Set UART device.
     *
     * @param[in] uartDev UART device
     */
    void setUartDevice(esphome::uart::UARTDevice* uartDev)
    {
        m_uartDev = uartDev;
    }

    int available() override;

    int read() override;

    int peek() override;

    size_t write(uint8_t) override;

private:

    esphome::uart::UARTDevice* m_uartDev; /**< UART device */

};

/******************************************************************************
 * Functions
 *****************************************************************************/

 /** @} */