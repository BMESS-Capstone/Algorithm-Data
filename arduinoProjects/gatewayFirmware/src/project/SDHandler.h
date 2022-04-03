#ifndef SDHandler_h
#define SDHandler_h

#include <SPI.h>
#include <SD.h>
#include "FS.h"


//LEFT TO DO: 
//Create new File on startup with name that is date
//Streamline code for Jeff to implement 
//OBJECT 
class SDHandler{
  public:
    SDHandler();
    File fd;
    char fileName[10] = "/File.txt"; // SD library only supports up to 8.3 names 
    const uint8_t chipSelect = 25;
    const uint8_t cardDetect = 26;
    bool alreadyBegan = false;  // SD.begin() misbehaves if not first call
    
 
    void write2SD(unsigned char* message, uint8_t index); //this is what actually writes to the SD

    bool initializeCard(void);
    

};
#endif
