#define TERM_CHAR '\n'
#define BUFFER_SIZE 64

void ReceiveData()
{
  char inputBuffer[BUFFER_SIZE];
  //clear data from inputbuffer to not receive the same commands again
  memset(inputBuffer, 0, sizeof(inputBuffer));

  int numberOfBytes = 0;

  if (BT.available())
  {
    numberOfBytes = BT.readBytesUntil(TERM_CHAR, inputBuffer, BUFFER_SIZE);
  }
  if (numberOfBytes > 0)
  {
    //process
    ProcessData(inputBuffer);
  }
}

void ProcessData(char* data)
{
#if DEBUG
  Serial.print("Received: ");
  Serial.println(data);
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
    fTime[l] = 0; //cancel any ongoing animation
    SetLedColor(l, atoi(strpbrk(data, "r") + 1), atoi(strpbrk(data, "g") + 1), atoi(strpbrk(data, "b") + 1));
  }

  //set color of all led
  //format is forced: ar5g50b250
  if (data[0] == 'a')
  {
    for (int i = 0; i < 6; i++)
    {
      fTime[i] = 0; //cancel any ongoing animation
    }
    SetAllLed(atoi(strpbrk(data, "r") + 1), atoi(strpbrk(data, "g") + 1), atoi(strpbrk(data, "b") + 1));
  }

  //fade subcategory
  if (data[0] == 'f')
  {
    if (data[1] == 'o')
    {
      //fade in single
      //fo0r255g255b255t1000
      int led = (int)data[2] - 48;
      fTime[led] = atoi(strpbrk(data, "t") + 1);
      tLeds[led].r = atoi(strpbrk(data, "r") + 1);
      tLeds[led].g = atoi(strpbrk(data, "g") + 1);
      tLeds[led].b = atoi(strpbrk(data, "b") + 1);
    }
    else if (data[1] == 'a')
    {
      //fade in all
      //far255g255b255t1000
      for (int i = 0; i < 6; i++)
      {
        fTime[i] = atoi(strpbrk(data, "t") + 1);
        tLeds[i].r = atoi(strpbrk(data, "r") + 1);
        tLeds[i].g = atoi(strpbrk(data, "g") + 1);
        tLeds[i].b = atoi(strpbrk(data, "b") + 1);
      }

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
