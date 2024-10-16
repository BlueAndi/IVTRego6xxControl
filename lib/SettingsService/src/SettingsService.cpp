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
 * @brief  Settings service
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "SettingsService.h"
#include "nvs.h"

#include <Logging.h>
#include <algorithm>

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

/** SettingsService namespace used for preferences */
static const char*  PREF_NAMESPACE                  = "settings";

/* ---------- Keys ---------- */

/* Note:
 * Zero-terminated ASCII string containing a key name.
 * Maximum string length is 15 bytes, excluding a zero terminator.
 * https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/storage/nvs_flash.html
 */

/** Settings version key */
static const char*  KEY_VERSION                     = "version";

/** Wifi network key */
static const char*  KEY_WIFI_SSID                   = "sta_ssid";

/** Wifi network passphrase key */
static const char*  KEY_WIFI_PASSPHRASE             = "sta_passphrase";

/** Wifi access point network key */
static const char*  KEY_WIFI_AP_SSID                = "ap_ssid";

/** Wifi access point network passphrase key */
static const char*  KEY_WIFI_AP_PASSPHRASE          = "ap_passphrase";

/** Website login user account key */
static const char*  KEY_WEB_LOGIN_USER              = "web_login_user";

/** Website login user password key */
static const char*  KEY_WEB_LOGIN_PASSWORD          = "web_login_pass";

/** Hostname key */
static const char*  KEY_HOSTNAME                    = "hostname";

/* ---------- Key value pair names ---------- */

/** SettingsService version name */
static const char*  NAME_VERSION                    = "SettingsService version";

/** Wifi network name of key value pair */
static const char*  NAME_WIFI_SSID                  = "Wifi SSID";

/** Wifi network passphrase name of key value pair */
static const char*  NAME_WIFI_PASSPHRASE            = "Wifi passphrase";

/** Wifi access point network name of key value pair */
static const char*  NAME_WIFI_AP_SSID               = "Wifi AP SSID";

/** Wifi access point network passphrase name of key value pair */
static const char*  NAME_WIFI_AP_PASSPHRASE         = "Wifi AP passphrase";

/** Website login user account name of key value pair */
static const char*  NAME_WEB_LOGIN_USER             = "Website login user";

/** Website login user password name of key value pair */
static const char*  NAME_WEB_LOGIN_PASSWORD         = "Website login password";

/** Hostname name of key value pair */
static const char*  NAME_HOSTNAME                   = "Hostname";

/* ---------- Default values ---------- */

/** SettingsService version default value */
static const uint32_t   DEFAULT_VERSION                 = 0U; /* 0 is important to detect whether the version is not stored yet. */

/** Wifi network default value */
static const char*      DEFAULT_WIFI_SSID               = "";

/** Wifi network passphrase default value */
static const char*      DEFAULT_WIFI_PASSPHRASE         = "";

/** Wifi access point network default value */
static const char*      DEFAULT_WIFI_AP_SSID            = "IVTReg6xxControl";

/** Wifi access point network passphrase default value */
static const char*      DEFAULT_WIFI_AP_PASSPHRASE      = "Luke, I am your father.";

/** Website login user account default value */
static const char*      DEFAULT_WEB_LOGIN_USER          = "luke";

/** Website login user password default value */
static const char*      DEFAULT_WEB_LOGIN_PASSWORD      = "skywalker";

/** Hostname default value */
static const char*      DEFAULT_HOSTNAME                = "IVTReg6xxControl";

/* ---------- Minimum values ---------- */

/** SettingsService version min. value */
static const uint32_t   MIN_VALUE_VERSION               = 0;

/** Wifi network SSID min. length. Section 7.3.2.1 of the 802.11-2007 specification. */
static const size_t     MIN_VALUE_WIFI_SSID             = 0;

/** Wifi network passphrase min. length */
static const size_t     MIN_VALUE_WIFI_PASSPHRASE       = 8U;

/** Wifi access point network SSID min. length. Section 7.3.2.1 of the 802.11-2007 specification. */
static const size_t     MIN_VALUE_WIFI_AP_SSID          = 0;

