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
 * @brief Ethernet driver
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "EthernetDrv.h"
#include <WiFi.h>
#include <ETH.h>
#include <Logging.h>

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

static void event(WiFiEvent_t event);

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/** Ethernet interface hostname. */
static String   gHostname;

/** Ethernet connection status. */
static bool     gIsConnected    = false;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

bool EthernetDrv::begin(const char* hostname)
{
    gHostname = hostname;

    /* Add a handler for network events. This is missnamed "WiFi" because
     * the ESP32 is historically WiFi only, but in our case, this will
     * react to Ethernet events.
     */
    WiFi.onEvent(event);

    /* Start Ethernet (this does NOT start WiFi at the same time) */
    return ETH.begin();
}


void EthernetDrv::end()
{
    ETH.end();
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

/**
 * Handle ethernet events.
 * 
 * @param[in] event Ethernet event
 */
static void event(WiFiEvent_t event)
{
    switch (event)
    {
    /* Ethernet service starts. */
    case ARDUINO_EVENT_ETH_START:
        LOG_INFO("ETH service starts.");
        ETH.setHostname(gHostname.c_str());
        break;

    /* Ethernet cable is plugged in. */
    case ARDUINO_EVENT_ETH_CONNECTED:
        LOG_INFO("ETH connected.");
        break;

    /* Obtain IP address through DHCP. */
    case ARDUINO_EVENT_ETH_GOT_IP:
        LOG_INFO("Got the IP address %s for ETH MAC %s.", ETH.localIP().toString().c_str(), ETH.macAddress().c_str());

        if (true == ETH.fullDuplex())
        {
            LOG_INFO("ETH full duplex.");
        }

        LOG_INFO("ETH link speed %u Mbps.", ETH.linkSpeed());

        gIsConnected = true;
        break;

    /* Ethernet cable is unplugged. */
    case ARDUINO_EVENT_ETH_DISCONNECTED:
        LOG_INFO("ETH disconnected.");
        gIsConnected = false;
        break;

    /* Ethernet interface stopped. */
    case ARDUINO_EVENT_ETH_STOP:
        LOG_INFO("ETH stopped.");
        gIsConnected = false;
        break;

    default:
        break;
    }
}
