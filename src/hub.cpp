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

GP_PGM(GP_TRANSP, "#00000000");

void buildMainApp()
{
    GP.BUILD_BEGIN(GP_DARK);
    GP.ALERT("alert1");

    GP.UI_MENU("Cnc3018 Pro", GP_RED); // начать меню

    // список имён компонентов на обновление
    GP.UPDATE("xAxis,yAxis,zAxis,wName,wPass,alert1,wSoftName,btName,spindelRPM,xAxisR,yAxisR,zAxisR");

    GP.PAGE_TITLE("CNC Portal");

    // ссылки меню
    GP.UI_LINK("/", "Управление");
    GP.UI_LINK("/sd", "Печать с SD");
    GP.UI_LINK("/settings", "Настройки");

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
    GP.GRID_RESPONSIVE(700);

    if (ui.uri("/"))
    {

        M_GRID(
            M_BLOCK_TAB(
                "Управление",
                M_BOX(GP.LABEL("Ось X "); GP.LABEL("", "xAxis");GP.LABEL("", "xAxisR"););
                M_BOX(GP.LABEL("Ось Y "); GP.LABEL("", "yAxis");GP.LABEL("", "yAxisR"););
                M_BOX(GP.LABEL("Ось Z "); GP.LABEL("", "zAxis");GP.LABEL("", "zAxisR"););
                M_BOX(GP.LABEL("Шпиндель "); GP.LABEL("", "spindelRPM"););
                M_BOX(GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true););
                M_BOX(GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true); GP.BUTTON("moveY_UP", "▲"); GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true););
                M_BOX(GP.BUTTON("moveX_LEFT", "◄"); GP.BUTTON("moveX_HOME", "⌂"); GP.BUTTON("moveX_RIGHT", "►"););
                M_BOX(GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true); GP.BUTTON("moveY_DOWN", "▼"); GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true););
                M_BOX(GP.BUTTON("moveSpacer", "", "", GP_TRANSP, "", true););
                M_BOX(
                    GP.LABEL("Шаг "); GP.SELECT("stepSize", "0.001,0.025,0.05,0.1,0.5,1,5,10,100", 5););
                M_BOX(
                    GP.LABEL("Ускорение "); GP.SELECT("acceleration", "1,5,10,25,50,100,250,500,1000,2500,5000,10000", 5););););
    }
    else if (ui.uri("/settings"))
    {
        GP.LABEL("Настройки");
        M_GRID(
            M_BLOCK_TAB(
                "Wi-Fi",
                M_BOX(GP.LABEL("Имя сети"); GP.TEXT("wName", "", wName););
                M_BOX(GP.LABEL("Пароль"); GP.TEXT("wPass", "", wPass););
                GP.PLAIN("Если в течение нескольких попыток не получится подключиться к сети с этими параметрами, то будет запущена WiFi SoftAP");
                GP.BUTTON("saveWifiParams", "Сохранить");););
        M_GRID(
            M_BLOCK_TAB(
                "Wi-Fi SoftAP",
                M_BOX(GP.LABEL("Имя сети"); GP.TEXT("wSoftName", "", wSoftName););
                GP.BUTTON("saveWifiSoftAPParams", "Сохранить");););
        M_GRID(
            M_BLOCK_TAB(
                "Bluetooth",
                M_BOX(GP.LABEL("Имя"); GP.TEXT("btName", "", btName););
                GP.BUTTON("saveBtParams", "Сохранить");););
    }

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
        ui.updateString("spindelRPM", spindelValue);
        //   ui.updateString("pass", valPass);
        //    ui.updateString("pass", valPass);
        if (ui.update("alert1"))
        {
            if (alert == 1)
            {
                alert = 0;
                ui.answer(String("Так нельзя! " + wName));
            }
            else if (alert == 2)
            {
                alert = 0;
                ui.answer(String("Имя встроенной ТД должно быть длиннее трёх символов! " + wSoftName));
            }
            else if (alert == 3)
            {
                alert = 0;
                ui.answer(String("Имя устройства должно быть длиннее трёх символов! " + btName));
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
                Serial.print("btName: ");
                Serial.print(btName);

                saveBtName(btName);
                delay(100);
                initBt();
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

    // подключаем конструктор и запускаем
    ui.attachBuild(build);
    ui.attach(action);
    ui.start();
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
