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
 * @brief  Rego6xx heatpump controller
 * @author Andreas Merkle <web@blue-andi.de>
 *
 * @{
 */

#ifndef __REGO6XX_CTRL_H__
#define __REGO6XX_CTRL_H__

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Arduino.h>
#include "Rego6xxStdRsp.h"
#include "Rego6xxConfirmRsp.h"
#include "Rego6xxErrorRsp.h"
#include "Rego6xxBoolRsp.h"
#include "Rego6xxDisplayRsp.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/******************************************************************************
 * Functions
 *****************************************************************************/

/**
 * Rego6xx heatpump controller.
 */
class Rego6xxCtrl
{
public:

    /** Commands of the heat pump regulator */
    enum CmdId : uint8_t
    {
        CMD_ID_READ_FRONT_PANEL  = 0x00, /**< Read from front panel (register 0x09ff) */
        CMD_ID_WRITE_FRONT_PANEL = 0x01, /**< Write to front panel (register 0x09ff) */
        CMD_ID_READ_SYSTEM_REG   = 0x02, /**< Read system register (register 0x1345) */
        CMD_ID_WRITE_SYSTEM_REG  = 0x03, /**< Write system register (register 0x1345) */
        CMD_ID_READ_TIMER_REG    = 0x04, /**< Read timer register (register 0x1b45) */
        CMD_ID_WRITE_TIMER_REG   = 0x05, /**< Write timer register (register 0x1b45) */
        CMD_ID_READ_REG_1B61     = 0x06, /**< Read register 1B61 (register 0x1b61) */
        CMD_ID_WRITE_REG_1B61    = 0x07, /**< Write register 1B61 (register 0x1b61) */
        CMD_ID_READ_DISPLAY      = 0x20, /**< Read display */
        CMD_ID_READ_LAST_ERROR   = 0x40, /**< Read last error line */
        CMD_ID_READ_PREV_ERROR   = 0x42, /**< Read previous error line */
        CMD_ID_READ_REGO_VERSION = 0x7f  /**< Read REGO version */
    };

    /** System register adresses of Rego600 */
    enum SysRegAddr : uint16_t
    {
        /* Settings */
        SYSREG_ADDR_HEAT_CURVE          = 0x0000, /**< Heat curve */
        SYSREG_ADDR_HEAT_CURVE_FINE_ADJ = 0x0001, /**< Heat curve fine adjustment */
        SYSREG_ADDR_INDOOR_TEMP         = 0x0021, /**< Indoor temperature settings */
        SYSREG_ADDR_CURVE               = 0x0022, /**< Curve influenced by indoor temperature */
        SYSREG_ADDR_CURVE_ADJ_20        = 0x001e, /**< Adjust curve at 20 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_15        = 0x001c, /**< Adjust curve at 15 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_10        = 0x001a, /**< Adjust curve at 10 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_5         = 0x0018, /**< Adjust curve at 5 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_0         = 0x0016, /**< Adjust curve at 0 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_M5        = 0x0014, /**< Adjust curve at -5 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_M10       = 0x0012, /**< Adjust curve at -10 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_M15       = 0x0010, /**< Adjust curve at -15 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_M20       = 0x000e, /**< Adjust curve at -20 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_M25       = 0x000c, /**< Adjust curve at -25 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_M30       = 0x000a, /**< Adjust curve at -30 degree outdoor temperature */
        SYSREG_ADDR_CURVE_ADJ_M35       = 0x0008, /**< Adjust curve at -35 degree outdoor temperature */
        SYSREG_ADDR_HEAT_CURVE_DIFF     = 0x0002, /**< Heat curve coupling difference */

        /* Control data */
        SYSREG_ADDR_GT1_TARGET          = 0x006e, /**< GT1 target value */
        SYSREG_ADDR_GT1_ON              = 0x006f, /**< GT1 on value */
        SYSREG_ADDR_GT1_OFF             = 0x0070, /**< GT1 off vlaue */
        SYSREG_ADDR_GT3_TARGET          = 0x002b, /**< GT3 target value */
        SYSREG_ADDR_GT3_ON              = 0x0073, /**< GT3 on value */
        SYSREG_ADDR_GT3_OFF             = 0x0074, /**< GT3 off vlaue */
        SYSREG_ADDR_GT4_TARGET          = 0x006d, /**< GT4 target value */
        SYSREG_ADDR_ADDHEAT_POWER       = 0x006c, /**< Additional heat power in percent */

        /* Device values */
        SYSREG_ADDR_P3                  = 0x01fd, /**< Ground loop pump P3 */
        SYSREG_ADDR_COMPRESSOR          = 0x01fe, /**< Compressor */
        SYSREG_ADDR_ADDHEAT_3KW         = 0x01ff, /**< Additional heat 3kW */
        SYSREG_ADDR_ADDHEAT_6KW         = 0x0200, /**< Additional heat 6kW */
        SYSREG_ADDR_P1                  = 0x0203, /**< Radiator pump P1 */
        SYSREG_ADDR_P2                  = 0x0204, /**< Heat carrier pump P2 */
        SYSREG_ADDR_VXV                 = 0x0205, /**< Tree-way valve VXV */
        SYSREG_ADDR_ALARM               = 0x0206, /**< Alarm */

        /* Sensor values */
        SYSREG_ADDR_GT1                 = 0x0209, /**< Radiator return GT1 */
        SYSREG_ADDR_GT2                 = 0x020a, /**< Outdoor GT2 */
        SYSREG_ADDR_GT3                 = 0x020b, /**< Hot water GT3 */
        SYSREG_ADDR_GT4                 = 0x020c, /**< Forward GT4 */
        SYSREG_ADDR_GT5                 = 0x020d, /**< Room GT5 */
        SYSREG_ADDR_GT6                 = 0x020e, /**< Compressor GT6 */
        SYSREG_ADDR_GT8                 = 0x020f, /**< Heat fluid out GT8 */
        SYSREG_ADDR_GT9                 = 0x0210, /**< Heat fluid in GT9 */
        SYSREG_ADDR_GT10                = 0x0211, /**< Cold fluid in GT10 */
        SYSREG_ADDR_GT11                = 0x0212, /**< Cold fulid out GT11 */
        SYSREG_ADDR_GT3X                = 0x0213  /**< External hot water GT3X */
    };

    /**
     * Front panel LED addresses of Rego600
     */
    enum FrontPanelAddr : uint16_t
    {
        FRONTPANEL_ADDR_POWER_BUTTON  = 0x0008, /**< Power button */
        FRONTPANEL_ADDR_LEFT_BUTTON   = 0x0009, /**< Left button */
        FRONTPANEL_ADDR_MIDDLE_BUTTON = 0x000A, /**< Middle button */
        FRONTPANEL_ADDR_RIGHT_BUTTON  = 0x000B, /**< Right button */
        FRONTPANEL_ADDR_POWER_LED     = 0x0012, /**< Power LED */
        FRONTPANEL_ADDR_PUMP_LED      = 0x0013, /**< Pump LED */
        FRONTPANEL_ADDR_HEATING_LED   = 0x0014, /**< Heating LED */
        FRONTPANEL_ADDR_BOILER_LED    = 0x0015, /**< Boiler LED */
        FRONTPANEL_ADDR_ALARM_LED     = 0x0016, /**< Alarm LED */
        FRONTPANEL_ADDR_WHEEL         = 0x0044  /**< Wheel */
    };

    /**
     * Display row identifiers of Rego600
     */
    enum Row : uint16_t
    {
        DISPLAY_ROW_1 = 0x00, /**< Row 1 */
        DISPLAY_ROW_2 = 0x01, /**< Row 2 */
        DISPLAY_ROW_3 = 0x02, /**< Row 3 */
        DISPLAY_ROW_4 = 0x03  /**< Row 4 */
    };

    /**
     * Constructs the Rego6xx controller.
     *
     * @param[in] stream    Input/Output stream connected to heatpump controller
     */
    Rego6xxCtrl(Stream& stream) :
        m_stream(stream),
        m_pendingRsp(nullptr),
        m_stdRsp(stream),
        m_confirmRsp(stream),
        m_errorRsp(stream),
        m_boolRsp(stream),
        m_displayRsp(stream)
    {
        m_stream.setTimeout(20U);
        clearRxBuffer();
    }

    /**
     * Destroys the Reg6xx controller.
     */
    ~Rego6xxCtrl()
    {
    }

    /**
     * Standard read from address
     *
     * @param[in] cmdId     Command id
     * @param[in] addr      Address
     *
     * @return Asynchronous response
     */
    const Rego6xxStdRsp* readStd(uint8_t cmdId, uint16_t addr);

    /**
     * Standard write value to addres.
     *
     * @param[in] cmdId Command id
     * @param[in] addr  Address
     * @param[in] value Value
     *
     * @return Asynchronous response
     */
    const Rego6xxConfirmRsp* writeStd(uint8_t cmdId, uint16_t addr, uint32_t value);

    /**
     * Convert 16 bit value to float.
     *
     * @param[in] value The 16-bit value, received from the heatpump controller.
     *
     * @return The float value, converted from the 16-bit value.
     */
    float toFloat(uint32_t value);

    /**
     * Convert 16 bit value to bool.
     *
     * @param[in] value The 16-bit value, received from the heatpump controller.
     *
     * @return The boolean value, converted from the 16-bit value.
     */
    bool toBool(uint32_t value);

    /**
     * Convert float to 16 bit value.
     *
     * @param[in] value The float value, which should be converted to a 16-bit value.
     *
     * @return The 16-bit value, converted from the float value.
     */
    uint32_t fromFloat(float value);

    /**
     * Read last error description.
     *
     * @return Asynchronous response
     */
    const Rego6xxErrorRsp* readLastError();

    /**
     * Read version of Rego6xxx controller.
     *
     * Note, the answer for a Rego600 controller is 0x0258.
     *
     * @return Asynchronous response
     */
    const Rego6xxStdRsp* readRegoVersion();

    /**
     * Read from display.
     *
     * @param[in] cmdId Command id
     * @param[in] addr  Address
     *
     * @return Asynchronous response
     */
    const Rego6xxDisplayRsp* readDisplay(uint8_t cmdId, uint16_t addr);

    /**
     * Write a value to the given address.
     * Only used for debugging purposes and reverse engineering of the Rego6xx controller
     * communication.
     *
     * @param[in] cmdId Command id
     * @param[in] addr  Address
     * @param[in] value Value
     *
     * @return Asynchronous response
     */
    String writeDbg(uint8_t cmdId, uint16_t addr, uint32_t value);

    /**
     * Process the controller, which is necessary to receive responses from
     * the heatpump.
     */
    void process();

    /**
     * Release response.
     */
    void release();

    /**
     * Any response pending?
     *
     * @return If waiting for a response, it will return true otherwise false.
     */
    bool isPending() const;

    /** Device address of heat pump controller */
    static const uint8_t DEV_ADDR_HEATPUMP = 0x81;

    /** Device address of the host */
    static const uint8_t DEV_ADDR_HOST     = 0x01;

    /** Command size in bytes */
    static const uint8_t CMD_SIZE          = 9;

private:

    Stream&           m_stream;     /**< Input/Output stream to heatpump controller. */
    Rego6xxRsp*       m_pendingRsp; /**< Current pending response */
    Rego6xxStdRsp     m_stdRsp;     /**< Standard response */
    Rego6xxConfirmRsp m_confirmRsp; /**< Confirmation response */
    Rego6xxErrorRsp   m_errorRsp;   /**< Error log response */
    Rego6xxBoolRsp    m_boolRsp;    /**< Boolean response */
    Rego6xxDisplayRsp m_displayRsp; /**< Display response */

    Rego6xxCtrl();

    /**
     * Clear receive buffer of the stream.
     */
    void clearRxBuffer()
    {
        while (0 < m_stream.available())
        {
            (void)m_stream.read();
        }
    }

    /**
     * Write command to heatpump controller.
     *
     * @param[in] devAddr   Device address (heatpump)
     * @param[in] cmdId     Command id
     * @param[in] data      Command data
     */
    void writeCmd(uint8_t devAddr, uint8_t cmdId, uint16_t regAddr, uint32_t data);
};

#endif /* __REGO6XX_CTRL_H__ */

/** @} */