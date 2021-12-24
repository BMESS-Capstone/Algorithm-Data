#ifndef CellCON_h
#define CellCON_h

#include <Arduino.h>

class CellCON {
    private:


    public:

    CellCON();
    boolean connect();
    boolean send(String message);
    tm getTime();
};

#endif