#include <iostream>

// Main Arduino
#include <Arduino.h>

// Bluetooth
#include "BluetoothSerial.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Algorithm
#include "algo.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

//Change to false ***
bool initial = true;

//****************************************************************

char *dummyIntensity[4] = {"100,124,142,643,121,543","100,124,142,543,121,643","100,124,142,843,121,443","100,124,142,743,121,543"};

//****************************************************************

//********************Standard Code Block***************************

void setup()
{
    delay(1000);
    Serial.begin(115200);
    SerialBT.begin("ESP32test");
}

void loop()
{
    while (1)
    {
        int StO2entry = 0;

        // Now to write a script to deal with incoming

        // 1. Read first two inputs and chuck them off a cliff
        #ifndef 0
        int chuckCount = 0;
        while (chuckCount < 2)
        {
            if (SerialBT.available())
            {
                Serial.write(SerialBT.read());
            }
            int s = SerialBT.read();
            chuckCount++;
        }
        #endif

        // 2. First Reading needs to go to the current intensity,
        // and shift readings to the last
        if (initial == false)
        {
            readIntoIntArray();
            currentToLast();
            initial = true;
        }

        // Temporary
        for(int i = 0; i < 6;i++){
            dummyIntensity[0][i] = lastIntensityArray[i];
            dummyIntensity[1][i] = currentIntensityArray[i];
        }

        // 3. Now you can start the normal loop
        // Preprocess
        inputFilter();
        std::cout << currentIntensityArray[0] << std::endl;

        // 4. Calculate the oDChange
        calculateODdelta();
        std::cout << oDChange[3] << " some space " << oDChange[5] << std::endl;

        // 5. Calculate the concentrations
        calcConc();

        // 6. Update the output values and send if needed
        updateOutput();
        std::cout << outputStO2[0] << std::endl;

        StO2entry++;
    }
}

// Helpers

void readIntoIntArray()
{
    int entryNum = 0;
    char *s = (char *)SerialBT.read();
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == ',')
        {
            i++;
        }
        currentIntensityArray[entryNum] = s[i];
        entryNum++;
    }
}

//*******************End of Code Block******************************
