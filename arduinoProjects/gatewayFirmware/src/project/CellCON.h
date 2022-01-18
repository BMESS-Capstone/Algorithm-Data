#ifndef CellCON_h
#define CellCON_h

//#include <Arduino.h> // NOTE: I don't think this is necessary
#include <SIM800L.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(2, 3);

#define SIM800_RST_PIN 6

SIM800L* sim800l;

class CellCON {
  public:
    CellCON();
    boolean connect();
    CellCON(const char* APN, const char* URL, const char* CT);
    boolean disconnect();
    void setupModule();
    boolean send(String message);
    String getTime();

  protected:
    char APN[];
    char URL[];
    char CONTENT_TYPE[];
};

#endif
