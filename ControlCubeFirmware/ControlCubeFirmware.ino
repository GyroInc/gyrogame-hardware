//used libraries
#include <FastLED.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <SoftwareSerial.h>

//presets
#define LED_PIN     9
#define NUM_LEDS    6
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define BAUDBT 57600
#define BAUDSE 9600

//variables
float x, y, z;

#define DEBUG true
#define GYRO false

#if DEBUG
unsigned long previousMillis = 0;
#endif

//persistent objects
SoftwareSerial BT(5, 6);
MPU6050 mpu6050(Wire, 0.1, 0.6);
CRGB leds[NUM_LEDS];

void setup() {
#if DEBUG
  Serial.begin(BAUDSE);
  Serial.println("Starting in debug mode");
#endif

  //initialize LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  SetLedBrightnes(128);
  LedOff();
#if DEBUG
  Serial.println("---LEDs initialized---");
#endif

  //initialize mpu
#if DEBUG
  Serial.println("Wire begin");
#endif
  Wire.begin();
  mpu6050.begin();
  mpu6050.setGyroOffsets(-1.83, 0.39, -1.13);
#if DEBUG
  Serial.println("---Gyro initialized---");
#endif

  //initialize bluetooth
  BT.begin(BAUDBT);
#if DEBUG
  Serial.println("---Bluetooth initialized---");
#endif
}

void loop() {
  //update gyro data
  GyroUpdate();

  ReceiveData();
  SendData();
}
