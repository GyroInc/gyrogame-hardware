#include <FastLED.h>

#define LED_PIN     9
#define NUM_LEDS    6
#define BRIGHTNESS  128
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);

float x, y, z;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  mpu6050.begin();

  Serial.begin(115200);
}

void loop() {
  mpu6050.update();
  x = mpu6050.getAccX();
  y = mpu6050.getAccY();
  z = mpu6050.getAccZ();

  //Serial.print("X : "); Serial.print(mpu6050.getAccX());
  //Serial.print("\tY : "); Serial.print(mpu6050.getAccY());
  //Serial.print("\tZ : "); Serial.println(mpu6050.getAccZ());

  //x axis
  if (x > 0.5)
  {
    clearLeds();
    leds[2].setRGB(255, 115, 0);
    leds[0].setRGB(0, 117, 113);
    leds[3].setRGB(0, 117, 113);
    leds[5].setRGB(0, 117, 113);
    leds[1].setRGB(0, 117, 113);
  }
  else if (x < -0.5)
  {
    clearLeds();
    leds[4].setRGB(255, 115, 0);
    leds[0].setRGB(0, 117, 113);
    leds[3].setRGB(0, 117, 113);
    leds[5].setRGB(0, 117, 113);
    leds[1].setRGB(0, 117, 113);
  }

  //y axis
  if (y > 0.5)
  {
    clearLeds();
    leds[3].setRGB(255, 115, 0);
    leds[0].setRGB(0, 117, 113);
    leds[2].setRGB(0, 117, 113);
    leds[5].setRGB(0, 117, 113);
    leds[4].setRGB(0, 117, 113);
  }
  else if (y < -0.5)
  {
    clearLeds();
    leds[1].setRGB(255, 115, 0);
    leds[0].setRGB(0, 117, 113);
    leds[2].setRGB(0, 117, 113);
    leds[5].setRGB(0, 117, 113);
    leds[4].setRGB(0, 117, 113);
  }

  //z axis
  if (z > 0.5)
  {
    clearLeds();
    leds[5].setRGB(255, 115, 0);
    leds[3].setRGB(0, 117, 113);
    leds[2].setRGB(0, 117, 113);
    leds[1].setRGB(0, 117, 113);
    leds[4].setRGB(0, 117, 113);
  }
  else if (z < -0.5)
  {
    clearLeds();
    leds[0].setRGB(255, 115, 0);
    leds[3].setRGB(0, 117, 113);
    leds[2].setRGB(0, 117, 113);
    leds[1].setRGB(0, 117, 113);
    leds[4].setRGB(0, 117, 113);
  }

  FastLED.show();
  delay(50);
}

void clearLeds()
{
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}
