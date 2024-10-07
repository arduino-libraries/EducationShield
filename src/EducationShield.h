#ifndef EducationShield_H
#define EducationShield_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Servo.h>
#include "utils/CapacitiveSensor.h"
#include <SD.h>

#if defined(__arc__)
#include <CurieBLE.h>
#include <CurieIMU.h>
#endif

#if defined(ARDUINO_ARCH_RENESAS_UNO) || defined(ARDUINO_ARCH_RENESAS)
#include <ArduinoBLE.h>
#endif

#define LED_LENGTH 20
#define BUTTONGROUP_LENGTH 10

class VUMeter{
	public:
		VUMeter();
		void config(int length, int pins[]);
		void begin();
		void on(int index);
		void off(int index);
		void scrollLeft(int speed,int startIndex=0);
		void scrollRight(int speed,int startIndex=0);
		void fill(int length);
		void fillFrom(int leftIndex,int rightIndex);
		void blink(int index, int speed, int times=1);
		void blinkAll(int speed,int times=1);
		void clear();

		void test();
	private:
		void blinkOnce(int index,int onTime,int offTime);
		//int getPin(int index);
		int pins[LED_LENGTH];
		int pinCount;
};

#define MELODY_LENGTH 32

class Melody{
	public:
		Melody(int pin);
		void play(int length, int notes[],int diration[], float speed=1.3);
		void beep(int note=20, int length=30);
		void playTone(int note, int length=10);

		void effect_win();
		void effect_gameover();
		void effect_score();

	private:
		int pin;

};

class Button{
	public:
		Button(int pin=8,bool pressedValue=HIGH);
		void begin();
		bool pressed(int timeout=0);
		bool released(int timeout=0);
		bool doublePressed(int timeout=0,int tolerance=500);

		bool isPressed();
		virtual bool getState();

	protected:
		int pin;
		bool pressedValue;
		bool lastState;
		long debounceTime;
		bool checkPress(int timeout, bool requiredState);

};

class ButtonGroup{
	public:
		ButtonGroup();
		void begin(int length, int buttons[], bool pressedValue=HIGH);
		int pressed(int timeout=0);
	private:
		int buttons[BUTTONGROUP_LENGTH];
		//bool iStarted[BUTTONGROUP_LENGTH];
		int buttonsCount;
		bool pressedValue;

		int checkPress(int timeout, bool requiredState);
};

class LightSensor : public Button{
	public:
		LightSensor(int pin=A1);
		void config(int baseValue,int threshold);
		virtual bool getState();
		void test();
		void calibrate(int t=5000);
		void showConfig();

	protected:
		int base;
		int threshold;

};

class TiltSwitch : public Button{
	public:
		TiltSwitch(int pin, bool pressedValue=LOW);
};


class CapacitiveSwitch : public Button{
	public:
		CapacitiveSwitch(int pin_in=13, int pin=12);
		void config(int threshold);
		void test();
		long getValue(int min=0);
		virtual bool getState();

	protected:
		CapacitiveSensor sensor;
		int threshold;
};

class LED{
	//Mostly for the LED component in Tinkerkit
	public:
		LED(int pin=9);
		void begin();
		void on();
		void off();
		void blink(int speed,int times=1);
	private:
		int pin;
};

class PiezoKnockSensor : public Button{
	public:
		PiezoKnockSensor(int pin=A0);
		void config(int threshold=40,int debounceTime=80);
		bool knocked(int timeout=0);
		void test();
	private:
		int threshold;
};


class Player{
    public:
        Player();
        void begin();
        void play(char* name);

    private:
        void initPlayer();
        void initSD();
        void printDirectory(File dir, int numTabs);
        File root;
        char* name;
};


class Knob{
	public:
		Knob(int pin);
		int getValue();
		void setLevels(int levels);
		int getLevel();
	private:
		int pin;
		int levels;
};


class Joystick{
    public:
        Joystick(int x, int y);
        int getX();
        int getY();
    private:
        int x;
        int y;
    };

class Wheels{
    public:
        Wheels(int lpin=10, int rpin=9);
        void begin();
        void goForward();
        void goBackwards();
        void turnLeft();
        void turnRight();
        void standStill();
        void follow(int d);
    private:
        int top, low, still;
        void go(int tl, int tr);
        int lpin, rpin;
        Servo left, right;
        int fromL, fromR;
        int toL, toR;
        int tl, tr;
    };
