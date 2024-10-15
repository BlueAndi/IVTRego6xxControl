/* MIT License
 *
 * Copyright (c) 2024 Andreas Merkle <web@blue-andi.de>
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
 * @brief  System state: Error
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "ErrorState.h"

#include <Logging.h>
#include <Util.h>
#include <Board.h>
#include <SettingsService.h>

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

void ErrorState::entry(StateMachine& sm)
{
    UTIL_NOT_USED(sm);

    LOG_INFO("Enter error state.");

    m_timer.start(BLINK_ON_PERIOD);
    Board::ledOn();
    m_cnt = 0U;

    /* Any low level error happended and no error message can be shown
     * by the system message handler?
     */
    if (ERROR_ID_NO_ERROR != m_errorId)
    {
        LOG_INFO("Low level error E%u.", m_errorId);
    }
    else
    {
        LOG_INFO("No low level error, see display for more information.");
    }

    SettingsService::getInstance().stop();
}

void ErrorState::process(StateMachine& sm)
{
    UTIL_NOT_USED(sm);

    /* The error state is signalled with the onboard LED.
     * If a dedicated error id is set, the number of the error is blinked, so
     * the user can count it.
     */
    if (true == m_timer.isTimeout())
    {
        if (false == Board::isLedOn())
        {
            Board::ledOn();
            m_timer.start(BLINK_ON_PERIOD);
        }
        else
        {
            Board::ledOff();

            if (ERROR_ID_NO_ERROR == m_errorId)
            {
                m_timer.start(BLINK_OFF_SHORT_PERIOD);
            }
            else
            {
                ++m_cnt;

                if (m_cnt < static_cast<uint8_t>(m_errorId))
                {
                    m_timer.start(BLINK_OFF_SHORT_PERIOD);
                }
                else
                {
                    m_timer.start(BLINK_OFF_LONG_PERIOD);
                    m_cnt = 0U;
                }
            }
        }
    }

    /* Wait for manual reset. */
}

void ErrorState::exit(StateMachine& sm)
{
    UTIL_NOT_USED(sm);

    /* Nothing to do. */
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
