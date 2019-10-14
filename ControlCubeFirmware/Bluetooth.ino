void ReceiveData()
{
  char inputBuffer[64];
  //clear data from inputbuffer to not receive the same commands again
  memset(inputBuffer, 0, sizeof(inputBuffer));
  int i = 0;

  if (BT.available() > 0)
  {
    while (BT.available() > 0)
    {
      inputBuffer[i++] = BT.read();
      if (i > 63) return;
    }
    //process
    ProcessData(inputBuffer);
  }
}

void ProcessData(char* data)
{
#if DEBUG
  Serial.print("Received: ");
  Serial.write(data);
#endif



  //confirm that its the controller for the game
  //cc stands for confirm cube
  if (data[0] == 'c' && data[1] == 'c')
  {
    //confim that its a valid control cube
    SendData("y");
    cubeConnected = true;
#if DEBUG
    Serial.println("---Bluetooth connected---");
#endif
  }

  //set color of a led
  //format is forced: l0r5g50b250
  if (data[0] == 'l')
  {
    //select led
    int l = (int)data[1] - 48;
    SetLedColor(l, atoi(strpbrk(data, "r") + 1), atoi(strpbrk(data, "g") + 1), atoi(strpbrk(data, "b") + 1));
  }

  //set color of all led
  //format is forced: ar5g50b250
  if (data[0] == 'a')
  {
    SetAllLed(atoi(strpbrk(data, "r") + 1), atoi(strpbrk(data, "g") + 1), atoi(strpbrk(data, "b") + 1));
  }

  //fade subcategory
  if (data[0] == 'f')
  {
    switch (data[1])
    {
      //fade in single led
      case 'i':
        //fi0r255g255b255t1000
        fLed = (int)data[2] - 48;
        leds[fLed].r = atoi(strpbrk(data, "r") + 1);
        leds[fLed].g = atoi(strpbrk(data, "g") + 1);
        leds[fLed].b = atoi(strpbrk(data, "b") + 1);
        fTime = atoi(strpbrk(data, "t") + 1);
        fi = true;
        break;
      //fade in all
      case 'a':
        //far255g255b255t1000
        tr = atoi(strpbrk(data, "r") + 1);
        tg = atoi(strpbrk(data, "g") + 1);
        tb = atoi(strpbrk(data, "b") + 1);
        fTime = atoi(strpbrk(data, "t") + 1);
        fia = true;
        break;
      //fade out single
      case 'o':
        //fo0t1000
        fLed = (int)data[2] - 48;
        fTime = atoi(strpbrk(data, "t") + 1);
        fo = true;
        break;
      //fade out all
      case 'p':
        //fbt1000
        fTime = atoi(strpbrk(data, "t") + 1);
        foa = true;
        break;
    }
  }

  //change brightness
  if (data[0] == 'b')
  {
    //b255
    SetLedBrightness(atoi(strpbrk(data, "b") + 1));
  }

  //+DISC:Success
  //computer disconnected
  if (data[0] == '+')
  {
    if (data[1] == 'D')
    {
#if DEBUG
      Serial.println("---Bluetooth disconnected---");
#endif
      cubeConnected = false;
      fo = false, foa = false;
      fi = false, fia = false;
      LedOff();
    }
  }
}

//send data to computer
void SendData(String data)
{
  BT.println(data);
#if DEBUG
  Serial.print("Sending to BT: ");
  Serial.println(data);
#endif
}
