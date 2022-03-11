#include SDHandler_h

#include <SPI.h>
#include <SD.h>
#include "FS.h"

extern boolean SDFlag;

//IF TIME PERMITS, ADD THE OPTION OF CHANGIN FILE NAME
  SDHandler::SDHandler(){
  pinMode(chipSelect, OUTPUT);
  pinMode(cardDetect, INPUT);  
}


//Called everytime a message is ready - implement in code by adding this function 
//message = message to be saved 
//index = length of message in bytes 
//NOTE: Index for current message structure is 161 bytes
void SDHandler::write2SD(unsigned char* message, uint8_t index){  
  
  //check card is working
  
  initializeCard();
  
  //Now write data to SD Card
  Serial.print("Opening file: ");
  Serial.println(fileName);
  Serial.print(fileName);
  
  if (SD.exists(fileName))
  {                         // file exists 
    //Serial.println(F(" exists."));
     fd = SD.open(fileName, FILE_APPEND);
  }
  else
  {
    //Serial.println(F(" doesn't exist. Creating."));
  
    fd = SD.open(fileName, FILE_WRITE);
  }
  if(fd){
    fd.write(message, index);
    fd.write('@'); //delimiter for parsing
    fd.flush(); 
    fd.close();
    SDFlag = True; 
  }
  else{
    //Serial.println("Failed to open file");
    SDFlag = False;
  }
    
}


void SDHandler::initializeCard(void)
{
  Serial.print(F("Initializing SD card..."));
  
  if(!SD.begin(25)){
    Serial.println("Card Mount Failed"); //card is not recognized .. check connection
    return;
  }
  
  // Card seems to exist.  begin() returns failure
  // even if it worked if it's not the first call.
  if (!SD.begin(chipSelect) && !alreadyBegan)  // begin uses half-speed...
  {
    Serial.println(F("Initialization failed!"));
    //initializeCard(); // Possible infinite retry loop is as valid as anything
  }
  else
  {
    alreadyBegan = true;
  }
  Serial.println(F("Initialization done."));

}

