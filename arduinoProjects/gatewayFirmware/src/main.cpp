#include <iostream>

// Main Arduino
#include <Arduino.h>

// Bluetooth
#include "BluetoothSerial.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Algorithm
#include "algo.h"

// Booleans to decide which output method to use
// Might have to hardcode one to be true until the hardware switch is built
bool isWifiConnected = false;
bool isSatConnected = false;
bool isCellConnected = false;

// Wifi Settings
#include "WifiCON.h"
#include <WiFi.h>
#include <HTTPClient.h>
const char *ssid = "NetworkName";
const char *password = "Password";
WifiCON WFCon;

// Cell Settings
#include "CellCON.h"
CellCON CLCon;

// Sat Settings
#include "SatCON.h"
SatCON STCon;

// Server Settings
String serverName = "the Server Address... Replace this";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// Change to false ***
bool initial = true;

int StO2entry = 0;

//********************Standard Code Block***************************

void setup()
{
    delay(1000);
    Serial.begin(115200);
    SerialBT.begin("ESP32test");

    // Objects of each connection typede
    WFCon = WifiCON(ssid,password);
    SatCON STCon = SatCON();
    CellCON CLCon = CellCON();
}

void loop()
{
    // Do a loop until storage is full
    String message = fullLoop();

    // Case 1 - Wifi output connetion
    if (isWifiConnected == true){
        // Send the message
        WFCon.send(message);
    }

    // Case 2 - Cellular output connection
    else if (isCellConnected == true)
    {
        CLCon.send(message);
    }

    // Case 3 - Satellite output connection
    else if  (isSatConnected == true)
    {
        STCon.send(message);
    }

    // Case 4 - No output connection
    else
    {
        Serial.println("No connection to Internet");
    }
}
//*******************End of Code Block******************************