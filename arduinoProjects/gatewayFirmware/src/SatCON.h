#ifndef SatCON_h
#define SatCON_h

#include <Arduino.h>

#include <IridiumSBD.h> // Click here to get the library: http://librarymanager/All#IridiumSBDI2C
#include <time.h>
#include <Wire.h>

#define IridiumWire Wire
#define DIAGNOSTICS false

IridiumSBD modem(IridiumWire);

class SatCON {
    private:


    public:

    SatCON();
    boolean connect();
    boolean disconnect();
    boolean send(String message);
    tm getTime();
};

#endif