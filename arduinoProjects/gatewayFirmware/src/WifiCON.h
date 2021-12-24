#ifndef WifiCON_h
#define WifiCON_h

#include <Arduino.h>

class WifiCON { 
    private:
    const char *ssid = nullptr;
    const char *password = nullptr;


    public:

    WifiCON();
    WifiCON(const char *ssid, const char *password);
    boolean connect();
    boolean send(String message);
    tm getTime();
};

#endif 