
#ifndef stepper_h
#define stepper_h

#include "MKL25Z4.h"                    // Device header

#define MOTOROFFSET 4
#define MOTORPIN1(MOTORNUMBER)  MOTORNUMBER * MOTOROFFSET
#define MOTORPIN2(MOTORNUMBER)  MOTORNUMBER * MOTOROFFSET + 1
#define MOTORPIN3(MOTORNUMBER)  MOTORNUMBER * MOTOROFFSET + 2
#define MOTORPIN4(MOTORNUMBER)  MOTORNUMBER * MOTOROFFSET + 3

#define MOTOR1 0
#define MOTOR2 2

	typedef enum{
			MOTORSTATE1, MOTORSTATE2, MOTORSTATE3, MOTORSTATE4, MOTOROFF
	} motorState;

	static const uint16_t fullRotation = 2048;
	static const uint32_t delay = 50000;
	extern const double angle_by_step;
	
	void motorInitialize(void);
	void motorService(motorState currentState, uint8_t whichMotor);
	void nextMotorState(uint8_t direction, uint8_t whichMotor);
	void turnMotorOff(uint8_t whichMotor);
	void clearMotorPins(uint8_t whichMotor);
	void rotateAngle(uint8_t whichMotor, double angle);
#endif
