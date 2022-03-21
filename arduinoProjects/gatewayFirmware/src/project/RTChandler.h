#ifndef RTChandler_h
#define RTChandler_h

#include <MCP7940.h>

const uint32_t SERIAL_SPEED{115200};     // Set the baud rate for Serial I/O
const uint8_t  LED_PIN{13};              // Arduino built-in LED pin number
const uint8_t  SPRINTF_BUFFER_SIZE{32};  // Buffer size for sprintf()

MCP7940_Class MCP7940;                           // Create an instance of the MCP7940
char          inputBuffer[SPRINTF_BUFFER_SIZE];  // Buffer for sprintf()/sscanf()

class RTChandler{
    public:
    RTChandler();

    void setupRTC();
    void setTime(std::vector<String> components);
    String getTime();

    DateTime now;
};


#endif