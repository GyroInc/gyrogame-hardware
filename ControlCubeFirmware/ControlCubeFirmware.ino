//used libraries
#include <FastLED.h>
//#include <MPU6050_tockn.h>
//#include <Wire.h>
#include <SoftwareSerial.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif


//presets
#define LED_PIN     9
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define INTERRUPT_PIN 2             // Gyro interrupt pin

#define BAUDBT 38400                //baudrate BT module <-> Arduino
#define BAUDSE 38400                //baudrate debug hardware serial
#define DEFAULT_BRIGHTNESS 30       //0-255
#define GYRO_CALIBRATE false
#define GYRO_GRAVITY_THRESHOLD 0.75
#define GYRO_SEND_DELAY 80

#define DEBUG true                  //enable debug data to be sent to hardware serial
#define DEBUG_GYRO_DATA false       //print debug gyro data
#define GYRO_ENABLED true           //enable / disable gyro related initialization and loop functions

//variables
float x, y, z;
//temp color
uint8_t tr, tg, tb;
bool cubeConnected = false;
bool reverse = false;

//reserved for led animations
unsigned long eTimer[6];
int fTime[6];
unsigned long timer1;
unsigned long timer2;
unsigned long gyroTimer;
int pairingCounter;


//persistent objects
SoftwareSerial BT(5, 6);
//MPU6050 mpu6050(Wire, 0.1, 0.9);
CRGB leds[6];
CRGB tLeds[6];

void setup()
{
#if DEBUG
  Serial.begin(BAUDSE);
  Serial.println("Starting in debug mode");
#endif

  //initialize LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, 6).setCorrection( TypicalLEDStrip );
  SetLedBrightness(DEFAULT_BRIGHTNESS);
  LedOff();
#if DEBUG
  Serial.println("---LEDs initialized---");
#endif

  //initialize mpu
#if GYRO_ENABLED
#if DEBUG
  Serial.println("---Initializing Gyro...---");
#endif
  //  Wire.begin();
  //  mpu6050.begin();
  GyroInit();
#if DEBUG
  Serial.println("---Gyro initialized---");
#endif
#endif

  //initialize bluetooth
  BT.begin(BAUDBT);
#if DEBUG
  Serial.println("---Bluetooth initialized---");
#endif

  //battery Measure pin
  pinMode(A0, INPUT);
}

void loop() {

  //do fancy pairing animation
  if (!cubeConnected)
  {
    if (millis() > timer1 + 10)
    {
      timer1 = millis();
      if (!reverse)
      {
        if (pairingCounter >= 254) reverse = true;
        SetAllLed(pairingCounter++, 0, 0);
      }
      else
      {
        if (pairingCounter <= 1) reverse = false;
        SetAllLed(pairingCounter--, 0, 0);
      }
    }
  }

  //periodicly send battery voltages
  if (millis() > timer1 + 10000)
  {
    timer1 = millis();
    SendData("v" + String(analogRead(A0), DEC));
  }

  //handle command input
  ReceiveData();

  //update gyro data
#if GYRO_ENABLED
  GyroUpdate();
  if (cubeConnected)
  {
    if (millis() > gyroTimer + GYRO_SEND_DELAY)
    {
      gyroTimer = millis();
      GyroSendData();
    }
  }
#endif

  //LED fading routine
  Fade();

  //refresh LEDs every 33 seconds
  if (millis() > timer2 + 33)
  {
    timer2 = millis();
    FastLED.show();
  }
}
