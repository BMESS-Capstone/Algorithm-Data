#ifndef CellCON_h
#define CellCON_h

#include <Arduino.h>
#include <SIM800L.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(2, 3);

#define SIM800_RST_PIN 6

const char APN[] = "Internet.be";
const char URL[] = "https://postman-echo.com/post";
const char CONTENT_TYPE[] = "application/json";

SIM800L* sim800l;

class CellCON {
    private:


    public:

    CellCON();
    boolean connect();
    boolean disconnect();
    void setupModule();
    boolean send(String message);
    String getTime();
};

#endif