#define KP 30
#define KD 7
#define LINE_THRESSHOLD 30
#define ROBOT_SPEED 50 //In % [0..100]
#define INTEGRATION_TIME 10

class IRArray{
    public:
        IRArray(int IR1=A1, int IR2=A2, int IR3=A3);
        void begin();
        void test();
				void setThreshold(int t);
        int readBinary();
        int readLine();
    private:
        int IR1, IR2, IR3;
        int sensPins[3];
        int sensorVal[3];
        int toBinary[3];
        int translateBinary();
        int calculateVelocity(int s);
        int last_diff;
				int threshold;

};

class UltrasonicSensor{
    public:
        UltrasonicSensor(int trig=11, int echo=12);
		int getDistance();
    private:
		int trig, echo;
};


#if defined(__arc__) || defined(ARDUINO_ARCH_RENESAS_UNO) || defined(ARDUINO_ARCH_RENESAS)

class BLEPeripheralBase{
	public:
		void setName(const char* name);
		//virtual void begin();
		bool searchCentral();
		bool connected();
		virtual bool dataReceived()=0;

		static const int MAX_LENGTH=20;

	protected:
		BLEPeripheralBase(/*const char* serviceID*/);
		BLEDevice central;

};

class BLEText : public BLEPeripheralBase{
	public:
		BLEText();
		void begin();
		bool dataReceived();

		void fetchData();
		char getCharAt(int position);

	protected:
		BLECharacteristic textChari;

		char readBuffer[MAX_LENGTH];
};

#define SERVICEID_MESSENGER "19b10000-e8f2-537e-4f6c-d104768a1214"
#define SERVICEID_LOGOROBOT "19f82bd2-da79-11e5-b5d2-0a1d41d68578"
#define SERVICEID_TAMAGOTCHI "361dbb0c-0193-49dd-93af-753ab760a344"

#define TYPE_NONE -1
#define TYPE_MESSENGER 1
#define TYPE_LOGOROBOT 2
#define TYPE_TAMAGOTCHI 3

class BLEuart : public BLEPeripheralBase{
	public:
		//BLEuart(const char* serviceID="6E500001-B5A3-F393-E0A9-E50E24DCCA9E");
		BLEuart(int exampleID=TYPE_NONE);
		void begin();
		void setExampleID(int exampleID);

		bool dataReceived();

		void fetchData();
		void send();

		void sendString(const char*, int length);
		const char* receivedString();
		int getReceivedLength();
		void addValue(int val);
		void addValueAt(unsigned char val, int position);
		unsigned char getValueAt(int position);

	protected:
		BLEService service;
		BLECharacteristic txChari;
		BLECharacteristic rxChari;
		BLECharacteristic typeChari;

		unsigned short receivedLength;
		unsigned char readBuffer[MAX_LENGTH];
		unsigned char writeBuffer[MAX_LENGTH];

		int writeLength;
	private:

};

#define USE_ISR false
#define FILTER_TYPE 0

class IMU{
	public:
		IMU();
		void begin(int accRange=2, int gyroRange=500);
		void calibrate();
		static void run();

		void detectShock(int shockThreashold=192, int shockDuration=11);
		void attachCallback(void (*callback)(void));

		int getPitch();
		int getRoll();

		int getAccelerometerX();
		int getAccelerometerY();
		int getAccelerometerZ();
		float getAccelerometerX_g();
		float getAccelerometerY_g();
		float getAccelerometerZ_g();

		int getGyroX();
		int getGyroY();
		int getGyroZ();
		float getGyroX_dps();
		float getGyroY_dps();
		float getGyroZ_dps();
	private:
		static void measureMotion();

		static void calculateRollPitch();
		static void calculateComplementaryRollPitch();

		static float convertAcclerometer_g(int16_t rawVal);
		static float convertGyro_dps(int16_t rawVal);

		static long timer;
		static bool useISR;

		//Kalman kalmanX;
		//Kalman kalmanY;
		static int accRange, gyroRange;

		static int16_t ax, ay, az;         // accelerometer values
		static int16_t gx, gy, gz;         // gyrometer values

		static float pitch, roll;

};
#endif
/*
class BLEAlienBaby : public BLEPeripheralBase{
	public:
		BLEAlienBaby();
		void begin();
};
*/
#endif
