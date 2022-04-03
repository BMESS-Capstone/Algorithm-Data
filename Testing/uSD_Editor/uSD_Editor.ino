//Test file showing implementation of the SDHandler Code 
extern boolean SDFlag = false;
#include "SDHandler.h"
SDHandler mySD;
String message = "2022-04-02";


void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
}


void setup(){
    Serial.begin(115200);

}

void loop(){
  for(int i = 0; i<10; i++){
    char buffer[20];
            
    message.toCharArray(buffer, 11);
    Serial.println(message); 
    mySD.write2SD((unsigned char *)buffer, 11);
  }
  readFile(SD, "/File.txt");
  
}
