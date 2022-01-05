#ifndef CellCON_h
#define CellCON_h

#include <Arduino.h>
#include "Sim800l.h"

class CellCON {
    private:


    public:

    CellCON();
    boolean connect();
    boolean send(String message);
    String getTime();
};

#endif