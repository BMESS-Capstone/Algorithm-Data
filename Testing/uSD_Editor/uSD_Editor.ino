//Test file showing implementation of the SDHandler Code 

#include "SDHandler.h"
SDHandler mySD;
String message = "2022-03-21";
void setup(){
}

void loop(){
mySD.write2SD((char*)message, 10);
  
}
