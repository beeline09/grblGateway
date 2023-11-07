#include <Arduino.h>
#include "Globals.h"
#include "espWiFi.h"
#include "hub.h"
#include "bt.h"
#include "grbl/grbl_chat.h"

// void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
// {
//   if (event == ESP_SPP_SRV_OPEN_EVT)
//   {
//     Serial.println("Client Connected");

//     Serial.println("Client Connected has address:");

//     for (int i = 0; i < 6; i++)
//     {
//       Serial.printf("%02X", param->srv_open.rem_bda[i]);
//       if (i < 5)
//       {
//         Serial.print(":");
//       }
//     }
//     Serial.println("--------------");
//   }
// }

void setup()
{
  Serial.begin(115200);

  // SerialBT.register_callback(callback);

  initBt();

  initWiFi();
  // initHub();
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
  WposX = getData()->position[0];
  WposY = getData()->position[1];
  WposZ = getData()->position[2];
  WposXR = getData()->offset[0];
  WposYR = getData()->offset[1];
  WposZR = getData()->offset[2];
  spindelValue = getData()->spindle.rpm_programmed;

  // delay(1);
  hubTick();
}