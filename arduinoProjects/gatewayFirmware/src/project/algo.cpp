// Main Arduino
#include <Arduino.h>
// Algorithm
#include "algo.h"

extern float sensorValue[SENSOR_DATA_LENGTH];
extern int batteryValue;
extern boolean isUpdated;
extern boolean switchSensor;
int tripcounter = 0;

algo::algo() {
  for (int i = 0; i < SENSOR_DATA_LENGTH; i++) {
    currentIntensityArray[i] = 0;
    lastIntensityArray[i] = 0;
    oDChange[i] = 0;
  }
}

void algo::inputFilter()
{
  for (int i = 0; i < SENSOR_DATA_LENGTH; i++)
  {
    //TODO:Needs to be edited
    if (currentIntensityArray[i] < 200)
      currentIntensityArray[i] = 1;
    if (currentIntensityArray[i] > 1000)
      currentIntensityArray[i] = 1000;
  }
}

boolean algo::calculateODdelta()
{
  // the change in optical density
  float oDLast, oDCurrent;
  for (int i = 0; i < SENSOR_DATA_LENGTH; i++)
  {
    // Last optical array
    if (lastIntensityArray[i] != 0)
      oDLast = log10(incidentIntensity / lastIntensityArray[i]);
    else
      oDLast = 0;

    // current Optical Density
    if (currentIntensityArray[i] != 0)
      oDCurrent = log10(incidentIntensity / currentIntensityArray[i]);
    else
      oDCurrent = 0;

    oDChange[i] = oDCurrent - oDLast;
  }

  if (oDChange[uChannel] == 0 && oDChange[wChannel] == 0)
    return false;

  return true;
}

BLA::Matrix<1, 2> algo::calcConc()
{
  // get the constant
  double frontCoeff = 1.0 / (sourceDetectorDistance);

  // change the two OD results we care about into a matrix
  BLA::Matrix<2, 1> oDMatrix = {oDChange[uChannel], oDChange[wChannel]};

  // compute the inverse matrix inputs
  BLA::Matrix<2, 2> inputMatrix = {
    (extCoeff(0, 0) * DPF[0]),
    (extCoeff(0, 1) * DPF[0]),
    (extCoeff(1, 0) * DPF[1]),
    (extCoeff(1, 1) * DPF[1])
  };

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
  outputStO2[StO2entry] = (concHbO2) / (concHb + concHbO2);
  previousStO2Value = outputStO2[StO2entry];
}

void algo::receiveUpdate()
{
  //Make sure sensor value is updated
  while (!isUpdated)
    delay(1);
  isUpdated = false;

  for (int i = 0; i < SENSOR_DATA_LENGTH ; i++)
    currentIntensityArray[i] = sensorValue[i];
}

void algo::currentToLast()
{
  for (int i = 0; i < SENSOR_DATA_LENGTH; i++)
    lastIntensityArray[i] = currentIntensityArray[i];
}

String algo::fullLoop(int deviceLocation, int& oxyValue)
{
  String output = "{";
  String tripID = "\"tripID\" : ";
  String location = " \"DeviceLocation\" : [";
  String values = " \"Values\" : [";
  String times = " \"Times\" : [";
  String battPerc = " \"BatteryPercentage\" : [";
  while (StO2entry < SENSOR_READINGS)
  {
    // 1. Read first two inputs and chuck them off a cliff
    if (initial == true)
    {
      receiveUpdate();
      initial = false;
      inputFilter();
      continue;
    }

    // 2. First Reading needs to go to the current intensity,
    // and shift readings to the last

    currentToLast();
    receiveUpdate(); // should pull one set of measurements

    // 3. Now you can start the normal loop
    // Preprocess
    inputFilter();

    // 4. Calculate the oDChange and returns true if there is a change
    if (calculateODdelta()) {
      // 5a. Calculate the concentrations
      calcConc();

      // 6. Update the output values and send if needed
      updateOutput();

      // 7. Concatenates the results into a single string
      values += String(outputStO2[StO2entry]) + ", ";
    } else
      values += String(previousStO2Value) + ", ";

    // 8. Add timestamps to the output (hh:mm:ss)
    times += (getDateAndTime() + ", ");

    StO2entry++;
  }
  // Makes the JSON string
  StO2entry = 0;
  output += location + String(deviceLocation) + ", ";
  output += "], ";
  output += tripID + String(tripcounter) + ", ";
  output += values;
  output += "], ";
  output += times;
  output += "], ";
  output += battPerc + String(batteryValue) + ", ";
  output += "]}";

  switchSensor = true;
  tripcounter++;

  //Update display value
  oxyValue = calcNewOxy();

  // Returns the String which is SENSOR_READINGS readings
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

int algo::calcNewOxy(){
    int sum = 0;
    for(int i=0;i<20;i++){
        sum+=outputStO2[i];
    }
    int average = sum/20;
    return int(average);
}