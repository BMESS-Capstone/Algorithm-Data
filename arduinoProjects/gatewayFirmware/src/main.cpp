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

// Change to false ***
bool initial = true;

//********************Standard Code Block***************************

void setup()
{
    delay(1000);
    Serial.begin(115200);
    SerialBT.begin("ESP32test");
}

void loop()
{
    int StO2entry = 0;
    while (1)
    {
        // Now to write a script to deal with incoming

        // 1. Read first two inputs and chuck them off a cliff
        if (initial == true)
        {
            int discardCount = 0;
            while (discardCount < 2)
            {
                if (SerialBT.available())
                {
                    Serial.write(SerialBT.read());
                }
                int s = SerialBT.read();
                discardCount++;
            }
            initial = false;
        }

        // 2. First Reading needs to go to the current intensity,
        // and shift readings to the last

        receiveUpdate();
        currentToLast();

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
    char *s = (char *)Serial.read();
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