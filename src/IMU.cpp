#if defined(__arc__)

#include "EducationShield.h"
#include "Arduino.h"
#include "CurieIMU.h"

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf

IMU::IMU(){	
}

void IMU::begin(){
  //Serial.println("Initializing IMU device...");
  CurieIMU.begin();

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
	#ifdef RESTRICT_PITCH // Eq. 25 and 26
	  roll  = atan2(ay, az) * RAD_TO_DEG;
	  pitch = atan(-ax / sqrt(ay * ay + az * az)) * RAD_TO_DEG;
	#else // Eq. 28 and 29
	  roll  = atan(ay / sqrt(ax * ax + az * az)) * RAD_TO_DEG;
	  pitch = atan2(-ax, az) * RAD_TO_DEG;
	#endif
}

void IMU::run(bool useRollPitch){
	measureMotion();

	if(!useRollPitch)return;

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

#endif