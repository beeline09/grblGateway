#include "espWiFi.h"
#include "Globals.h"
// #include <DNSServer.h>
#include <WiFi.h>
// #include <AsyncTCP.h>
// #include "ESPAsyncWebServer.h"
#include "prefs.h"
#include "hub.h"

// DNSServer dnsServer;
// AsyncWebServer server(80);

// class CaptiveRequestHandler : public AsyncWebHandler {
//   public:
//     CaptiveRequestHandler() {}
//     virtual ~CaptiveRequestHandler() {}

//     bool canHandle(AsyncWebServerRequest *request) {
//       //request->addInterestingHeader("ANY");
//       return true;
//     }

//     void handleRequest(AsyncWebServerRequest *request) {
//       request->send_P(200, "text/html", index_html);
//     }
// };

void initSoftAp()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP("GrblGateway");
    Serial.println("Start SoftAP");
    Serial.println(WiFi.softAPIP());
    // WiFi.begin(wifi_network_ssid, wifi_network_password);
    // dnsServer.start(53, "*", WiFi.softAPIP());
    // server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP
    // server.begin();
    // dnsServer.processNextRequest();
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
            // WiFi.begin(wifiName, wifiPass);
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