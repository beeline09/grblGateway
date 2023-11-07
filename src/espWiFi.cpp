#include "espWiFi.h"
#include "Globals.h"
#include <WiFi.h>
#include "prefs.h"
#include "hub.h"

void initSoftAp()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP("GrblGateway");
    Serial.println("Start SoftAP");
    Serial.println(WiFi.softAPIP());
    initHub();
}

void initWiFi()
{
    static int connectionAttempts = 0;
    WiFi.disconnect(true, true);
    String wifiName = getWifiName();
    String wifiPass = getWifiPassword();
    Serial.print("WiFi Name: ");
    Serial.println(wifiName);
    Serial.print("WiFi Pass: ");
    Serial.println(wifiPass);
    if (wifiName.length() < 1)
    {
        initSoftAp();
    }
    else
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(wifiName, wifiPass);
        while (WiFi.status() != WL_CONNECTED && connectionAttempts <= 10)
        {
            delay(1000);
            Serial.print(".");
            connectionAttempts++;
        }
        if (connectionAttempts > 10)
        {
            initSoftAp();
            connectionAttempts = 0;
            return;
        }
        connectionAttempts = 0;
        Serial.println(WiFi.localIP());
        initHub();
    }
}