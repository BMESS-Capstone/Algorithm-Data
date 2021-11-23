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
bool WiFiCON = false;
bool SatCON = false;
bool CellCON = false;

// Wifi Settings
#include <WiFi.h>
#include <HTTPClient.h>
const char *ssid = "NetworkName";
const char *password = "Password";

// Cell Settings

// Sat Settings

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
}

void loop()
{
    // Case 1 - Wifi output connetion
    if (WiFiCON == true)
    {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(1000);
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            HTTPClient http;

            // You'll have to change to url/address you want
            http.begin(serverName);
            http.addHeader("Content-Type", "text/plain");

            // You might have to change this
            String toSend = fullLoop();

            int httpResponseCode = http.POST(toSend);

            if (httpResponseCode > 0)
            {

                String response = http.getString();

                Serial.println("httpResponseCode");
                Serial.println(response);
            }
            else
            {
                Serial.println("Error on send");
                Serial.println("httpResponseCode");
            }
            http.end();
        }
    }

    // Case 2 - Cellular output connection
    else if (CellCON == true)
    {

        fullLoop();
    }

    // Case 3 - Satellite output connection
    else if (SatCON == true)
    {

        fullLoop();
    }

    // Case 4 - No output connection
    else
    {
        Serial.println("No connection to Internet");
    }
}

// Helpers

void readIntoIntArray()
{
    int entryNum = 0;
    char *s = (char *)Serial.read();
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == ',')
        {
            i++;
        }
        currentIntensityArray[entryNum] = s[i];
        entryNum++;
    }
}
//*******************End of Code Block******************************