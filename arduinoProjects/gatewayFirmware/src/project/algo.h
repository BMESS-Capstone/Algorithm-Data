#ifndef algo_h
#define algo_h

// Math libraries
#include <math.h>
#include <BasicLinearAlgebra.h>
#include <ctime>

class algo {
  public:
    algo();

    BluetoothSerial SerialBT;

    // Change to false ***
    bool initial = true;
    int StO2entry = 0;

    // Change this when I know a real value
    const float incidentIntensity = 750;

    // These are the storage for the intensities and the intermediates
    // Initialize the arrays
    float currentIntensityArray[6] = {0, 0, 0, 0, 0, 0};
    float lastIntensityArray[6] = {0, 0, 0, 0, 0, 0};
    float oDCurrent[6] = {0, 0, 0, 0, 0, 0};
    float oDLast[6] = {0, 0, 0, 0, 0, 0};
    float oDChange[6] = {0, 0, 0, 0, 0, 0};

    // Output array for the changes in concentration and intensity
    float outputStO2[20];

    // Wavelength constants in nanometers
    int HbWave = 760;
    int Hb02Wave = 860;

    // Channel that we care about
    // u is for Hb
    // w is for Hb02
    const float uChannel = 4;
    const float wChannel = 6;

    // Distance between source and detector
    const float optDist = 1.25;

    // Extinction coefficent matrix
    // These are estimates by interpolation to get us started ***
    // Units in (mM-1 * cm-1)
    // [0, 0] = extinction of Hb at lambda 1
    // [0, 1] = extinction of HbO2 at lambda 1
    // [1, 0] = extinction of Hb at lambda 2
    // [1, 1] = extinction of HbO2 at lambda 2
    const BLA::Matrix<2, 2> extCoeff = {0.38, 0.14, 0.18, 0.30};

    // Pathlength difference
    // We can assume it is a constant as it should not change reading that much
    const float pathLengthDelta = 1;

    // Differential pathlength factor
    // These are estimates which were interpolated from the research
    // To make these more accurate we would have to compute them
    // 4.38 corresponds to HbWave, and 3.94 corresponds to HbO2Wave
    const float DPF[2] = {4.38, 3.94};

    // The variable for the measurements of the concentration changes
    float concHb;
    float concHbO2;

    // Output counter, keeps track of when to signal the send function
    int queueCount = 0;

    void inputFilter();

    void calculateODdelta();

    BLA::Matrix<1, 2> calcConc();

    void updateOutput();

    void sendUpdate();

    void receiveUpdate();

    void currentToLast();

    String fullLoop();

    void readIntoIntArray();

    // Helper for the date and time
    String getDateAndTime();
};

#endif
