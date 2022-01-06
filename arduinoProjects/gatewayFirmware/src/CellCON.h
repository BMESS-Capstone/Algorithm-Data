#ifndef CellCON_h
#define CellCON_h

#include <Arduino.h>
#include "Sim800l.h"

#define SIM800_RST_PIN 6

const char APN[] = "Internet.be";
const char URL[] = "https://postman-echo.com/post";
const char CONTENT_TYPE[] = "application/json";
// *** This is what we need to change ***
// **************************************
char PAYLOAD[];

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