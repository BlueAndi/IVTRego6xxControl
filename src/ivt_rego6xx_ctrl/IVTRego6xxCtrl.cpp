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
#include <string>

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
     * The exact order will be determined by the state machine.
     */
    m_sensorTimer.start(SENSOR_READ_INITIAL);
    m_binarySensorTimer.start(SENSOR_READ_INITIAL);
    m_textSensorTimer.start(SENSOR_READ_INITIAL);
}

void IVTRego6xxCtrl::loop()
{
    /* Between each heatpump command/response there shall be a pause to avoid
     * the Rego6xx controller to be overloaded.
     */
    if (true == m_pauseTimer.isTimerRunning())
    {
        /* Continue heatpump communication? */
        if (true == m_pauseTimer.isTimeout())
        {
            m_pauseTimer.stop();

            /* Check buttons first for fast user experience. */
            m_state = STATE_BUTTONS;
        }
    }

    if (false == m_pauseTimer.isTimerRunning())
    {
        processStateMachine();
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

IVTRego6xxCtrl::State IVTRego6xxCtrl::getPendingState(IVTRego6xxCtrl::State currentState)
{
    State prevState = currentState;

    if (m_sensorCount > m_currentSensorIndex)
    {
        prevState = STATE_SENSORS;
    }
    else if (m_binarySensorCount > m_currentBinarySensorIndex)
    {
        prevState = STATE_BINARY_SENSORS;
    }
    else if (m_textSensorCount > m_currentTextSensorIndex)
    {
        prevState = STATE_TEXT_SENSORS;
    }
    else if (m_buttonCount > m_currentButtonIndex)
    {
        prevState = STATE_BUTTONS;
    }
    else
    {
        ;
    }

    return prevState;
}

void IVTRego6xxCtrl::processStateMachine()
{
    State nextState = m_state;

    switch (m_state)
    {
    case STATE_BUTTONS:
        /* If all buttons handled, continue with the pending state.
         * The pending state is the one, which has been paused before
         * because of the button press.
         */
        if (false == processButtons())
        {
            nextState = getPendingState(STATE_BUTTONS);

            if (STATE_BUTTONS == nextState)
            {
                nextState = STATE_SENSORS;
            }
        }
        break;

    case STATE_SENSORS:
        if (false == processSensors())
        {
            nextState = STATE_BINARY_SENSORS;
        }
        break;

    case STATE_BINARY_SENSORS:
        if (false == processBinarySensors())
        {
            nextState = STATE_TEXT_SENSORS;
        }
        break;

    case STATE_TEXT_SENSORS:
        if (false == processTextSensors())
        {
            nextState = STATE_SENSORS;
        }
        break;

    default:
        nextState = STATE_BUTTONS;
        break;
    }

    m_state = nextState;
}

bool IVTRego6xxCtrl::processButtons()
{
    bool isPending = false;

    /* If no command is pending, request next button. */
    if (nullptr == m_confirmRsp)
    {
        size_t buttonIndex;

        /* If all buttons are handled, start from the beginning. */
        if (m_buttonCount <= m_currentButtonIndex)
        {
            m_currentButtonIndex = 0U;
        }
        else
        {
            ++m_currentButtonIndex;
        }

        while (m_buttonCount > m_currentButtonIndex)
        {
            IVTRego6xxButton* currentButton = m_buttons[m_currentButtonIndex];
            bool              isPressed     = currentButton->isPressed();

            if (true == isPressed)
            {
                uint8_t  cmdId = currentButton->getCmdId();
                uint16_t addr  = currentButton->getAddr();
                uint16_t value = currentButton->getValue();

                ESP_LOGI(TAG, "Write button '%s' 0x%04X with 0x%02X (cmd id) at 0x%04X ...", currentButton->get_name().c_str(), value, cmdId, addr);
                m_confirmRsp = m_ctrl.writeStd(cmdId, addr, value);

                if (nullptr == m_confirmRsp)
                {
                    ESP_LOGE(TAG, "Failed to write button '%s' 0x%04X with 0x%02X (cmd id) at 0x%04X!", currentButton->get_name().c_str(), value, cmdId, addr);
                }
                else
                {
                    break;
                }
            }

            ++m_currentButtonIndex;
        }
    }
    /* Command response received? */
    else if ((true == m_confirmRsp->isUsed()) &&
             (false == m_confirmRsp->isPending()))
    {
        IVTRego6xxButton* currentButton = m_buttons[m_currentButtonIndex];

        if (true == m_confirmRsp->isTimeout())
        {
            ESP_LOGW(TAG, "Write button '%s' response timeout.", currentButton->get_name().c_str());
        }
        else if (false == m_confirmRsp->isValid())
        {
            ESP_LOGW(TAG, "Write button '%s' response invalid.", currentButton->get_name().c_str());
        }
        else if (Rego6xxCtrl::DEV_ADDR_HOST == m_confirmRsp->getDevAddr())
        {
            ESP_LOGW(TAG, "Write button '%s' response has wrong destination.", currentButton->get_name().c_str());
        }
        else
        {
            ESP_LOGI(TAG, "Write button '%s' successful.", currentButton->get_name().c_str());
        }

        m_ctrl.release();
        m_confirmRsp = nullptr;

        ++m_currentButtonIndex;

        /* Pause until next sensor will be read. */
        m_pauseTimer.start(REGO6xx_REQ_PAUSE);
    }
    else
    /* Wait for pending command response. */
    {
        /* Nothing to do */
        ;
    }

    if (m_buttonCount > m_currentButtonIndex)
    {
        /* Continue reading the buttons. */
        isPending = true;
    }

    return isPending;
}

bool IVTRego6xxCtrl::processSensors()
{
    bool isPending = false;

    /* Is it time to start reading the sensors or reading already in progess? */
    if (((true == m_sensorTimer.isTimerRunning()) && (true == m_sensorTimer.isTimeout())) ||
        (m_sensorCount > m_currentSensorIndex))
    {
        readSensors();
    }

    if (m_sensorCount > m_currentSensorIndex)
    {
        /* Continue reading the sensors. */
        isPending = true;
    }

    return isPending;
}

bool IVTRego6xxCtrl::processBinarySensors()
{
    bool isPending = false;

    /* Is it time to start reading the binary sensors or reading already in progess? */
    if (((true == m_binarySensorTimer.isTimerRunning()) && (true == m_binarySensorTimer.isTimeout())) ||
        (m_binarySensorCount > m_currentBinarySensorIndex))
    {
        readBinarySensors();
    }

    if (m_binarySensorCount > m_currentBinarySensorIndex)
    {
        /* Continue reading the binary sensors. */
        isPending = true;
    }

    return isPending;
}

bool IVTRego6xxCtrl::processTextSensors()
{
    bool isPending = false;

    /* Is it time to start reading the text sensors or reading already in progess? */
    if (((true == m_textSensorTimer.isTimerRunning()) && (true == m_textSensorTimer.isTimeout())) ||
        (m_textSensorCount > m_currentTextSensorIndex))
    {
        readTextSensors();
    }

    if (m_textSensorCount > m_currentTextSensorIndex)
    {
        /* Continue reading the text sensors. */
        isPending = true;
    }

    return isPending;
}

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

            /* Start timer for next sensor read immediately to keep the cycle. */
            m_sensorTimer.start(SENSOR_READ_PERIOD);
        }

        {
            IVTRego6xxSensor* currentSensor = m_sensors[m_currentSensorIndex];
            uint8_t           cmdId         = currentSensor->getCmdId();
            uint16_t          addr          = currentSensor->getAddr();

            ESP_LOGI(TAG, "Read sensor '%s' with 0x%02X (cmd id) at 0x%04X ...", currentSensor->get_name().c_str(), cmdId, addr);
            m_rego6xxRsp = m_ctrl.readStd(cmdId, addr);

            if (nullptr == m_rego6xxRsp)
            {
                ESP_LOGE(TAG, "Failed to read sensor '%s' with 0x%02X (cmd id) at 0x%04X!", currentSensor->get_name().c_str(), cmdId, addr);
                nextSensor = true;
            }
        }
    }
    /* Command response received? */
    else if ((true == m_rego6xxRsp->isUsed()) &&
             (false == m_rego6xxRsp->isPending()))
    {
        IVTRego6xxSensor* currentSensor = m_sensors[m_currentSensorIndex];

        if (true == m_rego6xxRsp->isTimeout())
        {
            ESP_LOGW(TAG, "Read sensor '%s' response timeout.", currentSensor->get_name().c_str());
        }
        else if (false == m_rego6xxRsp->isValid())
        {
            ESP_LOGW(TAG, "Read sensor '%s' response invalid.", currentSensor->get_name().c_str());
        }
        else if (Rego6xxCtrl::DEV_ADDR_HOST == m_rego6xxRsp->getDevAddr())
        {
            ESP_LOGW(TAG, "Read sensor '%s' response has wrong destination.", currentSensor->get_name().c_str());
        }
        else
        {
            float value = m_ctrl.toFloat(m_rego6xxRsp->getValue());

            currentSensor->publish_state(value);

            ESP_LOGI(TAG, "Read sensor '%s' successful.", currentSensor->get_name().c_str());
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

        /* Pause until next sensor will be read. */
        m_pauseTimer.start(REGO6xx_REQ_PAUSE);
    }
}

