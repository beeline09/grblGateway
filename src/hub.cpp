#include "hub.h"
#include "prefs.h"
#include "espWiFi.h"
#include "bt.h"
#include "Globals.h"
#include "grbl/grbl_chat.h"

GyverPortal ui;

String wName;
String wSoftName;
String btName;
String wPass;
int accXYIndex = 5;
int accZIndex = 4;
int stepSizeXYIndex = 7;
int stepSizeZIndex = 5;

uint16_t spindelRpmSliderMin;
uint16_t spindelRpmSliderMax;
uint16_t spindelRpmSliderCurrent;

GP_PGM(GP_TRANSP, "#00000000");

void buildMainApp()
{
    GP.BUILD_BEGIN(GP_DARK);
    GP.ALERT("alert1");

    GP.UI_MENU("Cnc3018 Pro", GP_RED); // начать меню

    // список имён компонентов на обновление
    GP.UPDATE("xAxis,yAxis,zAxis,wName,wPass,alert1,wSoftName,btName,spindelRPM,spindelRpmSlider,xAxisR,yAxisR,zAxisR,wMessage,wError,wAlarm,wSpMinRpm,wSpMaxRpm");

    GP.PAGE_TITLE("CNC Portal");

    GP.JS_BEGIN();
    GP.SEND(
        "function setSliderWidth() { var highlightedItems = document.querySelectorAll(\"select\"); highlightedItems.forEach(function (userItem) { userItem.style.width = '100%';});}");
    GP.JS_END();

    // ссылки меню
    GP.UI_LINK("/", "Control");
    GP.UI_LINK("/sd", "Print from SD");
    GP.UI_LINK("/settings", "Settings");
    GP.UI_LINK("/info", "Info");

    // кастомный контент
    GP.HR(GP_GRAY);
    GP.LABEL("Some label!");
    GP.BREAK();
    GP.LABEL(ui.getSystemTime().encode());
    GP.BUTTON_MINI("btn", "Button");

    // начать основное окно
    GP.UI_BODY();

    // далее рандомный интерфейс для теста
    // позволяет "отключить" таблицу при ширине экрана меньше 700px
    GP.GRID_RESPONSIVE(1080);

    if (ui.uri("/"))
    {
        M_GRID(
            M_BLOCK_TAB(
                "Control",
                M_BOX(GP.TITLE("Axis"); GP.TITLE("Position"); GP.TITLE("Offset"););
                M_BOX(GP.LABEL("  X  "); GP.LABEL("", "xAxis"); GP.LABEL("", "xAxisR"););
                M_BOX(GP.LABEL("  Y  "); GP.LABEL("", "yAxis"); GP.LABEL("", "yAxisR"););
                M_BOX(GP.LABEL("  Z  "); GP.LABEL("", "zAxis"); GP.LABEL("", "zAxisR"););
                GP.LABEL("\u3164");
                M_BOX(GP.LABEL("Spindel "); GP.LABEL("", "spindelRPM"););
                GP.LABEL("\u3164");
                M_BOX(GP.LABEL("Message "); GP.LABEL("", "wMessage"););
                M_BOX(GP.LABEL("Error "); GP.LABEL("", "wError"););
                M_BOX(GP.LABEL("Alarm "); GP.LABEL("", "wAlarm"););
                GP.LABEL("\u3164");
                M_BOX(GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true); GP.BUTTON("moveY_UP", "Y+"); GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true); GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true); GP.BUTTON("moveZ_UP", "Z+"););
                M_BOX(GP.BUTTON("moveX_LEFT", "X-"); GP.BUTTON("spindelPower", "💥 Spindel"); GP.BUTTON("moveX_RIGHT", "X+"); GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true); GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true););
                M_BOX(GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true); GP.BUTTON("moveY_DOWN", "Y-"); GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true); GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true); GP.BUTTON("moveZ_DOWN", "Z-"););
                M_BOX(GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true););
                M_BOX(GP.BUTTON("softReset", "↻ Soft Reset"); GP.BUTTON("resetZero", "⓿ Reset Zero"););
                M_BOX(GP.BUTTON("unlock", "🔓 Unlock"); GP.BUTTON("move_HOME", "⌂ Home"););
                M_BOX(GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true););
                M_BOX(GP.TITLE("\u3164"); GP.TITLE("X,Y"); GP.TITLE("Z\u3164\u3164\u3164\u3164"););
                M_BOX(
                    GP.LABEL("\u3164\u3164Step size"); GP.SELECT("stepSizeXY", "0.001,0.025,0.05,0.1,0.5,1,5,10,100", stepSizeXYIndex); GP.SELECT("stepSizeZ", "0.001,0.025,0.05,0.1,0.5,1,5,10", stepSizeZIndex););
                M_BOX(
                    GP.LABEL("Acceleration ");
                    GP.SELECT("accelerationXY", "1,5,10,25,50,100,250,500,1000,2500,5000,10000", accXYIndex); GP.SELECT("accelerationZ", "1,5,10,25,50,100,250,500", accZIndex););
                GP.LABEL("\u3164");
                M_BOX(
                    GP.LABEL("Spindel RPM ");
                    GP.SLIDER("spindelRpmSlider", spindelRpmSliderCurrent, spindelRpmSliderMin, spindelRpmSliderMax, 10.0););

            ););
    }
    else if (ui.uri("/settings"))
    {
        GP.LABEL("Настройки");
        M_GRID(
            M_BLOCK_TAB(
                "Wi-Fi",
                M_BOX(GP.LABEL("Network name"); GP.TEXT("wName", "", wName););
                M_BOX(GP.LABEL("Password"); GP.TEXT("wPass", "", wPass););
                GP.PLAIN(/*"Если в течение нескольких попыток не получится подключиться к сети с этими параметрами, то будет запущена WiFi SoftAP"*/ "If within several attempts it is not possible to connect to the network with these parameters, WiFi SoftAP will be launched");
                GP.BUTTON("saveWifiParams", "Save");););
        M_GRID(
            M_BLOCK_TAB(
                "Wi-Fi SoftAP",
                M_BOX(GP.LABEL("Network name"); GP.TEXT("wSoftName", "", wSoftName););
                GP.BUTTON("saveWifiSoftAPParams", "Save");););
        M_GRID(
            M_BLOCK_TAB(
                "Bluetooth",
                M_BOX(GP.LABEL("Name"); GP.TEXT("btName", "", btName););
                GP.BUTTON("saveBtParams", "Save");););
        M_GRID(
            M_BLOCK_TAB(
                "CNC params",
                M_BOX(GP.LABEL("Spindel min RPM"); GP.NUMBER("wSpMinRpm", "", spindleMinRpm););
                M_BOX(GP.LABEL("Spindel max RPM"); GP.NUMBER("wSpMaxRpm", "", spindleMaxRpm););
                GP.PLAIN(/*"Если в течение нескольких попыток не получится подключиться к сети с этими параметрами, то будет запущена WiFi SoftAP"*/ "If within several attempts it is not possible to connect to the network with these parameters, WiFi SoftAP will be launched");
                GP.BUTTON("saveCncParams", "Save");););
    }
    else if (ui.uri("/info"))
    {
        M_GRID(
            M_BLOCK_TAB(
                "Information",
                GP.SYSTEM_INFO("1.0.0-alpha"););); // + версия вашей программы (в таблице появится строка Version с указанным текстом), [строка]););
    }
    GP.JS_BEGIN();
    GP.SEND("setSliderWidth();");
    GP.JS_END();
    GP.UI_END(); // завершить окно панели управления <<<<-------------
    GP.BUILD_END();
}

