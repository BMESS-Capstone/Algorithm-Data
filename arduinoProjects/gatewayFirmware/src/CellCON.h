#ifndef CellCON_h
#define CellCON_h

#include <Arduino.h>
#include "Sim800l.h"

#define SIM800_RST_PIN 6

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