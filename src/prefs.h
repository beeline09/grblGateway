#include <Arduino.h>
#include <Preferences.h>

String getWifiName();
String getWifiPassword();
String getWifiSoftApName();
String getBtName();

void saveWiFiName(String name);
void saveWiFiPassword(String password);
void saveWifiSoftApName(String name);
void saveBtName(String name);