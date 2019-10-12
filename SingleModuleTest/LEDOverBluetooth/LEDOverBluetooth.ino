//modified example code
/*
   Find the outgoing hc06 com port in the bluetooth settings
   connect to the port using putty
   tap 1 and 0 into the console to test leds
*/

#include <FastLED.h>

#define LED_PIN     9
#define NUM_LEDS    6
#define BRIGHTNESS  8
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#include <SoftwareSerial.h>// import the serial library

SoftwareSerial BT(5, 6); // RX, TX
int ledpin = 13; // led on D13 will show blink on / off
int BluetoothData; // the data given from Computer

void setup()
{
  // put your setup code here, to run once:
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  BT.begin(57600);
  BT.println("Bluetooth On please press 1 or 0 blink LED ..");
  pinMode(ledpin, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (BT.available())
  {
    BluetoothData = BT.read();
    CRGB color = CRGB::Black;
    
    switch (BluetoothData)
    {
      case '1':
      color = CRGB::White;
      break;
      case '2':
      color = CRGB::Red;
      break;
      case '3':
      color = CRGB::Green;
      break;
      case '4':
      color = CRGB::Blue;
      break;
    }

    for ( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = color;
      }
      FastLED.show();
    delay(100);// prepare for next data ...
  }
}
