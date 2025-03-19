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
#include "StreamUartDevAdapter.h"

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
class IVTRego6xxCtrlComponent : public uart::UARTDevice, public Component
{
public:

    /**
     * Constructs the IVT rego6xx controller component.
     */
    IVTRego6xxCtrlComponent() :
        m_adapter(),
        m_ctrl(m_adapter)
    {
        m_adapter.setUartDevice(this);
    }

    /**
     * Destroys the IVT rego6xx controller component.
     */
    ~IVTRego6xxCtrlComponent()
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

private:

    StreamUartDevAdapter    m_adapter;   /**< Stream to UART device adapter. */
    Rego6xxCtrl             m_ctrl;      /**< IVT rego6xx controller. */
};

}  /* namespace ivt_rego6xx_ctrl */
}  /* namespace esphome */

/******************************************************************************
 * Functions
 *****************************************************************************/

 /** @} */