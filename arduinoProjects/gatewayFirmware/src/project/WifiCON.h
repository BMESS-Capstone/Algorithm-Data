#ifndef WifiCON_h
#define WifiCON_h

#include <Arduino.h>

class WifiCON {
  public:
    WifiCON();
    WifiCON(const char *ssid, const char *password, String serverName);
    boolean connect();
    boolean disconnect();
    boolean send(String message);
    String getTime();

  private:
    const char *ssid = nullptr;
    const char *password = nullptr;
    String serverName;
};

#endif
