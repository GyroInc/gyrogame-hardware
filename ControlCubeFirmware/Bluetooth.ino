void ReceiveData()
{
  String data;

  if (BT.available() > 0)
  {
    int counter = 0;
    while (BT.available() > 0)
    {
      data = BT.readString(); 

      #if DEBUG
        Serial.print(data);
      #endif
    }
  }
}

void SendData()
{

String data;

if (Serial.available() > 0)
{
  while (Serial.available() > 0)
    {
      data = Serial.readString(); 

      #if DEBUG
        BT.print(data);
      #endif
    }
  }
}
