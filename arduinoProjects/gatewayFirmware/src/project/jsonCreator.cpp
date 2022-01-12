#include "jsonCreator.h"

jsonCreator::jsonCreator(){

}

void jsonCreator::startUp(){
    Serial.begin(115200);
    Serial.println();
}

String jsonCreator::createJSON(String measurements, String time){
    return "Not completed.";
}

String jsonCreator::createErrJSON(){
    return "Not completed.";
}