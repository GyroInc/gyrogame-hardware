#include <MPU6050_tockn.h>
#include <Wire.h>
#include <SoftwareSerial.h>// import the serial library

SoftwareSerial BT(5, 6); // RX, TX
MPU6050 mpu6050(Wire, 0.1, 0.5);

void setup() {
  Wire.begin();  
  BT.begin(9600);
  mpu6050.begin();
  //mpu6050.calcGyroOffsets();
  mpu6050.setGyroOffsets(-1.83, 0.39, -1.13);
}

void loop() {
  mpu6050.update();
  BT.print(mpu6050.getAngleX());
  BT.print("\t");
  BT.print(mpu6050.getAngleY());
  BT.print("\t");
  BT.println(mpu6050.getAngleZ());
  delay(100);
}
