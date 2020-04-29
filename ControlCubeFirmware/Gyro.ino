
#define OLD_GYRO false

// ================================================================
// ===                MPU6050 LIBRARY CODE                      ===
// ================================================================
MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion quat;           // [w, x, y, z]         quaternion container


// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void GyroInit()
{
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
}


void GyroCalibrate()
{
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(4);
  mpu.setYGyroOffset(-42);
  mpu.setZGyroOffset(-23);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050
    //mpu.CalibrateAccel(6);
    //mpu.CalibrateGyro(6);
    //mpu.PrintActiveOffsets();
    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    digitalPinToInterrupt(INTERRUPT_PIN);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
  }
}

void GyroUpdate()
{
  // if programming failed, don't try to do anything
  if (!dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
    if (mpuInterrupt && fifoCount < packetSize) {
      // try to get out of the infinite loop
      fifoCount = mpu.getFIFOCount();
    }
  }

  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();
  if (fifoCount < packetSize) {
    //Lets go back and wait for another interrupt. We shouldn't be here, we got an interrupt from another event
    // This is blocking so don't do it   while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
  }
  // check for overflow (this should never happen unless our code is too inefficient)
  else if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    //  fifoCount = mpu.getFIFOCount();  // will be zero after reset no need to ask

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {

    // read a packet from FIFO
    while (fifoCount >= packetSize) { // Lets catch up to NOW, someone is using the dreaded delay()!
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      // track FIFO count here in case there is > 1 packet available
      // (this lets us immediately read more without waiting for an interrupt)
      fifoCount -= packetSize;
    }
  }

  // ================================================================
  // ===                END OF LIBRARY CODE                       ===
  // ================================================================


}


void GyroSendData()
{
  // display quaternion values in easy matrix form: w x y z
  mpu.dmpGetQuaternion(&quat, fifoBuffer);
  SendData("q" + String(quat.w) + "_" + String(quat.x) + "_" + String(quat.y) + "_" + String(quat.z));
}





















#if OLD_GYRO


void GyroUpdate()
{
  mpu6050.update();

  x = mpu6050.getAccX();
  y = mpu6050.getAccY();
  z = mpu6050.getAccZ();
}

void SendGyroData()
{
  if (millis() > gyroTimer + GYRO_SEND_DELAY)
  {
    gyroTimer = millis();
    Serial.print('g');
    Serial.print(mpu6050.getAngleX());
    Serial.print('_');
    Serial.print(mpu6050.getAngleY());
    Serial.print('_');
    Serial.println(mpu6050.getAngleZ());
    //Serial.println(FindFaceUp());
  }
}


void CalibrateGyro()
{
  mpu6050.calcGyroOffsets(false);
}

void SetGyroOffset(float x, float y, float z)
{
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

#endif
