#include <Arduino.h>
#include <Preferences.h>

String getWifiName();
String getWifiPassword();
void saveWiFiName(String name);
void saveWiFiPassword(String password);

String getWifiSoftApName();
void saveWifiSoftApName(String name);

String getBtName();
void saveBtName(String name);

uint16_t getSpindelMinRpm();
uint16_t getSpindelMaxRpm();
uint16_t getSpindelCurrentRpm();
void saveSpindleMinRpm(uint16_t rpm);
void saveSpindleMaxRpm(uint16_t rpm);
void saveSpindelCurrentRpm(uint16_t rpm);

float getStepSizeXY();
float getStepSizeZ();
uint16_t getAccelerationXY();
uint16_t getAccelerationZ();
void saveStepSizeXY(float size);
void saveStepSizeZ(float size);
void saveAccelerationXY(uint16_t acc);
void saveAccelerationZ(uint16_t acc);