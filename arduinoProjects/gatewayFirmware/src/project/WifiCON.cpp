#include "WifiCON.h"

// Wifi Settings
#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>

// Time Settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Time variables
String formattedTime;
String dayStamp;
String timeStamp;

WifiCON::WifiCON(){

}

WifiCON::WifiCON(const char *ssid, const char *password, String serverName){
    this->ssid = ssid;
    this->password = password;
    this->serverName = serverName;
}

boolean WifiCON::connect(){
    // Initialize Serial Monitor
    Serial.begin(115200);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

boolean WifiCON::disconnect(){
    //disconnect the wifi connection
    while(WiFi.status() == WL_CONNECTED){
        WiFi.disconnect();
    }
    Serial.println("");
    Serial.println("WiFi disconnected.");
}

boolean WifiCON::send(String message){ 
    
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        // You'll have to change to url/address you want
        http.begin(serverName);
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(message);

        // If the response code is a success
        if (httpResponseCode > 0)
        {
            String response = http.getString();

            Serial.println("httpResponseCode");
            Serial.println(response);

            http.end();
            return true;
        }
        // If the response code is a failure
        else
        {
            Serial.println("Error on send");
            Serial.println("httpResponseCode");

            http.end();
            return false;
        }
    }
}

String WifiCON::getTime(){ 
    // Should be in format HH:MM:SS

    // Start the time client
    timeClient.begin();
    // Adjust to MSTime
    timeClient.setTimeOffset(25200);

    // Makes sure that the client is up to date and synced
    while(!timeClient.update()) {
        timeClient.forceUpdate();
    }

    // The will get the formatted time
    formattedTime = timeClient.getFormattedTime();

    // For debug this will print to the terminal
    Serial.println(formattedTime);

    // Return the time to be added
    return formattedTime;
}


