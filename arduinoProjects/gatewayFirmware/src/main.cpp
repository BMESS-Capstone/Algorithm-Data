#include <Arduino.h>
#include <math.h>

// Change this when I know a real value
const int incidentIntensity = 750;

// These are the storage for the intensities and the intermediates
int currentIntensityArray[];
int lastIntensityArray[];
int oDCurrent[];
int oDLast[];
int oDChange[];

// Ouput array for the changes in concentration and intensity
int outputConc[];

// Wavelength constants in nanometers
const int hemoWave = 760;
const int deoxyHemoWave = 860;

// Distance between diodes in meters
const int optDist;

void setup()
{
    delay(1000);
    pinMode(11, INPUT);
    pinMode(12, OUTPUT);
    digitalRead(11);
    digitalWrite(12, HIGH);
    Serial.begin(9600);

    // Initialize the arrays
    int currentIntensityArray[] = {0, 0, 0, 0, 0, 0};
    int lastIntensityArray[] = {0, 0, 0, 0, 0, 0};
    int oDCurrent[] = {0, 0, 0, 0, 0, 0};
    int oDLast[] = {0, 0, 0, 0, 0, 0};
    int oDChange[] = {0, 0, 0, 0, 0, 0};
    int outputCon[50];
}

void loop()
{
    // put your main code here, to run repeatedly:
}

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
    //Last Optical Density
    for (int i = 0; i < 6; i++)
    {
        if (lastIntensityArray[i] != 0)
        {
            oDLast[i] = log10(incidentIntensity / lastIntensityArray[i]);
        }
        else
            oDLast[i] = 0;
    }
    //Current Optical Density
    for (int i = 0; i < 6; i++)
    {
        if (oDCurrent[i] != 0)
        {
            oDCurrent[i] = log10(incidentIntensity / currentIntensityArray[i]);
        }
        else
            oDCurrent[i] = 0;
    }
    //The change in optical density
    for (int i = 0; i < 6; i++)
    {
        oDChange[i] = oDCurrent[i] - oDLast[i];
    }

}