// билдер
void build()
{
    buildMainApp();
    Serial.println("Запускаем портал");
}

void actionLabels()
{
    if (ui.clickString("wName", wName))
    {
        Serial.print("wName: ");
        Serial.println(wName);
    }
    else if (ui.clickString("wPass", wPass))
    {
        Serial.print("wPass: ");
        Serial.println(wPass);
    }
    else if (ui.clickString("wSoftName", wSoftName))
    {
        Serial.print("wSoftName: ");
        Serial.println(wSoftName);
    }
    else if (ui.clickString("btName", btName))
    {
        Serial.print("btName: ");
        Serial.println(btName);
    }
    else if (ui.clickInt("wSpMinRpm", spindleMinRpm))
    {
        char bigBuf[100] = "";
        sprintf_P(bigBuf, "wSpMinRpm: %d", spindleMinRpm);
        Serial.println(bigBuf);
    }
    else if (ui.clickInt("wSpMaxRpm", spindleMaxRpm))
    {
        char bigBuf[100] = "";
        sprintf_P(bigBuf, "wSpMaxRpm: %d", spindleMaxRpm);
        Serial.println(bigBuf);
    }
}

int alert = 0;
void action()
{
    // было обновление
    if (ui.update())
    {
        //  grbl_data_t *data = getData();
        //  String xValue = String(data->position[0]);
        ui.updateString("xAxis", WposX);
        ui.updateString("xAxisR", WposXR);
        ui.updateString("yAxis", WposY);
        ui.updateString("yAxisR", WposYR);
        ui.updateString("zAxis", WposZ);
        ui.updateString("zAxisR", WposZR);
        ui.updateFloat("spindelRPM", spindelValue, 0);
        ui.updateString("wMessage", wMessage);
        ui.updateString("wError", wError);
        ui.updateString("wAlarm", wAlarm);
        //   ui.updateString("pass", valPass);
        //    ui.updateString("pass", valPass);
        if (ui.update("alert1"))
        {
            if (alert == 1)
            {
                alert = 0;
                ui.answer(String("Network name must be greater or equal 3 symbols! " + wName));
            }
            else if (alert == 2)
            {
                alert = 0;
                ui.answer(String("Software AP name must be greater or equal 3 symbols! " + wSoftName));
            }
            else if (alert == 3)
            {
                alert = 0;
                ui.answer(String("Device name must be greater or equal 3 symbols! " + btName));
            }
            else if (alert == 4)
            {
                alert = 0;
                ui.answer(String("The minimum spindle speed must be greater than or equal to zero. The maximum speed must be greater than the minimum!"));
            }
        }
    }
    if (ui.click())
    {
        actionLabels();
        if (ui.click("btn"))
        {
            WposX = "Haha";
        }
        // Сохраняем параметры подключения к существующей WiFi сети
        else if (ui.click("saveWifiParams"))
        {
            if (wName.length() <= 3)
            {
                alert = 1;
            }
            else
            {
                saveWiFiName(wName);
                saveWiFiPassword(wPass);
                delay(1000);
                initWiFi();
            }
        }
        // Сохраняем название встроенной ТД
        else if (ui.click("saveWifiSoftAPParams"))
        {
            Serial.println(wSoftName);
            if (wSoftName.length() <= 3)
            {
                alert = 2;
            }
            else
            {
                saveWifiSoftApName(wSoftName);
                delay(100);
                initWiFi();
            }
        }
        else if (ui.click("saveBtParams"))
        {
            if (btName.length() <= 3)
            {
                alert = 3;
            }
            else
            {
                Serial.println("");
                Serial.print("btName: ");
                Serial.println(btName);

                saveBtName(btName);
                delay(100);
                initBt();
            }
        }
        else if (ui.click("move_HOME"))
        {
            clearMessages();
            Serial.println("$H");
            delay(50);
            // Serial.println("?");
        }
        else if (ui.click("softReset"))
        {
            clearMessages();
            Serial.write(0x18);
            delay(50);
            Serial.println("?");
        }
        else if (ui.click("resetZero"))
        {
            clearMessages();
            Serial.println("G10 P0 L20 X0 Y0 Z0");
            delay(50);
            // Serial.println("?");
        }
        else if (ui.click("unlock"))
        {
            clearMessages();
            Serial.println("$X");
            delay(50);
            // Serial.println("?");
        }
        else if (ui.click("saveCncParams"))
        {
            if (spindleMinRpm < 0 || spindleMaxRpm < spindleMinRpm)
            {
                alert = 4;
            }
            else
            {
                saveSpindleMinRpm(spindleMinRpm);
                saveSpindleMaxRpm(spindleMaxRpm);
                delay(1000);
                GP.RELOAD("saveCncParams");
            }
        }
        else if (ui.click("spindelRpmSlider"))
        {
            uint16_t rpm = (uint16_t)ui.getFloat("spindelRpmSlider");
            saveSpindelCurrentRpm(rpm);
            spindelRpmSliderCurrent = rpm;
            char bigBuf[100] = "";
            sprintf_P(bigBuf, "Selected spider rpm: %d", rpm);
            Serial.println(bigBuf);

            if (spindelValue > 0.0 && rpm > 0 && ((uint16_t)spindelValue - rpm) != 0)
            {
                char bigBuf[25] = "";
                sprintf_P(bigBuf, "M3 S%d", rpm);
                Serial.println(bigBuf);
            }
        }
        else if (ui.click("spindelPower"))
        {
            char bigBuf[100] = "";
            sprintf_P(bigBuf, "Selected spindel rpm: %d", spindelRpmSliderCurrent);
            Serial.println(bigBuf);
            if (spindelValue == 0.0 && spindelRpmSliderCurrent > 0)
            {
                char bigBuf[25] = "";
                sprintf_P(bigBuf, "M3 S%d", spindelRpmSliderCurrent);
                Serial.println(bigBuf);
            }
            else
            {
                Serial.println("M5");
            }
        }
    }
}

