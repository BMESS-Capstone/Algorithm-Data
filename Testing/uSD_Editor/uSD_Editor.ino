//Test file showing implementation of the SDHandler Code 
extern boolean SDFlag = false;
#include "SDHandler.h"
SDHandler mySD;
String message = "2022-03-21";
void setup(){
}

void loop(){
char buffer[20];
        
message.toCharArray(buffer, 20);
mySD.write2SD((unsigned char *)buffer, 20);
  
}
