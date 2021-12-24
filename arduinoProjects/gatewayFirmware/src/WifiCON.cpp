#include "WifiCON.h"
#include "algo.h"

// Wifi Settings
#include <WiFi.h>
#include <HTTPClient.h>
const char *ssid = "NetworkName";
const char *password = "Password";

// Server Settings
String serverName = "the Server Address... Replace this";

boolean WifiCON::send(String message){ 
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
        String toSend = message;

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