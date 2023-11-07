#include "bt.h"
#include "Globals.h"
#include "prefs.h"

BluetoothSerial SerialBT;

void initBt()
{
    SerialBT.end();
    delay(1000);

    if (!SerialBT.begin(getBtName().c_str()))
    {
        Serial.println("An error occurred initializing Bluetooth");
    }
    else
    {
        Serial.println("Bluetooth initialized");
    }
}