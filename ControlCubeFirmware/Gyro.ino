void GyroUpdate()
{
  mpu6050.update();

  x = mpu6050.getAccX();
  y = mpu6050.getAccY();
  z = mpu6050.getAccZ();

#if GYRO
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 200) {
    Serial.print("GyroData: ");
    Serial.print(mpu6050.getAngleX());
    Serial.print("-");
    Serial.print(mpu6050.getAngleY());
    Serial.print("-");
    Serial.println(mpu6050.getAngleZ());
    previousMillis = currentMillis;
  }
#endif
}

void CalibrateGyro()
{
#if DEBUG
  Serial.println("Calculating gyro offsets");
  mpu6050.calcGyroOffsets(true);
  return;
#endif

  mpu6050.calcGyroOffsets(false);
}

void SetGyroOffset(float x, float y, float z)
{
#if DEBUG
  Serial.println("Setting Gyro offsets to:");
  Serial.print("x: ");
  Serial.println(x);
  Serial.print(" y: ");
  Serial.println(y);
  Serial.print(" z: ");
  Serial.println(z);
#endif
  mpu6050.setGyroOffsets(x, y, z);
}

int FindFaceUp()
{
  if (x > 0.5)
    return 3;
  else if (x < -0.5)
    return 5;
  else if (y > 0.5)
    return 4;
  else if (y < -0.5)
    return 2;
  else if (z > 0.5)
    return 6;
  else if (z < -0.5)
    return 0;
}
