#ifndef CellCON_h
#define CellCON_h

#include <Arduino.h>
#include <SIM800L.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(2, 3);

#define SIM800_RST_PIN 6

SIM800L* sim800l;

class CellCON {
    protected:
    char APN[];
    char URL[];
    char CONTENT_TYPE[];

    public:

    CellCON();
    boolean connect();
    CellCON(const char* APN, const char* URL, const char* CT);
    boolean disconnect();
    void setupModule();
    boolean send(String message);
    String getTime();
};

#endif