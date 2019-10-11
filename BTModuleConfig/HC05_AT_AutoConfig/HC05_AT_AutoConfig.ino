#include <SoftwareSerial.h>
SoftwareSerial BTSerial(5, 6); // RX | TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);       //monitor
  BTSerial.begin(38400);      //bluetooth module default AT baudrate

  sendATCommand("AT");    //PING
  delay(2000);
  sendATCommand("AT+NAME=Cube"); //CHANGE BT NAME
  delay(2000);
  sendATCommand("AT+PSWD=\"1337\""); // CHANGE PASSWORD
  delay(2000);
  sendATCommand("AT+UART=57600,0,0"); // SET BAUD TO 57600
}

void sendATCommand(String command) {
  Serial.println(command);
  BTSerial.println(command);
  if (BTSerial.available()) {
    while (BTSerial.available()) { // While there is more to be read, keep reading.
      delay(3);
      Serial.write((char)BTSerial.read());
    }
  }
}

void loop() {
}   //one-shot - nothing here
