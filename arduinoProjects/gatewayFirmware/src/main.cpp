#include <Arduino.h>

// Math libraries
#include <math.h>
#include <BasicLinearAlgebra.h>
#include <../lib/BasicLinearAlgebra-master/impl/NotSoBasicLinearAlgebra.h>

// Bluetooth
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// Input parser for csv types
#include <CSV_Parser.h>

CSV_Parser cp(csv_str, /*format*/ "sLdcfx-");

// Change this when I know a real value
const int incidentIntensity = 750;

// These are the storage for the intensities and the intermediates
int currentIntensityArray[];
int lastIntensityArray[];
int oDCurrent[];
int oDLast[];
int oDChange[];

// Output array for the changes in concentration and intensity
int outputStO2[50];

// Wavelength constants in nanometers
const int HbWave = 760;
const int Hb02Wave = 860;

// Channel that we care about
// u is for Hb
// w is for Hb02
const int uChannel = 4;
const int wChannel = 6;

// Distance between diodes in centimeters
const int optDist = 2;

// Extinction coefficent matrix
// These are estimates by interpolation to get us started ***
// Units in (mM-1 * cm-1)
// [0, 0] = extinction of Hb at lambda 1
// [0, 1] = extinction of HbO2 at lambda 1
// [1, 0] = extinction of Hb at lambda 2
// [1, 1] = extinction of HbO2 at lambda 2
const BLA::Matrix<2,2> extCoeff = {0.38,0.14,0.18,0.30};

// Pathlength difference
// We can assume it is a constant as it should not change reading that much
const double pathLengthDelta = 1;

// Differential pathlength factor
// Still need to figure this out
const double DPF = 1;

// The variable for the measurements of the concentration changes
float concHb;
float concHbO2;

// Output counter, keeps track of when to signal the send function
int queueCount = 0;

//********************Standard Code Block***************************

void setup()
{
    delay(1000);
    Serial.begin(115200);
    SerialBT.begin("ESP32test");

    // Initialize the arrays
    int currentIntensityArray[] = {0, 0, 0, 0, 0, 0};
    int lastIntensityArray[] = {0, 0, 0, 0, 0, 0};
    int oDCurrent[] = {0, 0, 0, 0, 0, 0};
    int oDLast[] = {0, 0, 0, 0, 0, 0};
    int oDChange[] = {0, 0, 0, 0, 0, 0};
}

void loop()
{
    // Checks for outgoing info in the bluetooth pipe to be sent
    if (Serial.available()) {
      SerialBT.write(Serial.read());
    }

    // Checks for incoming info in the bluetooth pipe to be received
    if (SerialBT.available()) {
      Serial.write(SerialBT.read());
    }

    // Now to write a script to deal with incoming

}

//*******************End of Code Block******************************

void initializeIntensities(){
    for (int i = 0; i < 6; i++)
    {
        lastIntensityArray[i] = Serial.parseInt();
    }
    for (int i = 0; i < 6; i++)
    {
        currentIntensityArray[i] = Serial.parseInt();
    }
}

void readIntensities()
{
    for (int i = 0; i < 6; i++)
    {
        lastIntensityArray[i] = currentIntensityArray[i];
        currentIntensityArray[i] = Serial.parseInt();
    }
    
}

void inputFilter()
{
    for (int i = 0; i < 6; i++)
    {
        if (currentIntensityArray[i] > 1000 || currentIntensityArray[i] < 200)
            currentIntensityArray[i] = 0;
    }
}

void calculateODdelta()
{
    //last Optical Density
    for (int i = 0; i < 6; i++)
    {
        if (lastIntensityArray[i] != 0)
        {
            oDLast[i] = log10(incidentIntensity / lastIntensityArray[i]);
        }
        else
            oDLast[i] = 0;
    }
    //turrent Optical Density
    for (int i = 0; i < 6; i++)
    {
        if (oDCurrent[i] != 0)
        {
            oDCurrent[i] = log10(incidentIntensity / currentIntensityArray[i]);
        }
        else
            oDCurrent[i] = 0;
    }
    //the change in optical density
    for (int i = 0; i < 6; i++)
    {
        oDChange[i] = oDCurrent[i] - oDLast[i];
    }
    
    
}

BLA::Matrix<1,2> calcExtinctionMatrix(){
    //get the constant
    double frontCoeff = 1/(DPF * pathLengthDelta);

    //change the two OD results we care about into a matrix
    BLA::Matrix<2,1> oDMatrix = {oDChange[HbWave],oDChange[Hb02Wave]};

    //get matrix multiplication result
    BLA::Matrix<2,1> result = BLA::Inverse(extCoeff) * oDMatrix * frontCoeff;

    //assigning the first result to ConcHb
    concHb = result(0,0);

    //assigning the second result to concHbO2
    concHbO2 = result(1,0);
}

void updateOutput(){
    //adding new output entry
    if(queueCount<50){
        outputStO2[queueCount] = (concHbO2)/(concHb + concHbO2);
    }
    elif(queueCount>=50){
        sendUpdate();
        queueCount = 0;
        outputStO2[queueCount] = (concHbO2)/(concHb + concHbO2);
    }

    queueCount++;

}

sendUpdate(){
    //write the implementation of the send function here.
}
