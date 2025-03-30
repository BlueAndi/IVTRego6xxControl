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
 * @brief  Rego6xx heatpump controller error response
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Rego6xxErrorRsp.h"
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

/**
 * Error messsage table ordered by error id.
 */
static const char* ERROR_MSGS[] =
{
    "Sensor radiator return (GT1)",
    "Outdoor sensor (GT2)",
    "Sensor hot water (GT3)",
    "Mixing valve sensor (GT4)",
    "Room sensor (GT5)",
    "Sensor compressor (GT6)",
    "Sensor heat tran fluid out (GT8)",
    "Sensor heat tran fluid in (GT9)",
    "Sensor cold tran fluid in (GT10)",
    "Sensor cold tran fluid in (GT11)",
    "Compresor circuit switch",
    "Electrical cassette",
    "HTF C=pump switch (MB2)",
    "Low pressure switch (LP)",
    "High pressure switch (HP)",
    "High return HP (GT9)",
    "HTF out max (GT8)",
    "HTF in under limit (GT10)",
    "HTF out under limit (GT11)",
    "Compressor superhear (GT6)",
    "3-phase incorrect order",
    "Power failure",
    "Varmetr. delta high"
};

/******************************************************************************
 * Public Methods
 *****************************************************************************/

bool Rego6xxErrorRsp::isValid() const
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

uint8_t Rego6xxErrorRsp::getDevAddr() const
{
    uint8_t devAddr = 0U;

    if ((false == isPending()) &&
        (true == isValid()))
    {
        devAddr = m_response[0];
    }

    return devAddr;
}

uint8_t Rego6xxErrorRsp::getErrorId() const
{
    uint8_t errorId = 0U;

    if ((false == isPending()) &&
        (true == isValid()))
    {
        const uint8_t   ERROR_ID_START_IDX  = 1U;
        uint8_t         column              = m_response[ERROR_ID_START_IDX + 0U] & 0x0FU;
        uint8_t         row                 = m_response[ERROR_ID_START_IDX + 1U] & 0x0FU;
        
        errorId = (column << 4U) | (row << 0U);
    }

    return errorId;
}

String Rego6xxErrorRsp::getErrorLog() const
{
    String  text;

    if ((false == isPending()) &&
        (true == isValid()))
    {
        const uint8_t   MAX_LEN         = 30U;
        const uint8_t   TEXT_START_IDX  = 3U;
        uint8_t         idx             = TEXT_START_IDX;

        /* Characters are coded as four bit pairs. First character informing
         * about column, second about row of character. For standard
         * characters is encoding same as computer character table, in that
         * case is possible to concat doubles and present it directly.
         */
        while((MAX_LEN + TEXT_START_IDX) > idx)
        {
            uint8_t column      = m_response[idx + 0U] & 0x0FU;
            uint8_t row         = m_response[idx + 1U] & 0x0FU;
            uint8_t character   = (column << 4U) | (row << 0U);

            text.concat(static_cast<char>(character));

            idx += 2U;
        }
    }

    return text;
}

const char* Rego6xxErrorRsp::getErrorDescription() const
{
    const char* description = "";

    if ((false == isPending()) &&
        (true == isValid()))
    {
        uint8_t errorId = getErrorId();

        if ((sizeof(ERROR_MSGS) / sizeof(ERROR_MSGS[0U])) <= errorId)
        {
            description = "?";
        }
        else
        {
            description = ERROR_MSGS[errorId];
        }
    }

    return description;
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
