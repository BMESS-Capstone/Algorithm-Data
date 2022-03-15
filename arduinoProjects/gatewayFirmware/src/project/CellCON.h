#ifndef CellCON_h
#define CellCON_h

#include <SIM800L.h>
#include <SoftwareSerial.h>
#include <vector>

#define SIM800_RST_PIN 6

#define RX_PIN 2
#define TX_PIN 3

class CellCON {
  public:
    CellCON();
    CellCON(const char* APN, const char* URL, const char* CT);
    void operator=(const CellCON &);
    boolean connect();
    boolean disconnect();
    void setupModule();
    boolean send(String message);
    std::vector<String> getTime();
    std::vector<String> split(String source, char delim);

  private:
    SIM800L *sim800l;
    SoftwareSerial softSerial;
    const char *APN = nullptr;
    const char *URL = nullptr;
    const char *CONTENT_TYPE = nullptr;
};

#endif
