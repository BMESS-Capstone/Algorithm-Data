#include "RTChandler.h"

RTChandler::RTChandler() {}

void RTChandler::setupRTC() {
  Serial.begin(SERIAL_SPEED);  // Start serial port at Baud rate
#ifdef __AVR_ATmega32U4__      // If this is a 32U4 processor, then
  delay(3000);                 // wait 3 seconds for the serial
#endif                         // interface to initialize
  Serial.print(F("\nStarting SetAndCalibrate program\n"));
  Serial.print(F("- Compiled with c++ version "));
  Serial.print(F(__VERSION__));  // Show compiler information
  Serial.print(F("\n- On "));
  Serial.print(F(__DATE__));
  Serial.print(F(" at "));
  Serial.print(F(__TIME__));
  Serial.print(F("\n"));
  while (!MCP7940.begin()) {  // Initialize RTC communications
    Serial.println(F("Unable to find MCP7940M. Checking again in 1s."));  // Show error text
    delay(1000);                                                          // wait a second
  }  // of loop until device is located
  Serial.println(F("MCP7940 initialized."));
  while (!MCP7940.deviceStatus()) {  // Turn oscillator on if necessary
    Serial.println(F("Oscillator is off, turning it on."));
    bool deviceStatus = MCP7940.deviceStart();  // Start oscillator and return state
    if (!deviceStatus) {                        // If it didn't start
      Serial.println(F("Oscillator did not start, trying again."));  // Show error and
      delay(1000);                                                   // wait for a second
    }                // of if-then oscillator didn't start
  }                  // of while the oscillator is off
  MCP7940.adjust();  // Set to library compile Date/Time
  Serial.println(F("Enter the following serial commands:"));
  Serial.println(F("SETDATE yyyy-mm-dd hh:mm:ss"));
  Serial.println(F("CALDATE yyyy-mm-dd hh:mm:ss"));
}

void RTChandler::setTime(std::vector<String> components) {

  MCP7940.adjust(

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
