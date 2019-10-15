void SetLedBrightness(uint8_t bright)
{
  FastLED.setBrightness(  bright );

#if DEBUG
  Serial.print("Set Brightnes to: ");
  Serial.println(bright);
#endif
}

void LedOff()
{
  for ( int i = 0; i < 6; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();

#if DEBUG
  Serial.println("Leds off");
#endif
}

void SetLedColor(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
  leds[led].setRGB(r, g, b);
}

void SetAllLed(uint8_t r, uint8_t g, uint8_t b)
{
  for (int i = 0; i < 6; i++)
  {
    leds[i].setRGB(r, g, b);
  }
}

//needs to be called in loop
void Fade()
{
  for (int i = 0; i < 6; i++)
  {
    if (fTime[i] != 0)
    {
      if(millis() > eTimer[i] + (fTime[i] / 255))
      {
        if(tLeds[i].r > leds[i].r) leds[i].r += 1;
        else if(tLeds[i].r < leds[i].r) leds[i].r -= 1;
        if(tLeds[i].g > leds[i].g) leds[i].g += 1;
        else if(tLeds[i].g < leds[i].g) leds[i].g -= 1;
        if(tLeds[i].b > leds[i].b) leds[i].b += 1;
        else if(tLeds[i].b < leds[i].b) leds[i].b -= 1;
        
        SetLedColor(i, leds[i].r, leds[i].g, leds[i].b);

        if(tLeds[i].r == leds[i].r && tLeds[i].g == leds[i].g && tLeds[i].b == leds[i].b) fTime[i] = 0;
      }
    }
  }
}
