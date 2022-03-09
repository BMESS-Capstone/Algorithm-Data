// Wifi Settings
#include "WifiCON.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
const char *ssid = "TELUS7125";
const char *password = "JR27uCVbdh3B";
const char *serverName = "https://pirfusix-solutions.herokuapp.com/tripdata/post";
const char *testServerName = "https://pirfusix-solutions.herokuapp.com/tripdata/test";
const char *receiveServerName = "https://pirfusix-solutions.herokuapp.com/tripdata/get";
WifiCON WFCon;
// The message
String json = "{\"testString\":\"new test case\"}";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WFCon = WifiCON(ssid, password, testServerName);
  if(WFCon.connect() != true){
    Serial.println("There was an error connecting to the network");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i =0; i<5; i++){
    if(WFCon.send(json) == false){
      Serial.println("There was an error sending the file");
      while(1);
    }
    Serial.println("The sending of 5 JSON was successful");
  }
  String time = WFCon.getTime();
  Serial.println("The time is:");
  Serial.println(time);
}
