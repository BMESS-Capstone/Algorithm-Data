#ifndef jsonCreator_h
#define jsonCreator_h

#include <Arduino.h>
#include <ArduinoJson.h>

class jsonCreator{
    private:
    

    public:
    jsonCreator();
    void startUp();
    String createJSON(String measurements, String time);
    String createErrJSON();
};

#endif