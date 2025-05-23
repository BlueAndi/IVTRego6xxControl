/* MIT License
 *
 * Copyright (c) 2020 - 2024 Andreas Merkle <web@blue-andi.de>
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
 * @brief  Rego6xx heatpump controller standard response
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Rego6xxStdRsp.h"
#include "Rego6xxUtil.h"

/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and classes
 *****************************************************************************/

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/******************************************************************************
 * Public Methods
 *****************************************************************************/

bool Rego6xxStdRsp::isValid() const
{
    bool isValid = false;

    if (false == isPending())
    {
        if (m_response[RSP_SIZE - 1U] == Rego6xxUtil::calculateChecksum(&m_response[1], RSP_SIZE - 2U))
        {
            isValid = true;
        }
    }

    return isValid;
}

uint8_t Rego6xxStdRsp::getDevAddr() const
{
    uint8_t devAddr = 0U;

    if ((false == isPending()) &&
        (true == isValid()))
    {
        devAddr = m_response[0];
    }

    return devAddr;
}

uint32_t Rego6xxStdRsp::getValue() const
{
    uint32_t data = 0U;

    if ((false == isPending()) &&
        (true == isValid()))
    {
        /* Common rules:
            - MSB first
            - 7 bit communication is used,
                e.g. register address 0x123456 in binary form 0001 0010 0011 0100 0101 0110
                will be expanded to 7bit form as 21-bit value 1001000 1101000 1010110
        */
        data  = ((uint16_t)(m_response[1] & 0x7FU)) << 14U;
        data |= ((uint16_t)(m_response[2] & 0x7FU)) <<  7U;
        data |= ((uint16_t)(m_response[3] & 0x7FU)) <<  0U;
    }

    return data;
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
