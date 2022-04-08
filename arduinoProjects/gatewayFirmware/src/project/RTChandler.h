#ifndef RTChandler_h
#define RTChandler_h

#include <MCP7940.h>
#include <vector>

#define SERIAL_SPEED 115200
#define SPRINTF_BUFFER_SIZE 32

class RTChandler {
  public:
    RTChandler();

    boolean setupRTC();
    void setTime(std::vector<String> components);
    String getTime();

    DateTime now;
  private:
    MCP7940_Class MCP7940;                    // Create an instance of the MCP7940
    char inputBuffer[SPRINTF_BUFFER_SIZE];    // Buffer for sprintf()/sscanf()
};


#endif
