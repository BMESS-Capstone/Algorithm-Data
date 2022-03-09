//
// Created by Jeff Roszell on 2022-03-01.
//

#include "SDHandler.h"

SDHandler::SDHandler(){
    this->numEntries=0;
    this->storage = std::vector<String>();
}

void SDHandler::store(String message){
    storage.push_back(message);
    this->numEntries++;
}

String SDHandler::retrieve(){
    if(numEntries>0){
        String message = storage.back();
        storage.pop_back();
        numEntries--;
        return message;
    }
    else return "empty";
}

void SDHandler::initializeCard(){
    Serial.println("Initializing SD card...");
    if(!SD.begin(10)){
        Serial.println("Error initializing SD card");
    }
    Serial.println("Initialized SD successfully");
}