#include "pit.h"

void pitInitialize(){
	
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT->CHANNEL[0].LDVAL = delay;
	PIT->CHANNEL[1].LDVAL = delay;
	
	NVIC_ClearPendingIRQ(PIT_IRQn); 	/* Clear any pending interrupt */
	NVIC_EnableIRQ(PIT_IRQn);
	
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;	// let channel 0 generate interrupts
	//PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;	// enable channel 0
	
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;	// let channel 1 generate interrupts
	//PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;	// enable channel 1
	
	PIT->MCR = 0x00;
	
}

void PIT_IRQHandler(){
	if(1 == (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)){
		nextMotorState(1, MOTOR1);
		pitch_angle -= angle_by_step;
		PIT->CHANNEL[0].TFLG &=  PIT_TFLG_TIF_MASK;   	/* write 1 to TCF to clear the LPTMR timer compare flag */
	}
	if(1 == (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK)){
		nextMotorState(0, MOTOR1);
		pitch_angle += angle_by_step;
		PIT->CHANNEL[1].TFLG &=  PIT_TFLG_TIF_MASK;   	/* write 1 to TCF to clear the LPTMR timer compare flag */
	}
}