void IVTRego6xxCtrl::readBinarySensors()
{
    bool nextSensor = false;

    /* If no command is pending, request next binary sensor. */
    if (nullptr == m_rego6xxRsp)
    {
        /* If all sensors are read, start from the beginning. */
        if (m_binarySensorCount <= m_currentBinarySensorIndex)
        {
            m_currentBinarySensorIndex = 0U;

            /* Start timer for next binary sensor read immediately to keep the cycle. */
            m_binarySensorTimer.start(BINARY_SENSOR_READ_PERIOD);
        }

        {
            IVTRego6xxBinarySensor* currentBinarySensor = m_binarySensors[m_currentBinarySensorIndex];
            uint8_t                 cmdId               = currentBinarySensor->getCmdId();
            uint16_t                addr                = currentBinarySensor->getAddr();

            ESP_LOGI(TAG, "Read binary sensor '%s' with 0x%02X (cmd id) at 0x%04X ...", currentBinarySensor->get_name().c_str(), cmdId, addr);
            m_rego6xxRsp = m_ctrl.readStd(cmdId, addr);

            if (nullptr == m_rego6xxRsp)
            {
                ESP_LOGE(TAG, "Failed to read binary sensor '%s' with 0x%02X (cmd id) at 0x%04X!", currentBinarySensor->get_name().c_str(), cmdId, addr);
                nextSensor = true;
            }
        }
    }
    /* Command response received? */
    else if ((true == m_rego6xxRsp->isUsed()) &&
             (false == m_rego6xxRsp->isPending()))
    {
        IVTRego6xxBinarySensor* currentBinarySensor = m_binarySensors[m_currentBinarySensorIndex];

        if (true == m_rego6xxRsp->isTimeout())
        {
            ESP_LOGW(TAG, "Read binary sensor '%s' response timeout.", currentBinarySensor->get_name().c_str());
        }
        else if (false == m_rego6xxRsp->isValid())
        {
            ESP_LOGW(TAG, "Read binary sensor '%s' response invalid.", currentBinarySensor->get_name().c_str());
        }
        else if (Rego6xxCtrl::DEV_ADDR_HOST == m_rego6xxRsp->getDevAddr())
        {
            ESP_LOGW(TAG, "Read binary sensor '%s' response has wrong destination.", currentBinarySensor->get_name().c_str());
        }
        else
        {
            bool state = m_ctrl.toBool(m_rego6xxRsp->getValue());

            currentBinarySensor->publish_state(state);

            ESP_LOGI(TAG, "Read binary sensor '%s' successful.", currentBinarySensor->get_name().c_str());
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
        ++m_currentBinarySensorIndex;

        /* Pause until next binary sensor will be read. */
        m_pauseTimer.start(REGO6xx_REQ_PAUSE);
    }
}

void IVTRego6xxCtrl::readTextSensors()
{
    bool nextSensor = false;

    /* If no command is pending, request next text sensor. */
    if (nullptr == m_displayRsp)
    {
        /* If all sensors are read, start from the beginning. */
        if (m_textSensorCount <= m_currentTextSensorIndex)
        {
            m_currentTextSensorIndex = 0U;

            /* Start timer for next text sensor read immediately to keep the cycle. */
            m_textSensorTimer.start(TEXT_SENSOR_READ_PERIOD);
        }

        {
            IVTRego6xxTextSensor* currentTextSensor = m_textSensors[m_currentTextSensorIndex];
            uint8_t               cmdId             = currentTextSensor->getCmdId();
            uint16_t              addr              = currentTextSensor->getAddr();

            ESP_LOGI(TAG, "Read text sensor '%s' with 0x%02X (cmd id) at 0x%04X ...", currentTextSensor->get_name().c_str(), cmdId, addr);
            m_displayRsp = m_ctrl.readDisplay(cmdId, addr);

            if (nullptr == m_displayRsp)
            {
                ESP_LOGE(TAG, "Failed to read text sensor '%s' with 0x%02X (cmd id) at 0x%04X!", currentTextSensor->get_name().c_str(), cmdId, addr);
                nextSensor = true;
            }
        }
    }
    /* Command response received? */
    else if ((true == m_displayRsp->isUsed()) &&
             (false == m_displayRsp->isPending()))
    {
        IVTRego6xxTextSensor* currentTextSensor = m_textSensors[m_currentTextSensorIndex];

        if (true == m_displayRsp->isTimeout())
        {
            ESP_LOGW(TAG, "Read text sensor '%s' response timeout.", currentTextSensor->get_name().c_str());
        }
        else if (false == m_displayRsp->isValid())
        {
            ESP_LOGW(TAG, "Read text sensor '%s' response invalid.", currentTextSensor->get_name().c_str());
        }
        else if (Rego6xxCtrl::DEV_ADDR_HOST == m_displayRsp->getDevAddr())
        {
            ESP_LOGW(TAG, "Read text sensor '%s' response has wrong destination.", currentTextSensor->get_name().c_str());
        }
        else
        {
            std::string msg = m_displayRsp->getMsg().c_str();

            currentTextSensor->publish_state(msg);

            ESP_LOGI(TAG, "Read text sensor '%s' successful.", currentTextSensor->get_name().c_str());
        }

        m_ctrl.release();
        m_displayRsp = nullptr;

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
        ++m_currentTextSensorIndex;

        /* Pause until next binary sensor will be read. */
        m_pauseTimer.start(REGO6xx_REQ_PAUSE);
    }
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/

} /* namespace ivt_rego6xx_ctrl */
} /* namespace esphome */
