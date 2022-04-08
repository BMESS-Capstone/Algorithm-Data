#include "RTChandler.h"

RTChandler::RTChandler() {}

boolean RTChandler::setupRTC() {
  //  Serial.begin(SERIAL_SPEED);  // Start serial port at Baud rate
#ifdef __AVR_ATmega32U4__      // If this is a 32U4 processor, then
  delay(3000);                 // wait 3 seconds for the serial
#endif                         // interface to initialize
  int counter = 0;
  while (!MCP7940.begin() && counter < 5) {  // Initialize RTC communications
    Serial.println(F("Unable to find MCP7940M. Checking again in 1s."));  // Show error text
    delay(1000);                                                          // wait a second
    counter++;
  }  // of loop until device is located
  if (counter >= 5) {
    return false;
  }
  Serial.println(F("MCP7940 initialized."));
  int counter_ds = 0;
  while (!MCP7940.deviceStatus() && counter_ds < 5) {  // Turn oscillator on if necessary
    Serial.println(F("Oscillator is off, turning it on."));
    bool deviceStatus = MCP7940.deviceStart();  // Start oscillator and return state
    int counter_os = 0;
    if (!deviceStatus && counter_os < 5) {                        // If it didn't start
      Serial.println(F("Oscillator did not start, trying again."));  // Show error and
      delay(1000);                                                   // wait for a second
      counter_os++;
    }                // of if-then oscillator didn't start
    if (counter_os >= 5) {
      return false;
    }
  }                  // of while the oscillator is off
  if (counter_ds >= 5) {
    return false;
  }

  MCP7940.adjust();  // Set to library compile Date/Time
  return true;
}

void RTChandler::setTime(std::vector<String> components) {
  MCP7940.calibrateOrAdjust(
    DateTime(2021, 3, 30, components.at(0).toInt(),
             components.at(1).toInt(), components.at(2).toInt())

  );  // Adjust the RTC date/time
  Serial.print(F("Date and time has been set."));
}

String RTChandler::getTime() {
  DateTime now = MCP7940.now();
  sprintf(inputBuffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  return inputBuffer;
}
