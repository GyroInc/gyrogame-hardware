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
  for(int i = 0; i < 6; i++)
  {
    leds[i].setRGB(r, g, b);
  }
}

//needs to be called in loop
void FadeOut()
{
  if(!fo) return;
  int timeStep = (int)fTime / 255;
  if(millis() > timer1 + timeStep)
  {
    timer1 = millis();
    SetLedColor(fLed, 
    (leds[fLed].r > 0 ? leds[fLed].r-=1: 0), 
    (leds[fLed].g > 0 ? leds[fLed].g-=1: 0), 
    (leds[fLed].b > 0 ? leds[fLed].b-=1: 0));

    if(leds[fLed].r == 0 && leds[fLed].g == 0 && leds[fLed].b == 0) fo = false;
  }  
}

//needs to be called in loop
void FadeOutAll()
{
  if(!foa) return;
  int timeStep = fTime / 255;
  bool alloff = false;
  if(millis() > timer1 + timeStep)
  {
    timer1 = millis();
    for(int l = 0; l < 6; l++)
    {
      SetLedColor(l,
      (leds[l].r > 0 ? leds[l].r-=1: 0), 
      (leds[l].g > 0 ? leds[l].g-=1: 0), 
      (leds[l].b > 0 ? leds[l].b-=1: 0));

      if(leds[l].r <= 0 && leds[l].g <= 0 && leds[l].b <= 0) alloff = true;
    }
  }
  if(alloff) foa = false;
}

//needs to be called in loop
void FadeIn()
{
  if(!fi) return;
  int timeStep = (int)fTime / 255;
  bool finished = false;
  if(millis() > timer2 + timeStep)
  {
    timer2 = millis();
    SetLedColor(fLed, 
    (tr < leds[fLed].r ? tr+=1: leds[fLed].r), 
    (tg < leds[fLed].g ? tg+=1: leds[fLed].g), 
    (tb < leds[fLed].b ? tb+=1: leds[fLed].b));

    if(tr == leds[fLed].r || tg == leds[fLed].g || tb == leds[fLed].b) finished = true;
  }  

  if(finished)
  {
    tr = 0; tg = 0; tb = 0;
    fi = false;
  }
}

//needs to be called in loop
void FadeInAll()
{
  if(!fia) return;
  int timeStep = (int)fTime / 255;
  bool finished = true;
  if(millis() > timer1 + timeStep)
  {
    timer1 = millis();
    for(int l = 0; l < 6; l++)
    {
      SetLedColor(l, 
      (leds[l].r < tr ? leds[l].r+=1: tr), 
      (leds[l].g < tg ? leds[l].g+=1: tg), 
      (leds[l].b < tb ? leds[l].b+=1: tb));

      if(leds[l].r < tr || leds[l].g < tg || leds[l].b < tb) finished = false;
    }    
    if(finished)
    {
      tr = 0; tg = 0; tb = 0;
      fia = false;
    }
  }
}
