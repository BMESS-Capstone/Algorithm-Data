#ifndef PARAMETERS_H
#define PARAMETERS_H

// Parameters only for Sensor
#define BATTERY_INTERVAL_MS 2000
#define BATTERY_PIN A0
#define LED_PIN_RED A4 //verified
#define LED_PIN_GREEN D3 //verified
#define LED_PIN_BLUE D5 //verified
#define ON LOW
#define OFF HIGH
#define SDA_PIN 8 //verified with findI2C_PinsAndDevices.ino
#define SCL_PIN 7 //verified with findI2C_PinsAndDevices.ino

// Parameters only for Gateway
#define ONBOARD_LED 2
#define INITIAL_STO2_VALUE 0.0
#define SENSOR_READINGS 20

// Shared parameters
#define TOTAL_POSSIBLE_LOCATIONS 4
#define LEFT_ARM 0
#define RIGHT_ARM 1
#define LEFT_LEG 2
#define RIGHT_LEG 3

#define SENSOR_DATA_LENGTH 2
#define U_CHANNEL 0
#define W_CHANNEL 1

#define CONNECT_UUID "6164e702-7565-11ec-90d6-0242ac120003"

#define SENSOR_CHAR_UUID "fec40b26-757a-11ec-90d6-0242ac120003"
#define BATTERY_CHAR_UUID "fec40dc4-757a-11ec-90d6-0242ac120003"

#endif