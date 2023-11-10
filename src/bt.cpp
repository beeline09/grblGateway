#include "bt.h"
#include "Globals.h"
#include "prefs.h"

BluetoothSerial SerialBT;
bool isBtConnected = false;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    // Serial.println("Client Connected");
    isBtConnected = true;
  }
  if(event == ESP_SPP_CLOSE_EVT ){
    // Serial.println("Client disconnected");
    isBtConnected = false;
  }
}

void initBt()
{
    SerialBT.end();
    delay(1000);

    SerialBT.register_callback(callback);

    if (!SerialBT.begin(getBtName().c_str()))
    {
        Serial.println("An error occurred initializing Bluetooth");
    }
    else
    {
        Serial.println("Bluetooth initialized");
    }
}