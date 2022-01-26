// Main Arduino
#include <Arduino.h>

// Algorithm
#include "algo.h"

extern float sensorValue[SENSOR_DATA_LENGTH];
extern int batteryValue; // TODO: implement battery data into JSON file (if time permits)
extern boolean isUpdated;

algo::algo(){
  for(int i = 0; i < SENSOR_DATA_LENGTH; i++) {
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

void algo::calculateODdelta()
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
}

BLA::Matrix<1, 2> algo::calcConc()
{
  // get the constant
  double frontCoeff = 1.0 / (pathLengthDelta);

  // change the two OD results we care about into a matrix
  BLA::Matrix<2, 1> oDMatrix = {oDChange[HbWave], oDChange[Hb02Wave]};

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

void algo::receiveUpdate()
{
  //Make sure sensor value is updated
  while(!isUpdated)
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

String algo::fullLoop()
{
  String output = "{";
  String values = " \"Values\" : [";
  String times = " \"Times\" : [";
  while (StO2entry < 20)
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

    // 4. Calculate the oDChange
    calculateODdelta();

    // 5. Calculate the concentrations
    calcConc();

    // 6. Update the output values and send if needed
    updateOutput();

    // 7. Concatenates the results into a single string
    values += ("\"%f\", ", outputStO2[StO2entry]);

    // 8. Add timestamps to the output (hh:mm:ss)
    times += (getDateAndTime() + ", ");

    StO2entry++;
  }
  // Makes the JSON string
  StO2entry = 0;
  output += values;
  output += "], ";
  output += times;
  output += "]}";
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
