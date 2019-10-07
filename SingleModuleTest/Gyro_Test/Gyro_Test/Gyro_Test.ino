
#include <MPU6050_tockn.h>
#include <Wire.h>

#include <FastLED.h>
#define LED_PIN     9
#define NUM_LEDS    6
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

MPU6050 mpu6050(Wire);
float y, p, r;
float ly, lp, lr;
float deltaX;
float deltaY;
float deltaZ;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop() {
  mpu6050.update();
  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.println(mpu6050.getAngleZ());

  ly = y;
  lp = p;
  lr = r;

  y = abs(mpu6050.getAngleZ());
  p = abs(mpu6050.getAngleY());
  r = abs(mpu6050.getAngleX());

  //calc size delta over time 
  deltaX = abs(r) - abs(lr);
  deltaY = abs(p) - abs(lp);
  deltaZ = abs(y) - abs(ly);


  //write value based color to cube
  for ( int i = 0; i < NUM_LEDS; i++) {
    // ...or as members of a three-element array:
    leds[i][0] = (int)abs(mpu6050.getAngleZ()) * 1.4;  // red
    leds[i][1] = (int)abs(mpu6050.getAngleY()) * 1.4; // green
    leds[i][2] = (int)abs(mpu6050.getAngleX()) * 1.4; // blue
  }

  FastLED.show();
}
