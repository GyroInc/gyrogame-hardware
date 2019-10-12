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
  }

  //set color of a led
  //format is forced: l0r5g50b250
  if (data[0] == 'l')
  {
    //select led
    int l = (int)data[1] - 48;
    SetLedColor(l, atoi(strpbrk(data, "r")+1), atoi(strpbrk(data, "g")+1), atoi(strpbrk(data, "b")+1));
  }

  //set color of all led
  //format is forced: ar5g50b250
  if (data[0] == 'a')
  {
    //select led
    for(int i = 0; i < 6; i++)
    {
      SetLedColor(i, atoi(strpbrk(data, "r")+1), atoi(strpbrk(data, "g")+1), atoi(strpbrk(data, "b")+1));
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
