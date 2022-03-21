#ifndef algo_h
#define algo_h

// Math libraries
#include <math.h>
#include <BasicLinearAlgebra.h>
#include <ctime>

#include "/Users/jeffroszell/Documents/School/Capstone/Algorithm-Data/arduinoProjects/parameters.h"
// #include "C:\Users\elmal\Documents\GitHub\Algorithm-Data\arduinoProjects\parameters.h"

class algo {
  public:
    algo();

    // Change to false ***
    bool initial = true;
    int StO2entry = 0;

    // TODO: Change this when I know a real value (max value of the sensor)
    const float incidentIntensity = 750;

    // These are the storage for the intensities and the intermediates
    // Initialize the arrays
    float currentIntensityArray[SENSOR_DATA_LENGTH];
    float lastIntensityArray[SENSOR_DATA_LENGTH];
    float oDChange[SENSOR_DATA_LENGTH];

    // Output array for the changes in concentration and intensity
    float outputStO2[SENSOR_READINGS];
    float previousStO2Value = INITIAL_STO2_VALUE; //TODO: Talk to MacBeth to determine proper value

    // Wavelength constants in nanometers
    // TODO: Determine where these variables are used
    int HbWave = 760;
    int Hb02Wave = 860;

    // Channel that we care about
    // u is for Hb
    // w is for Hb02
    const int uChannel = U_CHANNEL;
    const int wChannel = W_CHANNEL;

    // Distance between the sensors
    const float optDist = 1;

    // Extinction coefficent matrix
    // These are estimates by interpolation to get us started ***
    // Units in (mM-1 * cm-1)
    // [0, 0] = extinction of Hb at lambda 1
    // [0, 1] = extinction of HbO2 at lambda 1
    // [1, 0] = extinction of Hb at lambda 2
    // [1, 1] = extinction of HbO2 at lambda 2
    const BLA::Matrix<2, 2> extCoeff = {0.38, 0.14, 0.18, 0.30};

    // Pathlength difference
    // We have this up to be 12.5mm
    const float sourceDetectorDistance = 0.0125;

    // Differential pathlength factor
    // These are estimates which were interpolated from the research
    // To make these more accurate we would have to compute them
    // 4.38 corresponds to HbWave, and 3.94 corresponds to HbO2Wave
    const float DPF[2] = {4.38, 3.94};

    // The variable for the measurements of the concentration changes
    float concHb;
    float concHbO2;

    void inputFilter();

    boolean calculateODdelta();

    void calcConc();

    void updateOutput();

    void receiveUpdate();

    void currentToLast();

    String fullLoop(int deviceLocation);

    // Helper for the date and time
    String getDateAndTime();

    int calcNewOxy();
};

#endif