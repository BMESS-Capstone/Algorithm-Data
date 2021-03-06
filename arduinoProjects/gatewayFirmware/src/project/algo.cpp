// Main Arduino
#include <Arduino.h>
// Algorithm
#include "algo.h"

extern float sensorValue[SENSOR_DATA_LENGTH];
extern int batteryValue;
extern boolean isUpdated;
extern boolean switchSensor;
extern float oxyValue;
extern int tripcounter;
String hcn = "55555555";

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

void algo::calcConc()
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

  for (int i = 0; i < SENSOR_DATA_LENGTH ; i++) {
    currentIntensityArray[i] = sensorValue[i];
  }
}

void algo::currentToLast()
{
  for (int i = 0; i < SENSOR_DATA_LENGTH; i++)
    lastIntensityArray[i] = currentIntensityArray[i];
}

String algo::fullLoop(int deviceLocation, String rtcTime)
{
  String output = "";
  String values = "";
  String times = "";

  while (StO2entry < SENSOR_READINGS)
  {
    // 1. Read first two inputs and chuck them off a cliff
    if (initial == true)
    {
      receiveUpdate();
      initial = false;
      //      inputFilter();
      continue;
    }

    // 2. First Reading needs to go to the current intensity,
    // and shift readings to the last
    currentToLast();
    receiveUpdate(); // should pull one set of measurements

    // 3. Now you can start the normal loop
    // Preprocess
    //    inputFilter();

    // 4. Calculate the oDChange and returns true if there is a change
    if (calculateODdelta()) {
      // 5a. Calculate the concentrations
      calcConc();

      // 6. Update the output values and send if needed
      updateOutput();

      // 7. Concatenates the results into a single string
      values += String(outputStO2[StO2entry]) + ",";
    } else {
      values += String(previousStO2Value) + ",";

    }

    // 8. Add timestamps to the output (hh:mm:ss)
    times += (rtcTime + ", ");

    StO2entry++;
  }
  // Makes the JSON string
  StO2entry = 0;

  // get rid of end commas
  values.remove(values.length() - 1);
  times.remove(times.length() - 1);

  output += String(tripcounter);
  output += ";";
  output += hcn;
  output += ";[";
  output += values;
  output += "];[";
  output += times;
  output += "]";


  switchSensor = true;
  // TODO: implement the end of trip if there is time
  // tripcounter++;

  //Update display value
  oxyValue = calcNewOxy();

  // Returns the String which is SENSOR_READINGS readings
  return output;
}

// Developed from: https://github.com/KaunilD/mes2hb/blob/master/mes2hb/mes2hb.py
float algo::convert() {
  // HbWave = 760 & Hb02Wave = 860
  // U_Wave = 35.74+32.90+33.48+30.06+36.65+30.61+29.95 & W_Wave = 39.05+36.46+34.43+41.05+40.53+35.13+31.34
  float U_Wave_baseline = 29.95;
  float W_Wave_baseline = 31.34;
}

// Developed from: https://github.com/KaunilD/mes2hb/blob/a4406ec444b51612392f62c2933a53a41b0cf07f/mes2hb/absorption_coefficients.py
float algo::get_coefficient(int wav, String hb_type) {
  int l_wav = floor(wav);
  int u_wav = l_wav + 1;
  float l_coef, u_coef;

  if (hb_type == "oxy") {
    if (wav == HbWave) {
      l_coef = 0.1495594;
      u_coef = 0.1504855;
    } else {
      l_coef = 0.2611131;
      u_coef = 0.2620623;
    }
  } else if (hb_type == "dxy") {
    if (wav == HbWave) {
      l_coef = 0.3865221;
      u_coef = 0.3849846;
    } else if (wav == Hb02Wave) {
      l_coef = 0.1823647;
      u_coef = 0.1827244;
    }
  }

  return (u_coef - l_coef) / (u_wav - l_wav) * (wav - l_wav) + l_coef;
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

float algo::calcNewOxy() {
  float sum = 0;
  for (int i = 0; i < SENSOR_READINGS; i++) {
    sum += outputStO2[i];
  }
  float average = sum / float(SENSOR_READINGS);
  return average;
}
