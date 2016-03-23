#if defined(__arc__)

#include "EducationShield.h"
#include "Arduino.h"
#include "CurieIMU.h"
#include "CurieTimerOne.h"

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf

long IMU::timer;
bool IMU::useISR=false;

int IMU::accRange,IMU::gyroRange;

int16_t IMU::ax,IMU::ay,IMU::az;
int16_t IMU::gx,IMU::gy,IMU::gz;

float IMU::pitch,IMU::roll;

IMU::IMU(){	
}

void staticRun(){
  IMU::measureMotion();
  IMU::calculateRollPitch();

}

void IMU::begin(int accRange, int gyroRange){
  //Serial.println("Initializing IMU device...");
  CurieIMU.begin();

  CurieIMU.setAccelerometerRange(accRange);
  CurieIMU.setGyroRange(gyroRange);

  IMU::accRange=accRange;
  IMU::gyroRange=gyroRange;

  if(useISR){
    CurieTimerOne.start(10,&staticRun);  
  }
}


void IMU::calibrate(){
  
  // verify connection
  Serial.println("Testing device connections...");
  if (CurieIMU.testConnection()) {
    Serial.println("CurieIMU connection successful");
  } else {
    Serial.println("CurieIMU connection failed");
  }

  // use the code below to calibrate accel/gyro offset values
  Serial.println("Internal sensor offsets BEFORE calibration...");
  Serial.print(CurieIMU.getXAccelOffset());
  Serial.print("\t"); // -76
  Serial.print(CurieIMU.getYAccelOffset());
  Serial.print("\t"); // -235
  Serial.print(CurieIMU.getZAccelOffset());
  Serial.print("\t"); // 168
  Serial.print(CurieIMU.getXGyroOffset());
  Serial.print("\t"); // 0
  Serial.print(CurieIMU.getYGyroOffset());
  Serial.print("\t"); // 0
  Serial.println(CurieIMU.getZGyroOffset());


  Serial.println("About to calibrate. Make sure your board is stable and upright");
  delay(5000);

  // The board must be resting in a horizontal position for
  // the following calibration procedure to work correctly!
  Serial.print("Starting Gyroscope calibration...");
  CurieIMU.autoCalibrateGyroOffset();
  Serial.println(" Done");
  Serial.print("Starting Acceleration calibration...");
  CurieIMU.autoCalibrateXAccelOffset(0);
  CurieIMU.autoCalibrateYAccelOffset(0);
  CurieIMU.autoCalibrateZAccelOffset(1);
  Serial.println(" Done");

  Serial.println("Internal sensor offsets AFTER calibration...");
  Serial.print(CurieIMU.getXAccelOffset());
  Serial.print("\t"); // -76
  Serial.print(CurieIMU.getYAccelOffset());
  Serial.print("\t"); // -2359
  Serial.print(CurieIMU.getZAccelOffset());
  Serial.print("\t"); // 1688
  Serial.print(CurieIMU.getXGyroOffset());
  Serial.print("\t"); // 0
  Serial.print(CurieIMU.getYGyroOffset());
  Serial.print("\t"); // 0
  Serial.println(CurieIMU.getZGyroOffset());

  Serial.println("Enabling Gyroscope/Acceleration offset compensation");
  CurieIMU.setGyroOffsetEnabled(true);
  CurieIMU.setAccelOffsetEnabled(true);

}

void IMU::detectShock(int shockThreashold, int shockDuration){
  /* Enable Shock Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_SHOCK, 192); // 1.5g
  CurieIMU.setDetectionDuration(CURIE_IMU_SHOCK, 11);   // 30ms
  CurieIMU.interrupts(CURIE_IMU_SHOCK);

  /* Enable Interrupts Notifications */
  //CurieIMU.setIntEnabled(true);
}
void IMU::attachCallback(void (*callback)(void)){
	  CurieIMU.attachInterrupt(callback);
}

void IMU::measureMotion(){
	// read raw accel/gyro measurements from device
	CurieIMU.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
}



void IMU::calculateRollPitch(){
	// Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
	// atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
	// It is then converted from radians to degrees
	/*#ifdef RESTRICT_PITCH // Eq. 25 and 26
	  roll  = atan2(ay, az) * RAD_TO_DEG;
	  pitch = atan(-ax / sqrt(ay * ay + az * az)) * RAD_TO_DEG;
	#else // Eq. 28 and 29
	  roll  = atan(ay / sqrt(ax * ax + az * az)) * RAD_TO_DEG;
	  pitch = atan2(-ax, az) * RAD_TO_DEG;
	#endif*/

  calculateComplementaryRollPitch();
}
void IMU::calculateComplementaryRollPitch(){
  /*long dt=millis()-timer;
  timer=millis();*/
  float dt=10;

  pitch=pitch-convertGyro_dps(gy)*(dt/1000.0);
  roll=roll+convertGyro_dps(gx)*(dt/1000.0);

  float pitchAcc=atan2(ax,az)*RAD_TO_DEG;
  float rollAcc=atan2(ay,az)*RAD_TO_DEG;

  pitch=pitch*0.98+pitchAcc*0.02;
  roll=roll*0.98+rollAcc*0.02;
}

void IMU::run(){
	if(useISR)return;
  
  measureMotion();
	calculateRollPitch();
}

int IMU::getPitch(){
	return (int)pitch;
}

int IMU::getRoll(){
	return (int)roll;
}

int IMU::getAccelerometerX(){
	return (int)ax;
}

int IMU::getAccelerometerY(){
	return (int)ay;
}

int IMU::getAccelerometerZ(){
	return (int)az;
}

int IMU::getGyroX(){
	return (int)gx;	
}

int IMU::getGyroY(){
	return (int)gy;	
}

int IMU::getGyroZ(){
	return (int)gz;	
}

float IMU::getAccelerometerX_g(){
  return convertAcclerometer_g(ax);
}

float IMU::getAccelerometerY_g(){
  return convertAcclerometer_g(ay);
}

float IMU::getAccelerometerZ_g(){
  return convertAcclerometer_g(az);
}

float IMU::getGyroX_dps(){
  return convertGyro_dps(gx);
}

float IMU::getGyroY_dps(){
  return convertGyro_dps(gy);
}

float IMU::getGyroZ_dps(){
  return convertGyro_dps(gz);
}

float IMU::convertAcclerometer_g(int16_t rawVal){
  return (float)rawVal*accRange/32768.0;
}
float IMU::convertGyro_dps(int16_t rawVal){
  return (float)rawVal*gyroRange/32768.0;

}


#endif