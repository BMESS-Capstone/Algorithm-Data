/*
  pIRfusiX sensor BLE firmware

  Service:
  - sensorService (service encapsulating various characteristics)

  Characterstics for sensorService:
  - sensorChar (sensor value output)
  - batteryChar (battery voltage remaining)

  The circuit:
  - Arduino Nano 33 BLE or custom pIRfusiX PCB board
  - Sparkfun AS7265x Spectral Triade (SCL/SDA Conncetion)

  Pre-requisite:
  - Need to edit add the following function in Wire.cpp and respective definition in Wire.h
        void arduino::MbedI2C::begin(int sda, int scl) {
          _sda = digitalPinToPinName(sda);
          _scl = digitalPinToPinName(scl);
          master = new mbed::I2C(_sda, _scl);
        }

  Interacts with the ESP32 BLE client firmware

  Author: Khaled Elmalawany
*/

#include <ArduinoBLE.h>
#include "C:\Users\randy\Documents\GitHub\Algorithm-Data\arduinoProjects\parameters.h"
//#include "/home/pi/Documents/Algorithm-Data/arduinoProjects/parameters.h"

#include "AS726X.h"
AS726X sensor;
float counter = 0.0;

int location = RIGHT_ARM;

// BLE Service (NOTE: Consider moving into setup() to reduce dynamic memory)
BLEService sensorService(CONNECT_UUID);

// BLE Sensor Data Characteristic
BLECharacteristic sensorChar(SENSOR_CHAR_UUID,  // standard 16-bit characteristic UUID
                             BLERead | BLENotify,  // remote clients will be able to get notifications if this characteristic changes
                             sizeof(float)*SENSOR_DATA_LENGTH);


// BLE Battery Level Characteristic
BLEIntCharacteristic batteryChar(BATTERY_CHAR_UUID,  // standard 16-bit characteristic UUID
                                 BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

//Sensor global variables
float sensorValue[SENSOR_DATA_LENGTH];

// Battery global variables
int oldBatteryLevel = 0;
long previousMillis = 0;

void setup() {
  delay(1000);
  
  // initialize the built-in LED pin to indicate when a central is connected
  // Note: 4-pin RGB is common cathode
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  digitalWrite(LED_PIN_RED, OFF);
  digitalWrite(LED_PIN_GREEN, OFF);
  digitalWrite(LED_PIN_BLUE, OFF);

  // begin initialization
  if (!BLE.begin()) {
    digitalWrite(LED_PIN_BLUE, ON);
    while (1);
  }

  Wire.begin(SDA_PIN, SCL_PIN);
  if (sensor.begin() == false)
  {
    digitalWrite(LED_PIN_RED, ON);
    while (1);
  }

  // Note: Mode 2 - Continuous reading of all channels is the default
  sensor.setMeasurementMode(2);

  //0-255 valid but 0 seems to cause the sensors to read very slowly
  //Using 1: (1+1)*2.8ms = 5.6ms per reading
  //But we need two integration cycles so 89Hz is aproximately the fastest read rate
  sensor.setIntegrationTime(1);

  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("pIRfusiX sensor");
  BLE.setAdvertisedService(sensorService); // add the service UUID
  sensorService.addCharacteristic(sensorChar); // add the sensor characteristic
  sensorService.addCharacteristic(batteryChar); // add the battery characteristic
  BLE.addService(sensorService); // Add the connection service
  batteryChar.writeValue(location);

  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();
}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    // turn on the LED to indicate the connection:
    digitalWrite(LED_PIN_GREEN, ON);
    previousMillis = millis();

    // while the central is connected:
    while (central.connected()) {
      //**********IR Sensor*****************
      if (sensorChar.subscribed()) {
        readSensor();
        sensorChar.writeValue(sensorValue, sizeof(float)*SENSOR_DATA_LENGTH);
      }

      //**********BATERY*****************
      // if some interval time has passed, check the battery level:
      if (batteryChar.subscribed() && millis() - previousMillis > BATTERY_INTERVAL_MS) {
        previousMillis = millis();
        updateBatteryLevel();
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_PIN_GREEN, OFF);

    // Reset the location
    batteryChar.writeValue(location);
  }
}

void updateBatteryLevel() {
  /* Read the current voltage level on the battery's analog input pin.
     This is used here to simulate the charge level of a battery.
  */
  int battery = analogRead(BATTERY_PIN);
  int batteryLevel = map(battery, 0, 1023, 0, 100);

  batteryChar.writeValue(batteryLevel);  // and update the battery level characteristic
}

void readSensor() {
  // Will turn on Bulb then turn it off to prevent overheating
  // Will make sure that data is available
  sensor.takeMeasurementsWithBulb();

  sensorValue[0] = sensor.getCalibratedU();
  sensorValue[1] = sensor.getCalibratedW();
}
