#ifndef SatCON_h
#define SatCON_h

//#include <Arduino.h> // NOTE: I don't think this is necessary

#include <IridiumSBD.h> // Click here to get the library: http://librarymanager/All#IridiumSBDI2C
#include <time.h>
#include <Wire.h>

#define IridiumWire Serial // ESP32 has 3 Serial ports: Serial, Serial1, Serial2
#define DIAGNOSTICS false

class SatCON {
  public:
    SatCON();
    void operator=(const SatCON &);
    boolean connect();
    boolean disconnect();
    boolean send(String message);
    String getTime();
  private:
    IridiumSBD modem = IridiumSBD(IridiumWire);
};

#endif
