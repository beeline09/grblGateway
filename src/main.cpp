#include <Arduino.h>
#include "Globals.h"
#include "espWiFi.h"
#include "hub.h"
#include "bt.h"
#include "grbl/grbl_chat.h"
#include "machine.h"

void setup()
{
  Serial.begin(115200);
  initBt();
  initWiFi();
  Serial.write(0x18);
  delay(100);
  Serial.println("$$");
}

void processMachineError(int errNum)
{
  switch (errNum)
  {
  case 1:
    wError = "Command letter was not found.";
    break;
  case 2:
    wError = "GCode Command value invalid or missing.";
    break;
  case 3:
    wError = "Grbl '$' not recognized or supported.";
    break;
  case 4:
    wError = "Negative value for an expected positive value.";
    break;
  case 5:
    wError = "Homing fail. Homing not enabled in settings.";
    break;
  case 6:
    wError = "Min step pulse must be greater than 3usec.";
    break;
  case 7:
    wError = "EEPROM read failed. Default values used.";
    break;
  case 8:
    wError = "Grbl '$' command Only valid when Idle.";
    break;
  case 9:
    wError = "GCode commands invalid in alarm or jog state.";
    break;
  case 10:
    wError = "Soft limits require homing to be enabled.";
    break;
  case 11:
    wError = "Max characters per line exceeded. Ignored.";
    break;
  case 12:
    wError = "Grbl '$' setting exceeds the maximum step rate.";
    break;
  case 13:
    wError = "Safety door opened and door state initiated.";
    break;
  case 14:
    wError = "Build info or start-up line > EEPROM line length";
    break;
  case 15:
    wError = "Jog target exceeds machine travel, ignored.";
    break;
  case 16:
    wError = "Jog Cmd missing '=' or has prohibited GCode.";
    break;
  case 17:
    wError = "Laser mode requires PWM output.";
    break;
  case 20:
    wError = "Unsupported or invalid GCode command.";
    break;
  case 21:
    wError = "> 1 GCode command in a modal group in block.";
    break;
  case 22:
    wError = "Feed rate has not yet been set or is undefined.";
    break;
  case 23:
    wError = "GCode command requires an integer value.";
    break;
  case 24:
    wError = "> 1 GCode command using axis words found.";
    break;
  case 25:
    wError = "Repeated GCode word found in block.";
    break;
  case 26:
    wError = "No axis words found in command block.";
    break;
  case 27:
    wError = "Line number value is invalid.";
    break;
  case 28:
    wError = "GCode Cmd missing a required value word.";
    break;
  case 29:
    wError = "G59.x WCS are not supported.";
    break;
  case 30:
    wError = "G53 only valid with G0 and G1 motion modes.";
    break;
  case 31:
    wError = "Unneeded Axis words found in block.";
    break;
  case 32:
    wError = "G2/G3 arcs need >= 1 in-plane axis word.";
    break;
  case 33:
    wError = "Motion command target is invalid.";
    break;
  case 34:
    wError = "Arc radius value is invalid.";
    break;
  case 35:
    wError = "G2/G3 arcs need >= 1 in-plane offset word.";
    break;
  case 36:
    wError = "Unused value words found in block.";
    break;
  case 37:
    wError = "G43.1 offset not assigned to tool length axis.";
    break;
  case 38:
    wError = "Tool number greater than max value.";
    break;
  default:
    wError = "Unknown error " + errNum;
    break;
  }
}

void processAlarm(int alarmNumber)
{
  switch (alarmNumber)
  {
  case 1:
    wAlarm = "Hard limit triggered. Position Lost.";
    break;
  case 2:
    wAlarm = "Soft limit alarm, position kept. Unlock is Safe.";
    break;
  case 3:
    wAlarm = "Reset while in motion. Position lost.";
    break;
  case 4:
    wAlarm = "Probe fail. Probe not in expected initial state.";
    break;
  case 5:
    wAlarm = "Probe fail. Probe did not contact the work.";
    break;
  case 6:
    wAlarm = "Homing fail. The active homing cycle was reset.";
    break;
  case 7:
    wAlarm = "Homing fail. Door opened during homing cycle.";
    break;
  case 8:
    wAlarm = "Homing fail. Pull off failed to clear limit switch.";
    break;
  case 9:
    wAlarm = "Homing fail. Could not find limit switch.";
    break;

  default:
    wAlarm = "";
    break;
  }
}

grbl_data_t ddata;
void loop()
{
  static int_fast16_t c;
  static uint_fast16_t char_counter = 0;
  while (SerialBT.available())
  {
    Serial.write(SerialBT.read());
  }
  while (Serial.available())
  {
    c = Serial.read();
    SerialBT.write(c);
    if (((c == '\n') || (c == '\r')))
    { // End of block reached

      if (char_counter > 0)
      {
        ddata.block[ddata.block[0] == '<' || ddata.block[0] == '[' ? char_counter - 1 : char_counter] = '\0'; // strip last character from long messages
        parseData(ddata.block);
        char_counter = 0;
      }
    }
    else if (char_counter < sizeof(ddata.block))
      ddata.block[char_counter++] = (char)c; // Upcase lowercase??
  }
  grbl_data_t *data = getData();
  WposX = data->position[0];
  WposY = data->position[1];
  WposZ = data->position[2];
  WposXR = data->offset[0];
  WposYR = data->offset[1];
  WposZR = data->offset[2];
  spindelValue = data->spindle.rpm_programmed;

  if (!strncmp(data->error, "error:", 6))
  {
    String sData = String(data->error);
    int errNum = sData.substring(6).toInt();
    // SerialBT.print("Error: ");
    // SerialBT.print(sData);
    // SerialBT.print(", parsedNumber: ");
    // SerialBT.println(errNum);
    processMachineError(errNum);
    clearGrblError();
  }
  if (!strncmp(data->alarm, "ALARM:", 6))
  {
    String sData = String(data->alarm);
    int alarmNum = sData.substring(6).toInt();
    // SerialBT.print("Alarm: ");
    // SerialBT.print(sData);
    // SerialBT.print(", parsedAlarm: ");
    // SerialBT.println(alarmNum);
    processAlarm(alarmNum);
    clearGrblAlarm();
  }
  if (!strncmp(data->message, "[MSG:", 5))
  {
    wMessage = String(data->message).substring(5);
    clearGrblMessage();
  }
  // delay(1);
  hubTick();
  machineTick();
}

void clearMessages()
{
  clearGrblAlarm();
  clearGrblError();
  clearGrblMessage();
  wMessage = "";
  wError = "";
  wAlarm = "";
}
