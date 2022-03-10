#ifndef WifiCON_h
#define WifiCON_h

#include <Arduino.h>

class WifiCON {
  public:
    WifiCON();
    WifiCON(const char *ssid, const char *password, const char *serverName);
    boolean connect();
    boolean disconnect();
    boolean send(String message);
    std::vector<String> getTime();
    std::vector<String> WifiCON::split(String source, char delim);

  private:
    const char *ssid = nullptr;
    const char *password = nullptr;
    const char *serverName = nullptr;
};

#endif
