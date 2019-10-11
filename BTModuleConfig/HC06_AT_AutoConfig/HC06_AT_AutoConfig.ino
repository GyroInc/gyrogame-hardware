/*
 One Shot
 Kudos to marguskohv - he sowed the seed....
Serial monitor is just aide memoire
 */
#include <SoftwareSerial.h>
SoftwareSerial Serial1(5, 6); // RX | TX
String command = ""; // Stores response from HC-06

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);       //monitor
  Serial1.begin(57600);      //bluetooth

  Serial.print("AT      "); 
  Serial1.print("AT");                  //PING
  if (Serial1.available()) {
    while(Serial1.available()) { // While there is more to be read, keep reading.
    delay(3);
    char c = Serial1.read();
      command += c;   
    }
  }
  delay(2000);
  Serial.println(command);
  command = ""; // No repeats

  Serial.print("AT+NAMECube      ");
  Serial1.print("AT+NAMECube");        //CHANGE NAME
  if (Serial1.available()) {
    while(Serial1.available()) { // While there is more to be read, keep reading.
        delay(3);
      command += (char)Serial1.read(); 
    }
  }
  delay(2000);
  Serial.println(command);
  command = ""; // No repeats

  Serial.println("AT+PIN1337");
  Serial1.print("AT+PIN1337");        //CHANGE PASSWORD
  if (Serial1.available()) {
    while(Serial1.available()) { // While there is more to be read, keep reading.
        delay(3);
      command += (char)Serial1.read(); 
    }
  }
  delay(2000);   
  Serial.println(command);
  command = ""; // No repeats

  Serial.print("AT+BAUD7     "); 
  Serial1.print("AT+BAUD7");               //CHANGE SPEED TO 115K
  if (Serial1.available()) {
    while(Serial1.available()) { // While there is more to be read, keep reading.
      command += (char)Serial1.read();   
    }
  }
delay(2000);       
Serial.println(command);
}

void loop(){
}   //one-shot - nothing here
