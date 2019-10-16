void GyroUpdate()
{
  mpu6050.update();

  x = mpu6050.getAccX();
  y = mpu6050.getAccY();
  z = mpu6050.getAccZ();

#if DEBUG_GYRO_DATA
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 100) {
    Serial.print("GyroData: ");
    Serial.print(mpu6050.getAngleX());
    Serial.print("_");
    Serial.print(mpu6050.getAngleY());
    Serial.print("_");
    Serial.println(mpu6050.getAngleZ());
    previousMillis = currentMillis;
  }
#endif
}

void SendGyroData()
{
  if(!cubeConnected) return;
  if(millis() > gyroTimer + GYRO_SEND_DELAY)
  {
    gyroTimer = millis();
    BT.print('g');
    BT.print(mpu6050.getAngleX());
    BT.print('_');
    BT.print(mpu6050.getAngleY());
    BT.print('_');
    BT.println(mpu6050.getAngleZ());
    //BT.println(FindFaceUp());
  }
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
  if (x > GYRO_GRAVITY_THRESHOLD)
    return 2;
  else if (x < -GYRO_GRAVITY_THRESHOLD)
    return 4;
  else if (y > GYRO_GRAVITY_THRESHOLD)
    return 3;
  else if (y < -GYRO_GRAVITY_THRESHOLD)
    return 1;
  else if (z > GYRO_GRAVITY_THRESHOLD)
    return 5;
  else if (z < -GYRO_GRAVITY_THRESHOLD)
    return 0;
}
