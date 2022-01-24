#include <iostream>

// Main Arduino
#include <Arduino.h>

// Algorithm
#include "algo.h"
algo ALGO = algo();

// Communication case variable
// 1 is wifi connection
// 2 is cell connection
// 3 is satellite connection
// Default is no connection
int var = 1;

// Wifi Settings
#include "WifiCON.h"
#include <WiFi.h>
#include <HTTPClient.h>
const char *ssid = "NetworkName";
const char *password = "Password";
WifiCON WFCon;

// Cell Settings
#include "CellCON.h"
const char *APN = "Some APN Settings";
const char *URL = "http://www.google.com";
const char *CONTENT_TYPE = "application/json";
CellCON CLCon;

// Sat Settings
#include "SatCON.h"
SatCON STCon;

// Server Settings
const char *serverName = "the Server Address... Replace this";

// Change to false ***
bool initial = true;
bool RTCset = false;

int StO2entry = 0;

//********************Standard Code Block***************************

void setup()
{
  Serial.begin(115200);

  // Objects of each connection typede
  CLCon = CellCON(APN, URL, CONTENT_TYPE);
  WFCon = WifiCON(ssid, password, serverName);
  STCon = SatCON();

  while (RTCset == false)
    // Add the RTC update here
    // hh:mm:ss
    if (WFCon.connect() == true) {
      //set RTC using wifi
      
      RTCset = true;
    }
    else if (CLCon.connect() == true) {
      //set RTC using cell

      RTCset = true;
    }
    else if (STCon.connect() == true) {
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

  switch (var) {
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
      if (WFCon.connect() == true) {
        var = 1;
        break;
      }
      if (CLCon.connect() == true) {
        var = 2;
        break;
      }
      if (STCon.connect() == true) {
        var = 3;
        break;
      }
      break;
  }
}
//*******************End of Code Block******************************