/** Wifi access point network passphrase min. length */
static const size_t     MIN_VALUE_WIFI_AP_PASSPHRASE    = 8U;

/** Website login user account min. length */
static const size_t     MIN_VALUE_WEB_LOGIN_USER        = 4U;

/** Website login user password min. length */
static const size_t     MIN_VALUE_WEB_LOGIN_PASSWORD    = 4U;

/** Hostname min. length */
static const size_t     MIN_VALUE_HOSTNAME              = 1U;

/* ---------- Maximum values ---------- */

/** SettingsService version max. value */
static const uint32_t   MAX_VALUE_VERSION               = UINT32_MAX;

/** Wifi network SSID max. length. Section 7.3.2.1 of the 802.11-2007 specification. */
static const size_t     MAX_VALUE_WIFI_SSID             = 32U;

/** Wifi network passphrase max. length */
static const size_t     MAX_VALUE_WIFI_PASSPHRASE       = 64U;

/** Wifi access point network SSID max. length. Section 7.3.2.1 of the 802.11-2007 specification. */
static const size_t     MAX_VALUE_WIFI_AP_SSID          = 32U;

/** Wifi access point network passphrase max. length */
static const size_t     MAX_VALUE_WIFI_AP_PASSPHRASE    = 64U;

/** Website login user account max. length */
static const size_t     MAX_VALUE_WEB_LOGIN_USER        = 16U;

/** Website login user password max. length */
static const size_t     MAX_VALUE_WEB_LOGIN_PASSWORD    = 32U;

/** Hostname max. length */
static const size_t     MAX_VALUE_HOSTNAME              = 63U;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

bool SettingsService::start()
{
    LOG_INFO("Settings service started.");

    return true;
}

void SettingsService::stop()
{
    LOG_INFO("Settings service stopped.");
}

void SettingsService::process()
{
    /* Nothing to do. */
}

bool SettingsService::open(bool readOnly)
{
    /* Open Preferences with namespace. Each application module, library, etc
     * has to use a namespace name to prevent key name collisions. We will open storage in
     * RW-mode (second parameter has to be false).
     * Note: Namespace name is limited to 15 chars.
     */
    bool status = m_preferences.begin(PREF_NAMESPACE, readOnly);

    /* If settings storage doesn't exist, it will be created. */
    if ((false == status) &&
        (true == readOnly))
    {
        status = m_preferences.begin(PREF_NAMESPACE, false);

        if (true == status)
        {
            m_preferences.end();
            status = m_preferences.begin(PREF_NAMESPACE, readOnly);
        }
    }

    return status;
}

void SettingsService::close()
{
    m_preferences.end();
}

void SettingsService::cleanUp()
{
    uint32_t storedVersion = m_version.getValue();

    /* Clean up is only necessary, if settings version is different. */
    if (VERSION != storedVersion)
    {
        nvs_iterator_t  it;
        esp_err_t       err = nvs_entry_find(NVS_DEFAULT_PART_NAME, PREF_NAMESPACE, NVS_TYPE_ANY, &it);
        
        if (ESP_ERR_NVS_NOT_FOUND == err)
        {
            /* It seems to be the initial setup and there are no settings yet. */
        }
        else if (ESP_OK != err)
        {
            LOG_WARNING("Settings cleanup failed: %d", err);
        }
        else
        {
            while (ESP_OK == err)
            {
                nvs_entry_info_t info;

                err = nvs_entry_info(it, &info);

                if (ESP_OK == err)
                {
                    /* Obsolete setting?
                     * m_version key must be handled separate, because its not part of the settings list.
                     */
                    if ((0 != strcmp(m_version.getKey(), info.key)) &&
                        (nullptr == getSettingByKey(info.key)))
                    {
                        LOG_WARNING("Obsolete key %s removed from settings.", info.key);

                        if (false == m_preferences.remove(info.key))
                        {
                            LOG_ERROR("Failed to remove key %s removed from settings.", info.key);
                        }
                    }
                    else
                    {
                        LOG_INFO("Settings key %s is valid.", info.key);
                    }

                    err = nvs_entry_next(&it);
                }
            };
        }

        /* Update version */
        m_version.setValue(VERSION);
    }
}

