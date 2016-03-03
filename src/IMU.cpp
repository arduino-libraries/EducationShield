#if defined(__arc__)

#include "EducationShield.h"
#include "Arduino.h"
#include "CurieImu.h"

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf

IMU::IMU(){	
}

void IMU::begin(){
  //Serial.println("Initializing IMU device...");
  CurieImu.initialize();

}

void IMU::calibrate(){
  // verify connection
  Serial.println("Testing device connections...");
  if (CurieImu.testConnection()) {
    Serial.println("CurieImu connection successful");
  } else {
    Serial.println("CurieImu connection failed");
  }

  // use the code below to calibrate accel/gyro offset values
  Serial.println("Internal sensor offsets BEFORE calibration...");
  Serial.print(CurieImu.getXAccelOffset());
  Serial.print("\t"); // -76
  Serial.print(CurieImu.getYAccelOffset());
  Serial.print("\t"); // -235
  Serial.print(CurieImu.getZAccelOffset());
  Serial.print("\t"); // 168
  Serial.print(CurieImu.getXGyroOffset());
  Serial.print("\t"); // 0
  Serial.print(CurieImu.getYGyroOffset());
  Serial.print("\t"); // 0
  Serial.println(CurieImu.getZGyroOffset());


  Serial.println("About to calibrate. Make sure your board is stable and upright");
  delay(5000);

  // The board must be resting in a horizontal position for
  // the following calibration procedure to work correctly!
  Serial.print("Starting Gyroscope calibration...");
  CurieImu.autoCalibrateGyroOffset();
  Serial.println(" Done");
  Serial.print("Starting Acceleration calibration...");
  CurieImu.autoCalibrateXAccelOffset(0);
  CurieImu.autoCalibrateYAccelOffset(0);
  CurieImu.autoCalibrateZAccelOffset(1);
  Serial.println(" Done");

  Serial.println("Internal sensor offsets AFTER calibration...");
  Serial.print(CurieImu.getXAccelOffset());
  Serial.print("\t"); // -76
  Serial.print(CurieImu.getYAccelOffset());
  Serial.print("\t"); // -2359
  Serial.print(CurieImu.getZAccelOffset());
  Serial.print("\t"); // 1688
  Serial.print(CurieImu.getXGyroOffset());
  Serial.print("\t"); // 0
  Serial.print(CurieImu.getYGyroOffset());
  Serial.print("\t"); // 0
  Serial.println(CurieImu.getZGyroOffset());

  Serial.println("Enabling Gyroscope/Acceleration offset compensation");
  CurieImu.setGyroOffsetEnabled(true);
  CurieImu.setAccelOffsetEnabled(true);

}

void IMU::detectShock(int shockThreashold, int shockDuration){
  /* Enable Shock Detection */
  CurieImu.setShockDetectionThreshold(192); // 1.5g
  CurieImu.setShockDetectionDuration(11);   // 30ms
  CurieImu.setIntShockEnabled(true);

  /* Enable Interrupts Notifications */
  CurieImu.setIntEnabled(true);
}
void IMU::attachCallback(void (*callback)(void)){
	  CurieImu.attachInterrupt(callback);
}

void IMU::measureMotion(){
	// read raw accel/gyro measurements from device
	CurieImu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
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