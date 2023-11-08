#include <Arduino.h>
#include "BluetoothSerial.h"
// #include "grbl/c_src/grbl_chat_c.h"

extern BluetoothSerial SerialBT;
// extern grbl_data_t grbl_data;
extern String ipAddress;

extern String WposX; // last known X pos on workpiece, space for 9 characters ( -999.999\0 )
extern String WposY; // last known Y pos on workpiece
extern String WposZ; // last known Z heighton workpiece, space for 8 characters is enough( -99.999\0 )
extern String WposXR; // last known X pos on workpiece, space for 9 characters ( -999.999\0 )
extern String WposYR; // last known Y pos on workpiece
extern String WposZR; // last known Z heighton workpiece, space for 8 characters is enough( -99.999\0 )
extern String spindelValue;
extern String wMessage;
extern String wAlarm;
extern String wError;
extern void clearMessages();

extern String machineStatus; // last know state (Idle, Run, Hold, Door, Home, Alarm, Check)

extern bool awaitingOK; // this is set true when we are waiting for the ok signal from the grbl board (see the sendCodeLine() void)
extern bool homing;
extern bool modWhileRun;