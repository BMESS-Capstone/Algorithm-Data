#include <Arduino.h>
#include <math.h>
#include <BasicLinearAlgebra.h>
#include <../lib/BasicLinearAlgebra-master/impl/NotSoBasicLinearAlgebra.h>

// Change this when I know a real value
const int incidentIntensity = 750;

// These are the storage for the intensities and the intermediates
int currentIntensityArray[];
int lastIntensityArray[];
int oDCurrent[];
int oDLast[];
int oDChange[];

// Ouput array for the changes in concentration and intensity
int outputConc[50][2];

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

void setup()
{
    delay(1000);
    Serial.begin(9600);

    // Initialize the arrays
    int currentIntensityArray[] = {0, 0, 0, 0, 0, 0};
    int lastIntensityArray[] = {0, 0, 0, 0, 0, 0};
    int oDCurrent[] = {0, 0, 0, 0, 0, 0};
    int oDLast[] = {0, 0, 0, 0, 0, 0};
    int oDChange[] = {0, 0, 0, 0, 0, 0};
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
    //Adding to the Matrix of outputCon
    for(int i; i<50; i++){
        if(outputConc[i][0]==0){
            outputConc[i][0] = concHb;
            outputConc[i][1] = concHbO2;
        }
        else if(i==49 && outputConc[i][1]==0){
            //Signal an interupt and output!!
        }
    }
}
