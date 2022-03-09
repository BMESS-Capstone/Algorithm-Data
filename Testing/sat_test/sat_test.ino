// Sat settings
#include "SatCON.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

String json = "{\"testString\":\"sat test message\"}";
SatCON STCon;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  STCon = SatCON();
  if(STCon.connect() != true){
    Serial.println("There was an error connecting to the satellite.");
  }
  for(int i =0; i<5; i++){
    if(STCon.send(json) == false){
      Serial.println("There was an error sending the file");
    }
  }
  Serial.println("The sending of 5 JSON was successful");
  
  String time = STCon.getTime();
  Serial.println("The time is:");
  Serial.println(time);

  STCon.disconnect();
}

void loop() {
  // put your maivn code here, to run repeatedly:
  
}
