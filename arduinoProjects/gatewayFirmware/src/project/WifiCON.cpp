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

extern Screen screen;

WifiCON::WifiCON() {}

WifiCON::WifiCON(const char *ssid, const char *password, const char *serverName) {
  this->ssid = ssid;
  this->password = password;
  this->serverName = serverName;
}

boolean WifiCON::connect() {
  // Initialize Serial Monitor
  //Serial.begin(115200);
  screen.showUserDisplay("Testing WiFi");
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED && counter < 20) {
    delay(500);
    Serial.print(".");
    counter++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    // Print local IP address and start web server
    Serial.println();
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    screen.showUserDisplay("WiFi Connected");
    return true;
  } else {
    Serial.println("WiFi not connected.");
    return false;
  }
}

boolean WifiCON::disconnect() {
  //disconnect the wifi connection
  while (WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
  }
  Serial.println("");
  Serial.println("WiFi disconnected.");
}

boolean WifiCON::send(String message) {

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

std::vector<String> WifiCON::getTime() {
  // Should be in format HH:MM:SS

  // Start the time client
  timeClient.begin();
  // Adjust to MSTime
  timeClient.setTimeOffset(25200);

  // Makes sure that the client is up to date and synced
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  // The will get the formatted time
  formattedTime = timeClient.getFormattedTime();

  // For debug this will print to the terminal
  Serial.println(formattedTime);

  // Return the time to be added
  return split(formattedTime, ':');
}

std::vector<String> WifiCON::split(String source, char delim) {

  std::vector<String> result = std::vector<String>();

  int ind1;
  int ind2;
  String hours;
  String minutes;
  String seconds;


  ind1 = source.indexOf(delim);
  hours = source.substring(0, ind1);
  ind2 = source.indexOf(delim, ind1 + 1);
  minutes = source.substring(ind1 + 1, ind2);
  seconds = source.substring(ind2 + 1);

  result.push_back(hours);
  result.push_back(minutes);
  result.push_back(seconds);

  return result;
}
