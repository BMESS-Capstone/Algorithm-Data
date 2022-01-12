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

algo::algo(){ 

}

void algo::inputFilter()
{
    for (int i = 0; i < 6; i++)
    {
        if (currentIntensityArray[i] < 200)
            currentIntensityArray[i] = 0;
        if (currentIntensityArray[i] > 1000)
            currentIntensityArray[i] = 1000;
    }
}

void algo::calculateODdelta()
{
    // last Optical Density
    for (int i = 0; i < 6; i++)
    {
        if (lastIntensityArray[i] != 0)
        {
            oDLast[i] = log10(incidentIntensity / lastIntensityArray[i]);
        }
        else
            oDLast[i] = 0;
    }
    // current Optical Density
    for (int i = 0; i < 6; i++)
    {
        if (oDCurrent[i] != 0)
        {
            oDCurrent[i] = log10(incidentIntensity / currentIntensityArray[i]);
        }
        else
            oDCurrent[i] = 0;
    }
    // the change in optical density
    for (int i = 0; i < 6; i++)
    {
        oDChange[i] = oDCurrent[i] - oDLast[i];
    }
}

BLA::Matrix<1, 2> algo::calcConc()
{
    // get the constant
    double frontCoeff = 1 / (pathLengthDelta);

    // change the two OD results we care about into a matrix
    BLA::Matrix<2, 1> oDMatrix = {oDChange[HbWave], oDChange[Hb02Wave]};

    // compute the inverse matrix inputs
    BLA::Matrix<2, 2> inputMatrix = {
        (extCoeff(0,0) * DPF[0]),
        (extCoeff(0,1) * DPF[0]),
        (extCoeff(1,0) * DPF[1]),
        (extCoeff(1,1) * DPF[1])};

    // get matrix multiplication result
    BLA::Matrix<2, 1> result = BLA::Inverse(inputMatrix) * oDMatrix * frontCoeff;

    // assigning the first result to ConcHb
    concHb = result(0, 0);

    // assigning the second result to concHbO2
    concHbO2 = result(1, 0);
}

void algo::updateOutput()
{
    // adding new output entry
    if (queueCount < 20)
    {
        outputStO2[queueCount] = (concHbO2) / (concHb + concHbO2);
    }
    else if (queueCount >= 20)
    {
        queueCount = 0;
        outputStO2[queueCount] = (concHbO2) / (concHb + concHbO2);
    }

    queueCount++;
}

void algo::sendUpdate()
{
    // This is where we will write the 20 entries to the outgoing HTTP Post
}

void algo::readIntoIntArray()
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

void algo::receiveUpdate()
{
    // Checks for incoming info in the bluetooth pipe to be received
    if (SerialBT.available())
    {
        while (SerialBT.available())
        {
            Serial.write(SerialBT.read());
        }
        readIntoIntArray();
    }
}

void algo::currentToLast()
{
    for (int i = 0; i < 6; i++)
    {
        currentIntensityArray[i] = lastIntensityArray[i];
    }
}

String algo::fullLoop()
{
    String output = "";
    while (StO2entry < 20)
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

        // 4. Calculate the oDChange
        calculateODdelta();

        // 5. Calculate the concentrations
        calcConc();

        // 6. Update the output values and send if needed
        updateOutput();

        // 7. Concatenates the results into a single string
        output += ("%f, ", outputStO2[StO2entry]);

        // 8. Add timestamps to the output
        output += (getDateAndTime() + ", ");

        StO2entry++;
    }
    // Restarts count for next round
    StO2entry = 0;
    // Returns the String which is 20 readings
    return output;
}

// Helper for the date and time
String algo::getDateAndTime()
{
    time_t curr_time;
    tm *curr_tm;

    time(&curr_time);
    curr_tm = localtime(&curr_time);
    char time[100];

    strftime(time, 50, "%T", curr_tm);
    String toReturn = time;
    return toReturn;
}