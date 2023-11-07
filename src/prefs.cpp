#include "prefs.h"

#define KEY_WIFI_NAME "wifiName"
#define KEY_WIFI_SOFT_AP_NAME "wifiSoftName"
#define KEY_WIFI_PASSWORD "wifiPassword"
#define KEY_BT_NAME "btName"
#define KEY_BT_PIN "btPin"

Preferences preferences;

String getWifiName()
{
    preferences.begin("bt-gateway", false);
    String name = preferences.getString(KEY_WIFI_NAME, "");
    preferences.end();
    return name;
}

String getWifiPassword()
{
    preferences.begin("bt-gateway", false);
    String pass = preferences.getString(KEY_WIFI_PASSWORD, "");
    preferences.end();
    return pass;
}

String getWifiSoftApName()
{
    preferences.begin("bt-gateway", false);
    String name = preferences.getString(KEY_WIFI_SOFT_AP_NAME, "GrblGatewaySoftAP");
    preferences.end();
    return name;
}
String getBtName()
{
    preferences.begin("bt-gateway", false);
    String name = preferences.getString(KEY_BT_NAME, "CNC3018 Pro");
    preferences.end();
    return name;
}

void saveWiFiName(String name)
{
    preferences.begin("bt-gateway", false);
    preferences.putString(KEY_WIFI_NAME, name);
    preferences.end();
}

void saveWiFiPassword(String password)
{
    preferences.begin("bt-gateway", false);
    preferences.putString(KEY_WIFI_PASSWORD, password);
    preferences.end();
}

void saveWifiSoftApName(String name)
{
    preferences.begin("bt-gateway", false);
    preferences.putString(KEY_WIFI_SOFT_AP_NAME, name);
    preferences.end();
}

void saveBtName(String name)
{
    preferences.begin("bt-gateway", false);
    preferences.putString(KEY_BT_NAME, name);
    preferences.end();
}