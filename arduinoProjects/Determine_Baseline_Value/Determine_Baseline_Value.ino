#include "AS726X.h"
AS726X sensor;

#define REJECT_COUNT 50
#define MEASUREMENTS 100

float mean_baseline_U = 0;
float mean_baseline_W = 0;
int counter = 0;

// NOTE: Make sure arm is completely relaxed

void setup() {
  Wire.begin(8, 7);
  Serial.begin(115200);

  while(Serial.available() == 0) {}
  Serial.read();

  sensor.begin();
  // Reject first few measurements to reach baseline
  for (int i = 0; i < REJECT_COUNT; i++) {
    sensor.takeMeasurementsWithBulb();
    Serial.println(sensor.getCalibratedU(), 2);
    Serial.println(sensor.getCalibratedW(), 2);
  }
  Serial.println();
}

void loop() {
  if (counter < MEASUREMENTS) {
    sensor.takeMeasurementsWithBulb();
    float u_wave = sensor.getCalibratedU();
    float w_wave = sensor.getCalibratedW();

    // Best to important Serial Plotter
    Serial.println(u_wave, 2);
    Serial.println(w_wave, 2);

    mean_baseline_U += u_wave;
    mean_baseline_W += w_wave;
    ++counter;
  } else {
    Serial.println();
    mean_baseline_U /= MEASUREMENTS;
    mean_baseline_W /= MEASUREMENTS;
    Serial.println("Baseline U:" + String(mean_baseline_U));
    Serial.println("Baseline W:" + String(mean_baseline_W));
    while(1);
  }
}
