#ifndef SatCON_h
#define SatCON_h

#include <Arduino.h>

class SatCON {
    private:


    public:

    SatCON();
    boolean connect();
    boolean send(String message);
    tm getTime();
};

#endif