void initHub()
{
    wName = getWifiName();
    wPass = getWifiPassword();
    wSoftName = getWifiSoftApName();
    btName = getBtName();
    spindleMinRpm = getSpindelMinRpm();
    spindleMaxRpm = getSpindelMaxRpm();
    spindleCurrentRpm = getSpindelCurrentRpm();
    spindelRpmSliderMin = spindleMinRpm;
    spindelRpmSliderMax = spindleMaxRpm;
    spindelRpmSliderCurrent = spindleCurrentRpm;

    stepSizeXYIndex = 7;
    stepSizeZIndex = 5;

    //"1,5,10,25,50,100,250,500,1000,2500,5000,10000"
    switch (getAccelerationXY())
    {
    case 1:
        accXYIndex = 0;
        break;
    case 5:
        accXYIndex = 1;
        break;
    case 10:
        accXYIndex = 2;
        break;
    case 25:
        accXYIndex = 3;
        break;
    case 50:
        accXYIndex = 4;
        break;
    case 100:
        accXYIndex = 5;
        break;
    case 250:
        accXYIndex = 6;
        break;
    case 500:
        accXYIndex = 7;
        break;
    case 1000:
        accXYIndex = 8;
        break;
    case 2500:
        accXYIndex = 9;
        break;
    case 5000:
        accXYIndex = 10;
        break;
    case 10000:
        accXYIndex = 11;
        break;

    default:
        accXYIndex = 5;
        break;
    }

    //"1,5,10,25,50,100,250,500"
    switch (getAccelerationZ())
    {
    case 1:
        accZIndex = 0;
        break;
    case 5:
        accZIndex = 1;
        break;
    case 10:
        accZIndex = 2;
        break;
    case 25:
        accZIndex = 3;
        break;
    case 50:
        accZIndex = 4;
        break;
    case 100:
        accZIndex = 5;
        break;
    case 250:
        accZIndex = 6;
        break;
    case 500:
        accZIndex = 7;
        break;

    default:
        accZIndex = 4;
        break;
    }

    //"0.001,0.025,0.05,0.1,0.5,1,5,10,100"
    float szXY = getStepSizeXY();
    if (szXY == 0.001)
    {
        stepSizeXYIndex = 0;
    }
    else if (szXY == 0.025)
    {
        stepSizeXYIndex = 1;
    }
    else if (szXY == 0.05)
    {
        stepSizeXYIndex = 2;
    }
    else if (szXY == 0.1)
    {
        stepSizeXYIndex = 3;
    }
    else if (szXY == 0.5)
    {
        stepSizeXYIndex = 4;
    }
    else if (szXY == 1.0)
    {
        stepSizeXYIndex = 5;
    }
    else if (szXY == 5.0)
    {
        stepSizeXYIndex = 6;
    }
    else if (szXY == 10.0)
    {
        stepSizeXYIndex = 7;
    }
    else if (szXY == 100.0)
    {
        stepSizeXYIndex = 8;
    }
    //"0.001,0.025,0.05,0.1,0.5,1,5,10"
    float szZ = getStepSizeZ();
    if (szZ == 0.001)
    {
        stepSizeZIndex = 0;
    }
    else if (szZ == 0.025)
    {
        stepSizeZIndex = 1;
    }
    else if (szZ == 0.05)
    {
        stepSizeZIndex = 2;
    }
    else if (szZ == 0.1)
    {
        stepSizeZIndex = 3;
    }
    else if (szZ == 0.5)
    {
        stepSizeZIndex = 4;
    }
    else if (szZ == 1.0)
    {
        stepSizeZIndex = 5;
    }
    else if (szZ == 5.0)
    {
        stepSizeZIndex = 6;
    }
    else if (szZ == 10.0)
    {
        stepSizeZIndex = 7;
    }

    // подключаем конструктор и запускаем
    ui.attachBuild(build);
    ui.attach(action);
    if (ipAddress.length() > 0)
    {
        ui.start(ipAddress.c_str());
    }
    else
    {
        ui.start();
    }
}

void refreshHub()
{
    // hub.refresh();
    ui.update();
}

void hubTick()
{
    ui.tick();
}
