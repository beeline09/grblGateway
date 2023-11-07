#include "machine.h"
#include "Globals.h"


String WposX; // last known X pos on workpiece, space for 9 characters ( -999.999\0 )
String WposY; // last known Y pos on workpiece
String WposZ; // last known Z heighton workpiece, space for 8 characters is enough( -99.999\0 )
String WposXR; // last known X pos on workpiece, space for 9 characters ( -999.999\0 )
String WposYR; // last known Y pos on workpiece
String WposZR; // last known Z heighton workpiece, space for 8 characters is enough( -99.999\0 )
String spindelValue;
String machineStatus; // last know state (Idle, Run, Hold, Door, Home, Alarm, Check)
bool awaitingOK = false; // this is set true when we are waiting for the ok signal from the grbl board (see the sendCodeLine() void)
bool homing = false;
bool modWhileRun = false;

unsigned long runningTime, lastUpdateMenu, timeWithOutPress, timeExit = 5000, lastButtonCheck;

unsigned int numLineTotal = 0, numLineAct = 0;

int16_t oldPosition = 0;
int16_t position = 0;

byte timeDelay = 150, optionSelectLast;
void getStatus(){}
/*
void getStatus()
{
  /*
    This gets the status of the machine
    The status message of the machine might look something like this (this is a worst scenario message)
    The max length of the message is 72 characters long (including carriage return).
    <Idle|WPos:0.000,0.000,0.000|FS:0,0>
  */
/*
  char content[80];
  char character;
  byte index = 0;
  bool completeMessage = false;
  int i = 0;
  int c = 0;
  //  SerialGRBL.println(F("GetStatus calls for CheckForOk"));
  checkForOk();

  SerialGRBL.print(F("?")); // Ask the machine status
  unsigned long times = millis();
  while (SerialGRBL.available() == 0)
  {
    if (homing)
      setTextDisplay(F("       Homing"), F("       Cycle"), F(" "), F("   Please Wait..."));
    else if (millis() - times >= 10000)
      controlMenu();
  } // Wait for response
  while (SerialGRBL.available())
  {
    character = SerialGRBL.read();
    content[index] = character;
    if (content[index] == '>')
      completeMessage = true; // a simple check to see if the message is complete
    if (index > 0)
    {
      if (content[index] == 'k' && content[index - 1] == 'o')
      {
        awaitingOK = false;
        //      SerialGRBL.println(F("< OK from status"));
      }
    }
    index++;
    delay(1);
  }

  if (!completeMessage)
  {
    return;
  }
  //  SerialGRBL.println(content);
  i++;
  while (c < 9 && content[i] != '|')
  {
    machineStatus[c++] = content[i++]; // get the machine status
    machineStatus[c] = 0;
  }
  while (content[i++] != ':')
    ; // skip until the first ':'
  c = 0;
  while (c < 8 && content[i] != ',')
  {
    WposX[c++] = content[i++]; // get WposX
    WposX[c] = 0;
  }
  c = 0;
  i++;
  while (c < 8 && content[i] != ',')
  {
    WposY[c++] = content[i++]; // get WposY
    WposY[c] = 0;
  }
  c = 0;
  i++;
  while (c < 8 && content[i] != '|')
  {
    WposZ[c++] = content[i++]; // get WposZ
    WposZ[c] = 0;
  }
  if (WposZ[0] == '-')
  {
    WposZ[5] = '0';
    WposZ[6] = 0;
  }
  else
  {
    WposZ[4] = '0';
    WposZ[5] = 0;
  }
}
*/
void checkForOk()
{
  // read the receive buffer (if anything to read)
  char c, lastc;
  bool error5 = false;
  c = 64;
  lastc = 64;
  while (Serial.available())
  {
    c = Serial.read();
    if (lastc == ':' && c == '5')
      error5 = true;
    else if (lastc == 'o' && c == 'k')
    {
      awaitingOK = false;
      // SerialGRBL.println(F("< OK"));
    }
    lastc = c;
    delay(1);
  }
  if (error5)
  {
    //  SerialGRBL.println(F("(error:5) Homing cycle failure. Homing is not enabled via settings."));
    //  setTextDisplay(F("error:5"), F("Homing cycle failure"), F("It's not enabled"), F("via settings ($22)"));
    //  delay(5000);
  }
}