KeyValue* SettingsService::getSettingByKey(const char* key)
{
    std::vector<KeyValue*>::const_iterator  it;
    KeyValue*                               keyValuePair    = nullptr;

    for(it = m_keyValueList.begin(); it != m_keyValueList.end(); ++it)
    {
        if (nullptr != *it)
        {
            if (0 == strcmp((*it)->getKey(), key))
            {
                keyValuePair = *it;
                break;
            }
        }
    }

    return keyValuePair;
}

bool SettingsService::registerSetting(KeyValue* setting)
{
    bool isSuccessful = false;

    if (nullptr != setting)
    {
        /* Register setting only once! */
        if (std::find(m_keyValueList.begin(), m_keyValueList.end(), setting) == m_keyValueList.end())
        {
            setting->setPersistentStorage(m_preferences);
            m_keyValueList.push_back(setting);

            isSuccessful = true;
        }
    }

    return isSuccessful;
}

void SettingsService::unregisterSetting(KeyValue* setting)
{
    std::vector<KeyValue*>::iterator it = m_keyValueList.begin();

    while(m_keyValueList.end() != it)
    {
        if (setting == *it)
        {
            it = m_keyValueList.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

SettingsService::SettingsService() :
    m_preferences(),
    m_keyValueList(),
    m_version               (m_preferences, KEY_VERSION,                NAME_VERSION,               DEFAULT_VERSION,                MIN_VALUE_VERSION,              MAX_VALUE_VERSION),
    m_wifiSSID              (m_preferences, KEY_WIFI_SSID,              NAME_WIFI_SSID,             DEFAULT_WIFI_SSID,              MIN_VALUE_WIFI_SSID,            MAX_VALUE_WIFI_SSID),
    m_wifiPassphrase        (m_preferences, KEY_WIFI_PASSPHRASE,        NAME_WIFI_PASSPHRASE,       DEFAULT_WIFI_PASSPHRASE,        MIN_VALUE_WIFI_PASSPHRASE,      MAX_VALUE_WIFI_PASSPHRASE,      true),
    m_apSSID                (m_preferences, KEY_WIFI_AP_SSID,           NAME_WIFI_AP_SSID,          DEFAULT_WIFI_AP_SSID,           MIN_VALUE_WIFI_AP_SSID,         MAX_VALUE_WIFI_AP_SSID),
    m_apPassphrase          (m_preferences, KEY_WIFI_AP_PASSPHRASE,     NAME_WIFI_AP_PASSPHRASE,    DEFAULT_WIFI_AP_PASSPHRASE,     MIN_VALUE_WIFI_AP_PASSPHRASE,   MAX_VALUE_WIFI_AP_PASSPHRASE,   true),
    m_webLoginUser          (m_preferences, KEY_WEB_LOGIN_USER,         NAME_WEB_LOGIN_USER,        DEFAULT_WEB_LOGIN_USER,         MIN_VALUE_WEB_LOGIN_USER,       MAX_VALUE_WEB_LOGIN_USER),
    m_webLoginPassword      (m_preferences, KEY_WEB_LOGIN_PASSWORD,     NAME_WEB_LOGIN_PASSWORD,    DEFAULT_WEB_LOGIN_PASSWORD,     MIN_VALUE_WEB_LOGIN_PASSWORD,   MAX_VALUE_WEB_LOGIN_PASSWORD,   true),
    m_hostname              (m_preferences, KEY_HOSTNAME,               NAME_HOSTNAME,              DEFAULT_HOSTNAME,               MIN_VALUE_HOSTNAME,             MAX_VALUE_HOSTNAME)
{

    /* Skip m_version, because it shall not be modified by the user. */
    m_keyValueList.push_back(&m_wifiSSID);
    m_keyValueList.push_back(&m_wifiPassphrase);
    m_keyValueList.push_back(&m_apSSID);
    m_keyValueList.push_back(&m_apPassphrase);
    m_keyValueList.push_back(&m_webLoginUser);
    m_keyValueList.push_back(&m_webLoginPassword);
    m_keyValueList.push_back(&m_hostname);
}

SettingsService::~SettingsService()
{
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/
