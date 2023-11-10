#include "prefs.h"

#define KEY_WIFI_NAME "wifiName"
#define KEY_WIFI_SOFT_AP_NAME "wifiSoftName"
#define KEY_WIFI_PASSWORD "wifiPassword"
#define KEY_BT_NAME "btName"
#define KEY_BT_PIN "btPin"
#define KEY_SPINDEL_MIN_RPM "spindelMinRpm"
#define KEY_SPINDEL_MAX_RPM "spindelMaxRpm"
#define KEY_SPINDEL_CURRENT_RPM "spindelCurRpm"
#define KEY_STEP_SIZE_XY "stepSizeXY"
#define KEY_STEP_SIZE_Z "stepSizeZ"
#define KEY_ACCELERATION_XY "accXY"
#define KEY_ACCELERATION_Z "accZ"

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

uint16_t getSpindelMinRpm()
{
    preferences.begin("bt-gateway", false);
    uint16_t rpm = preferences.getInt(KEY_SPINDEL_MIN_RPM, 100);
    preferences.end();
    return rpm;
}
uint16_t getSpindelMaxRpm()
{
    preferences.begin("bt-gateway", false);
    uint16_t rpm = preferences.getInt(KEY_SPINDEL_MAX_RPM, 10000);
    preferences.end();
    return rpm;
}
uint16_t getSpindelCurrentRpm()
{
    preferences.begin("bt-gateway", false);
    uint16_t rpm = preferences.getInt(KEY_SPINDEL_CURRENT_RPM, 990);
    preferences.end();
    return rpm;
}
void saveSpindleMinRpm(uint16_t rpm)
{
    preferences.begin("bt-gateway", false);
    preferences.putInt(KEY_SPINDEL_MIN_RPM, rpm);
    preferences.end();
}
void saveSpindleMaxRpm(uint16_t rpm)
{
    preferences.begin("bt-gateway", false);
    preferences.putInt(KEY_SPINDEL_MAX_RPM, rpm);
    preferences.end();
}

void saveSpindelCurrentRpm(uint16_t rpm)
{
    preferences.begin("bt-gateway", false);
    preferences.putInt(KEY_SPINDEL_CURRENT_RPM, rpm);
    preferences.end();
}

float getStepSizeXY()
{
    preferences.begin("bt-gateway", false);
    float sz = preferences.getFloat(KEY_STEP_SIZE_XY, 10);
    preferences.end();
    return sz;
}
float getStepSizeZ()
{
    preferences.begin("bt-gateway", false);
    float sz = preferences.getFloat(KEY_STEP_SIZE_Z, 1);
    preferences.end();
    return sz;
}
uint16_t getAccelerationXY()
{
    preferences.begin("bt-gateway", false);
    uint16_t acc = preferences.getInt(KEY_ACCELERATION_XY, 100);
    preferences.end();
    return acc;
}
uint16_t getAccelerationZ()
{
    preferences.begin("bt-gateway", false);
    uint16_t acc = preferences.getInt(KEY_ACCELERATION_Z, 50);
    preferences.end();
    return acc;
}
void saveStepSizeXY(float size)
{
    preferences.begin("bt-gateway", false);
    preferences.putFloat(KEY_STEP_SIZE_XY, size);
    preferences.end();
}
void saveStepSizeZ(float size)
{
    preferences.begin("bt-gateway", false);
    preferences.putFloat(KEY_STEP_SIZE_Z, size);
    preferences.end();
}
void saveAccelerationXY(uint16_t acc)
{
    preferences.begin("bt-gateway", false);
    preferences.putInt(KEY_ACCELERATION_XY, acc);
    preferences.end();
}
void saveAccelerationZ(uint16_t acc)
{
    preferences.begin("bt-gateway", false);
    preferences.putInt(KEY_ACCELERATION_Z, acc);
    preferences.end();
}