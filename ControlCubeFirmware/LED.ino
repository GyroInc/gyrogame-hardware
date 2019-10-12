void SetLedBrightnes(uint8_t bright)
{
  FastLED.setBrightness(  bright );

#if DEBUG
  Serial.print("Set Brightnes to: ");
  Serial.println(bright);
#endif
}

void LedOff()
{
  for ( int i = 0; i < NUM_LEDS; i++) {
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
