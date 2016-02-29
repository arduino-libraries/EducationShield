#include "EducationShield.h"
#include "Arduino.h"
#include "CurieImu.h"

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf

IMU::IMU(){	
}

void IMU::begin(){
	  // Curie IMU setup
  //--------------------------------------------------
  // initialize device
  Serial.println("Initializing IMU device...");
  CurieImu.initialize();

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

  // configure Arduino LED for activity indicator
  //pinMode(ledPin, OUTPUT);

  // read raw accel/gyro measurements from device
  CurieImu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Curie IMU setup end
  //--------------------------------------------------

  //////////////////////////
  //KALMAN FILTER START
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

  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;

  timer = micros();
  //////////////////////////
  //KALMAN FILTER END
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
void IMU::run(bool useRollPitch, bool useKalman){
	measureMotion();

	if(!useRollPitch)return;

	calculateRollPitch();

	if(!useKalman)return;

	//////////////////////////
	//KALMAN FILTER START
	double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
	timer = micros();

	double gyroXrate = gx / 131.0; // Convert to deg/s
	double gyroYrate = gy / 131.0; // Convert to deg/s

	#ifdef RESTRICT_PITCH
	  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
	  if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
	    kalmanX.setAngle(roll);
	    compAngleX = roll;
	    kalAngleX = roll;
	    gyroXangle = roll;
	  } else
	    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

	  if (abs(kalAngleX) > 90)
	    gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
	  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
	#else
	  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
	  if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
	    kalmanY.setAngle(pitch);
	    compAngleY = pitch;
	    kalAngleY = pitch;
	    gyroYangle = pitch;
	  } else
	    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter

	  if (abs(kalAngleY) > 90)
	    gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
	  kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
	#endif

	gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
	gyroYangle += gyroYrate * dt;
	//gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
	//gyroYangle += kalmanY.getRate() * dt;

	compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
	compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

	// Reset the gyro angle when it has drifted too much
	if (gyroXangle < -180 || gyroXangle > 180)
	  gyroXangle = kalAngleX;
	if (gyroYangle < -180 || gyroYangle > 180)
	  gyroYangle = kalAngleY;

	//////////////////////////
	//KALMAN FILTER END

	//print the result of the filter
	Serial.print(roll); Serial.print("\t"); //+-180
	Serial.print("\t");
	Serial.print(pitch); Serial.print("\t");  //+-90
	Serial.println();

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