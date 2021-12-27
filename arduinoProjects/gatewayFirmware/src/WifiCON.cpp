#include "WifiCON.h"
#include "algo.h"

// Wifi Settings
#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>

const char *ssid = "NetworkName";
const char *password = "Password";

// Server Settings
String serverName = "the Server Address... Replace this";

// Time Settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Time variables
String formattedDate;
String dayStamp;
String timeStamp;

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

boolean WifiCON::send(String message){ 
    
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        // You'll have to change to url/address you want
        http.begin(serverName);
        http.addHeader("Content-Type", "text/plain");

        // You might have to change this
        String toSend = message;

        int httpResponseCode = http.POST(toSend);

        // If the response code is a success
        if (httpResponseCode > 0)
        {
            
            // Actually need to make a json here
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

    // Start the time client
    timeClient.begin();
    // Adjust to MSTime
    timeClient.setTimeOffset(25200);

    // Makes sure that the client is up to date and synced
    while(!timeClient.update()) {
        timeClient.forceUpdate();
    }

    // The will get the formatted time
    formattedDate = timeClient.getFormattedTime();

    // For debug this will print to the terminal
    Serial.println(formattedDate);

    // Return the time to be added
    return formattedDate;
}


