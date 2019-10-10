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
    #if DEBUG
        Serial.println();
    #endif
  }
}

void SendData()
{

}
