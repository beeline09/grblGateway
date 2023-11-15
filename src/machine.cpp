#include "machine.h"
#include "Globals.h"

String WposX;  // last known X pos on workpiece, space for 9 characters ( -999.999\0 )
String WposY;  // last known Y pos on workpiece
String WposZ;  // last known Z heighton workpiece, space for 8 characters is enough( -99.999\0 )
String WposXR; // last known X pos on workpiece, space for 9 characters ( -999.999\0 )
String WposYR; // last known Y pos on workpiece
String WposZR; // last known Z heighton workpiece, space for 8 characters is enough( -99.999\0 )
float spindelValue;
uint16_t spindleMinRpm;
uint16_t spindleMaxRpm;
uint16_t spindleCurrentRpm;
String wMessage;
String wAlarm;
String wError;
String machineStatus;    // last know state (Idle, Run, Hold, Door, Home, Alarm, Check)
bool awaitingOK = false; // this is set true when we are waiting for the ok signal from the grbl board (see the sendCodeLine() void)
bool homing = false;
bool modWhileRun = false;

unsigned long runningTime, lastUpdateMenu, timeWithOutPress, timeExit = 5000, lastButtonCheck;

unsigned int numLineTotal = 0, numLineAct = 0;

int16_t oldPosition = 0;
int16_t position = 0;

byte timeDelay = 150, optionSelectLast;

void machineTick()
{
  static unsigned long lastSendStatus = 0;
  if (millis() - lastSendStatus >= 1000 || lastSendStatus == 0 || millis() - lastSendStatus < 0)
  {
    lastSendStatus = millis();
    if (!isBtConnected)
    {
      sendCommand("?");
    }
  }
}

int sendCommand(char *command)
{
  if (!isBtConnected)
  {
    Serial.println(command);
  }
  else
  {
    return 5;
  }
  return 0;
}