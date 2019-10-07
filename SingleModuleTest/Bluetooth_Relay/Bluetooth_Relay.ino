#include <SoftwareSerial.h>
// Programm HC-06
// load programm to Arduino
// no bluetooth connection to HC-06 Module
// send AT commands over Serial
// mit PUTTY auf Arduino COM verbinden
// kein NL und CR setzen
// AT -> OK
// AT+NAME<Name> -> Oksetname
// AT+PIN<4stellige zahl> -> OKsetPIN
// AT+BAUD<X> -> OK<X>
// X=4 : 9600bps (Default)
// X=6 : 38400bps
// X=7 : 57600bps
// X=8 : 115200bps
/////////////////////////
// HC-06 | Arduino
// TX    | D10
// RX    | D11
////////////////////////
SoftwareSerial mySerial(5, 6);

void setup() {

  Serial.begin(57600);
  delay(500);
   mySerial.begin(9600);
// mySerial.begin(57600);
delay(500);
  Serial.println("Ready");
}

void loop()

{
  if ( Serial.available() )
  {
    char a = Serial.read();
    mySerial.print(a);
  }
  if ( mySerial.available() )
  {
    char a = mySerial.read();
    Serial.print(a);
  }
}
