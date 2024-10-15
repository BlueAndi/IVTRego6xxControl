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
 * @brief  System state: Init
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "InitState.h"

#include <Arduino.h>
#include <WiFi.h>
#include <Board.h>
#include <lwip/init.h>
#include <SettingsService.h>
#include <Logging.h>
#include <LittleFS.h>
#include <Wire.h>
#include <JsonFile.h>
#include <ESPmDNS.h>

#include "ReadyState.h"
#include "ErrorState.h"
#include "WiFiUtil.h"
#include "UpdateMgr.h"
#include "EthernetDrv.h"
#include "MyWebServer.h"
#include "WebConfig.h"
#include "Version.h"

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
 * The filename of the version information file.
 */
static const char VERSION_FILE_NAME[] = "/version.json";

/******************************************************************************
 * Public Methods
 *****************************************************************************/

void InitState::entry(StateMachine& sm)
{
    bool                isError  = false;
    ErrorState::ErrorId errorId  = ErrorState::ERROR_ID_UNKNOWN;
    SettingsService&    settings = SettingsService::getInstance();
    String              uniqueId;

    /* Initialize hardware */
    Board::init();

    /* Show as soon as possible the user on the serial console that the system is booting. */
    showStartupInfoOnSerial();

    /* To avoid name clashes, add a unqiue id to some of the default values. */
    getDeviceUniqueId(uniqueId);
    settings.getWifiApSSID().setUniqueId(uniqueId);
    settings.getHostname().setUniqueId(uniqueId);

    /* Set two-wire (I2C) pins, before calling begin(). */
    if (false == Wire.setPins(Board::Pin::i2cSdaPinNo, Board::Pin::i2cSclPinNo))
    {
        LOG_FATAL("Couldn't set two-wire pins.");
        errorId = ErrorState::ERROR_ID_TWO_WIRE_ERROR;
        isError = true;
    }
    /* Initialize two-wire (I2C) */
    else if (false == Wire.begin())
    {
        LOG_FATAL("Couldn't initialize two-wire.");
        errorId = ErrorState::ERROR_ID_TWO_WIRE_ERROR;
        isError = true;
    }
    /* Mounting the filesystem. */
    else if (false == mountFilesystem())
    {
        LOG_FATAL("Couldn't mount the filesystem.");
        errorId = ErrorState::ERROR_ID_BAD_FS;
        isError = true;
    }
    /* Check whether the filesystem is valid.
     * This is simply done by checking for a specific file in the root directory.
     */
    else if (false == LittleFS.exists(VERSION_FILE_NAME))
    {
        LOG_FATAL("Filesystem is invalid.");
        errorId = ErrorState::ERROR_ID_BAD_FS;
        isError = true;
    }
    /* Start settings service */
    else if (false == SettingsService::getInstance().start())
    {
        LOG_FATAL("Starting settings service failed.");
        errorId = ErrorState::ERROR_ID_SERVICE;
        isError = true;
    }
    else
    {
        /* Nothing to do. */
    }

    /* Continue only if there is no error yet. */
    if (true == isError)
    {
        /* Error detected. */
        ;
    }
    /* Initialize over-the-air update server */
    else if (false == UpdateMgr::getInstance().init())
    {
        LOG_FATAL("Failed to initialize Arduino OTA.");
        errorId = ErrorState::ERROR_ID_UPDATE_MGR;
        isError = true;
    }
    else
    {
        /* Clean up settings first! Important step after a firmware update to
         * keep the settings up-to-date and prevent the persistency will
         * silently growing up with unused stuff.
         */
        if (true == settings.open(false))
        {
            LOG_INFO("Clean up settings.");
            settings.cleanUp();
            settings.close();
        }

        /* Don't store the wifi configuration in the NVS.
         * This seems to cause a reset after a client connected to the access point.
         * https://github.com/espressif/arduino-esp32/issues/2025#issuecomment-503415364
         */
        WiFi.persistent(false);

        /* Show a warning in case the filesystem may not be compatible to the firmware version. */
        if (false == isFsCompatible())
        {
            const uint32_t DURATION_NON_SCROLLING = 4000U; /* ms */
            const uint32_t SCROLLING_REPEAT_NUM   = 1U;
            const uint32_t DURATION_PAUSE         = 500U; /* ms */
            const uint32_t SCROLLING_NO_REPEAT    = 0U;
            const char*    errMsg                 = "WARN: Filesystem may not be compatible.";

            LOG_WARNING(errMsg);
        }
    }

    /* Any error happened? */
    if (true == isError)
    {
        ErrorState::getInstance().setErrorId(errorId);
        sm.setState(ErrorState::getInstance());
    }
}

void InitState::process(StateMachine& sm)
{
    SettingsService::getInstance().process();

    sm.setState(ReadyState::getInstance());
}

