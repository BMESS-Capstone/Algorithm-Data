#ifndef CellCON_h
#define CellCON_h

#include <SIM800L.h>
#include <SoftwareSerial.h>

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
    String getTime();

  private:
    SIM800L *sim800l;
    SoftwareSerial softSerial;
    const char *APN = nullptr;
    const char *URL = nullptr;
    const char *CONTENT_TYPE = nullptr;
};

#endif
