#ifndef SatCON_h
#define SatCON_h

#include <IridiumSBD.h> // Click here to get the library: http://librarymanager/All#IridiumSBDI2C
#include <time.h>
#include <Wire.h>
#include <vector>

#define IridiumWire Wire // ESP32 has 3 Serial ports: Serial, Serial1, Serial2
#define DIAGNOSTICS false

class SatCON {
  public:
    SatCON();
    void operator=(const SatCON &);
    boolean connect();
    boolean disconnect();
    boolean send(String message);
    std::vector<String> getTime();
    std::vector<String> split(String source, char delim);
    int signalQuality = -1;
  private:
    IridiumSBD modem = IridiumSBD(IridiumWire);
};

#endif
