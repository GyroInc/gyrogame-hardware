//used libraries
#include <FastLED.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <NeoSWSerial.h>

//presets
#define LED_PIN     9
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define DEBUG true
#define GYRO false
#define BAUDBT 38400
#define BAUDSE 38400
#define DEFAULT_BRIGHTNESS 64

//variables
float x, y, z;
//temp color
uint8_t tr, tg, tb;
bool cubeConnected = false;
bool reverse = false;

//reserved for led animations
unsigned long timer1;
unsigned long timer2;
int counter;
//runtime vars for effects
bool fo = false, foa = false;
bool fi = false, fia = false;
uint8_t fLed;
int fTime;

#if DEBUG
unsigned long previousMillis = 0;
#endif

//persistent objects
NeoSWSerial BT(5, 6);
MPU6050 mpu6050(Wire, 0.1, 0.6);
CRGB leds[6];

void setup() {
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
  //do fancy pairing animation
  if(!cubeConnected)
  {
    if(millis() > timer1 + 10)
    {
      timer1 = millis();
      if(!reverse)
      {
        if(counter >= 254) reverse = true;
        SetAllLed(0, 0, counter++);
      }
      else
      {
        if(counter <= 1) reverse = false;
        SetAllLed(0, 0, counter--);
      }
    }
  }
  
  //update gyro data
  GyroUpdate();
  //handle command input
  ReceiveData();

  //time dependent effect functions
  FadeOut();
  FadeOutAll();
  FadeIn();
  FadeInAll();

  FastLED.show();
}
