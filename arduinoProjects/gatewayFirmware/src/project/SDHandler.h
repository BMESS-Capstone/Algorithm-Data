//
// Created by Jeff Roszell on 2022-03-01.
//

#ifndef ARDUINOPROJECTS_SDHANDLER_H
#define ARDUINOPROJECTS_SDHANDLER_H

#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include <vector>



class SDHandler {
private:
    int numEntries;
    //This is wrong because you want to write to file on SD
    //std::vector<String> storage;
public:
    SDHandler();
    void store(String message);
    String retrieve();
    void initializeCard();
};


#endif //ARDUINOPROJECTS_SDHANDLER_H
