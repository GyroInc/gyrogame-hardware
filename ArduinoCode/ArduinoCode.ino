//
//First add all libraries involved
//

//Smart led Lib:
#include <FastLED.h>

//Gyro Sensor libraries including I2C
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

//Software Serial for seamless bluetooth connection
#include <SoftwareSerial.h>

//
//create necessary objects and instances
//

//Soft serial
SoftwareSerial mySerial(5, 6); // RX, TX

//
//create dev command definitions
//

//relays serial input to bluetooth module for setup
#define RELAY_MODE false;

void setup() {

}

void loop() {

}
