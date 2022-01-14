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
algo ALGO = algo();

// Communication case variable
// 1 is wifi connection
// 2 is cell connection
// 3 is satellite connection
// Default is no connection
int var = 0;

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
bool RTCset = false;

int StO2entry = 0;

//********************Standard Code Block***************************

void setup()
{
    delay(1000);
    Serial.begin(115200);
    SerialBT.begin("ESP32test");

    // Objects of each connection typede
    WFCon = WifiCON(ssid, password, serverName);
    STCon = SatCON();
    CLCon = CellCON();

    while(RTCset==false)
    // Add the RTC update here
    if(WFCon.connect()==true){
        //set RTC using wifi
        RTCset = true;
    }
    else if(CLCon.connect()==true){
        //set RTC using cell
        RTCset = true;
    }
    else if(STCon.connect()==true){
        //set RTC using sat
        RTCset = true;
    }
    else
    continue;
}

void loop()
{
    // Do a loop until storage is full
    String message = ALGO.fullLoop();

    switch(var){
        case 1:
            // Send the message
            WFCon.connect();
            WFCon.send(message);
            WFCon.disconnect();
            break;

        case 2:
            CLCon.connect();
            CLCon.send(message);
            CLCon.disconnect();
            break;

        case 3:
            STCon.connect();
            STCon.send(message);
            STCon.disconnect();
            break;

        default:
            Serial.println("No connection to Internet");
            // If we want more lights and sirens, put them here

            // We could add an if statement here
            if(WFCon.connect()==true){
                var = 1;
                break;
            }
            if(CLCon.connect()==true){
                var = 2;
                break;
            }
            if(STCon.connect()==true){
                var = 3;
                break;
            }
            break;
    }
}
//*******************End of Code Block******************************