void InitState::exit(StateMachine& sm)
{
    /* Continue initialization steps only, if there was no low level error before. */
    if (ErrorState::ERROR_ID_NO_ERROR == ErrorState::getInstance().getErrorId())
    {
        SettingsService& settings = SettingsService::getInstance();
        String           hostname;

        /* Get hostname. */
        if (false == settings.open(true))
        {
            LOG_WARNING("Use default hostname.");

            hostname = settings.getHostname().getDefault();
        }
        else
        {
            hostname = settings.getHostname().getValue();

            settings.close();
        }

        if (false == EthernetDrv::begin(hostname.c_str()))
        {
            /* Fatal error */
            LOG_FATAL("Failed to start ethernet driver.");

            sm.setState(ErrorState::getInstance());
        }
        /* Enable mDNS */
        else if (false == MDNS.begin(hostname.c_str()))
        {
            /* Fatal error */
            LOG_FATAL("Failed to setup mDNS.");

            sm.setState(ErrorState::getInstance());
        }
        else
        {
            /* Initialize webserver. The filesystem must be mounted before! */
            MyWebServer::init(false);
            MDNS.addService("http", "tcp", WebConfig::WEBSERVER_PORT);

            /* Start over-the-air update server. */
            UpdateMgr::getInstance().begin();
            MDNS.enableArduino(WebConfig::ARDUINO_OTA_PORT, true); /* This typically set by ArduinoOTA, but is disabled there. */

            /* Start webserver after the wifi access point is running.
             * If its done earlier, it will cause an exception because the LwIP stack
             * is not initialized.
             * The LwIP stack is initialized with wifiLowLevelInit()!
             */
            MyWebServer::begin();
        }
    }
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

void InitState::showStartupInfoOnSerial()
{
    LOG_INFO("IVTRego6xxControl starts up ...");
    LOG_INFO("Target: %s", Version::TARGET);
    LOG_INFO("SW version: %s", Version::SOFTWARE_VER);
    delay(20U); /* To avoid missing log messages on the console */
    LOG_INFO("SW revision: %s", Version::SOFTWARE_REV);
    LOG_INFO("ESP32 chip rev.: %u", ESP.getChipRevision());
    LOG_INFO("ESP32 SDK version: %s", ESP.getSdkVersion());
    delay(20U); /* To avoid missing log messages on the console */
    LOG_INFO("Wifi MAC: %s", WiFi.macAddress().c_str());
    LOG_INFO("LwIP version: %s", LWIP_VERSION_STRING);
    delay(20U); /* To avoid missing log messages on the console */
}

bool InitState::isFsCompatible()
{
    bool         isCompatible = false;
    JsonFile     jsonFile(LittleFS);
    JsonDocument jsonDoc;

    if (true == jsonFile.load(VERSION_FILE_NAME, jsonDoc))
    {
        JsonVariantConst jsonVersion = jsonDoc["version"];

        if (false == jsonVersion.isNull())
        {
            String fileSystemVersion = jsonVersion.as<String>();
            String firmwareVersion   = Version::SOFTWARE_VER;

            /* Note that the firmware version may have a additional postfix.
             * Example: v4.1.2:b or v4.1.2:b:lc
             * See ./scripts/get_get_rev.py for the different postfixes.
             */
            if (true == firmwareVersion.startsWith(fileSystemVersion))
            {
                isCompatible = true;
            }
        }
    }

    return isCompatible;
}

bool InitState::mountFilesystem()
{
    bool        isSuccessful                = false;
    bool        formatOnFail                = false;
    const char* BASE_PATH                   = "/littlefs";
    uint8_t     maxOpenFiles                = 10U;
    const char* PARTITION_LABEL_DEFAULT     = "spiffs"; /* Default for most of the partitions, defined by Platformio. */
    const char* PARTITION_LABEL_ALTERNATIVE = "ffat";   /* Sometimes its different, than the default in Platformio. */

    /* Mount filesytem with default partition label. If it fails, use alternative. */
    if (false == LittleFS.begin(formatOnFail, BASE_PATH, maxOpenFiles, PARTITION_LABEL_DEFAULT))
    {
        /* Try to mount with alternative partition label. */
        if (true == LittleFS.begin(formatOnFail, BASE_PATH, maxOpenFiles, PARTITION_LABEL_ALTERNATIVE))
        {
            /* Successful mounted with alternative partition label. */
            isSuccessful = true;
        }
    }
    /* Successful mounted with default partition label. */
    else
    {
        isSuccessful = true;
    }

    return isSuccessful;
}

void InitState::getDeviceUniqueId(String& deviceUniqueId)
{
    /* Use the last 4 bytes of the factory programmed wifi MAC address to generate a unique id. */
    String chipId;

    WiFiUtil::getChipId(chipId);

    deviceUniqueId += "-";
    deviceUniqueId += chipId.substring(4U);
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/
