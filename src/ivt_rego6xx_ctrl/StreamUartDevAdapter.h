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

    /**
     * Available bytes in the input buffer.
     * 
     * @return Number of bytes available in the input buffer.
     */
    int available() override;

    /**
     * Read a byte from the input buffer.
     * 
     * @return Byte from the input buffer or -1 if no byte is available.
     */
    int read() override;

    /**
     * Peek a byte from the input buffer.
     * 
     * @return Byte from the input buffer or -1 if no byte is available.
     */
    int peek() override;

    /**
     * Write a byte to the output buffer.
     * 
     * @return Number of bytes written.
     */
    size_t write(uint8_t data) override;

private:

    esphome::uart::UARTDevice* m_uartDev; /**< UART device */

};

/******************************************************************************
 * Functions
 *****************************************************************************/

 /** @} */