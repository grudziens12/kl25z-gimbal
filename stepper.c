#include "stepper.h"

motorState motor1State = MOTOROFF;
motorState motor2State = MOTOROFF;
const double angle_by_step = 360.0/2048.0;

void motorInitialize(void){
	
	SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK; 					/* Enable clock for port C */
	PORTB->PCR[MOTORPIN1(MOTOR1)] |= PORT_PCR_MUX(1); 	// set GPIO
	PORTB->PCR[MOTORPIN2(MOTOR1)] |= PORT_PCR_MUX(1); 
	PORTB->PCR[MOTORPIN3(MOTOR1)] |= PORT_PCR_MUX(1); 
	PORTB->PCR[MOTORPIN4(MOTOR1)] |= PORT_PCR_MUX(1); 	
	
	PORTB->PCR[MOTORPIN1(MOTOR2)] |= PORT_PCR_MUX(1); 
	PORTB->PCR[MOTORPIN2(MOTOR2)] |= PORT_PCR_MUX(1); 
	PORTB->PCR[MOTORPIN3(MOTOR2)] |= PORT_PCR_MUX(1); 
	PORTB->PCR[MOTORPIN4(MOTOR2)] |= PORT_PCR_MUX(1); 	
	
	PTB->PDDR |= (1<<MOTORPIN1(MOTOR1));
	PTB->PDDR |= (1<<MOTORPIN2(MOTOR1));
	PTB->PDDR |= (1<<MOTORPIN3(MOTOR1));
	PTB->PDDR |= (1<<MOTORPIN4(MOTOR1));
	
	PTB->PDDR |= (2<<MOTORPIN1(MOTOR2));
	PTB->PDDR |= (2<<MOTORPIN2(MOTOR2));
	PTB->PDDR |= (2<<MOTORPIN3(MOTOR2));
	PTB->PDDR |= (2<<MOTORPIN4(MOTOR2));
}
	
void motorService(motorState currentState, uint8_t whichMotor){
	clearMotorPins(whichMotor);
	switch(currentState){
		case MOTORSTATE1:
			PTB->PSOR |= (1UL << MOTORPIN1(whichMotor));
			PTB->PSOR |= (1UL << MOTORPIN2(whichMotor));
			break;
		
		case MOTORSTATE2:
			PTB->PSOR |= (1UL << MOTORPIN2(whichMotor));
			PTB->PSOR |= (1UL << MOTORPIN3(whichMotor));
			break;
		
		case MOTORSTATE3:
			PTB->PSOR |= (1UL << MOTORPIN3(whichMotor));
			PTB->PSOR |= (1UL << MOTORPIN4(whichMotor));
			break;
		
		case MOTORSTATE4:
			PTB->PSOR |= (1UL << MOTORPIN4(whichMotor));
			PTB->PSOR |= (1UL << MOTORPIN1(whichMotor));
			break;
		
		case MOTOROFF:
			
			break;
		
		default:
			
				break;
	}
}

void nextMotorState(uint8_t direction, uint8_t whichMotor){
	motorState newMotorState;
	motorState currentMotorState = (MOTOR1 ==  whichMotor ? motor1State : motor2State);
	
	if (1 == direction){ 			// rotate counterclockwise
		switch(currentMotorState){
			case MOTORSTATE1:
				newMotorState = MOTORSTATE2;
				break;
			
			case MOTORSTATE2:
				newMotorState = MOTORSTATE3;
				break;
			
			case MOTORSTATE3:
				newMotorState = MOTORSTATE4;
				break;
			
			case MOTORSTATE4:
				newMotorState = MOTORSTATE1;
				break;
			
			case MOTOROFF:
				newMotorState = MOTORSTATE1;
				break;
			
			default:
				newMotorState = MOTOROFF;
					break;
		}
	}
	else{				// rotate clockwise
		switch(currentMotorState){
			case MOTORSTATE1:
				newMotorState = MOTORSTATE4;
				break;
			
			case MOTORSTATE2:
				newMotorState = MOTORSTATE1;
				break;
			
			case MOTORSTATE3:
				newMotorState = MOTORSTATE2;
				break;
			
			case MOTORSTATE4:
				newMotorState = MOTORSTATE3;
				break;
			
			case MOTOROFF:
				newMotorState = MOTORSTATE1;
				break;
			
			default:
				newMotorState = MOTOROFF;
					break;
		}
	}
	if(MOTOR1 == whichMotor){
		motor1State = newMotorState;
		motorService(motor1State, MOTOR1);
	}
	else{
		motor2State = newMotorState;
		motorService(motor2State, MOTOR2);
	}
}

void turnMotorOff(uint8_t whichMotor){
	if(MOTOR1 == whichMotor){
		motor1State = MOTOROFF;
		motorService(motor1State, MOTOR1);
	}
	else{
		motor2State = MOTOROFF;
		motorService(motor2State, MOTOR2);
	}
}

void clearMotorPins(uint8_t whichMotor){
	PTB->PCOR |= (1UL << MOTORPIN1(whichMotor));
	PTB->PCOR |= (1UL << MOTORPIN2(whichMotor));
	PTB->PCOR |= (1UL << MOTORPIN3(whichMotor));
	PTB->PCOR |= (1UL << MOTORPIN4(whichMotor));
}

void rotateAngle(uint8_t whichMotor, double angle){
	uint16_t numberOfSteps;
	uint8_t direction;
	if(angle < 0.0){
		angle = -angle;
		direction = 0;
	}
	else{
		direction = 1;
	}
	numberOfSteps = (uint16_t)(((double)fullRotation*angle)/360.0);
	for(int i=0; i<numberOfSteps;++i){
		nextMotorState(direction, whichMotor);
		for(int k=0; k<=delay; ++k);
	}
	turnMotorOff(whichMotor);
}
