#include "main.h"

volatile uint8_t acc_read = 30;
double pitch_angle = 0.0;
uint8_t rotation = 0;
acc_data_types acc_readings;
volatile double first_pitch;
volatile double second_pitch;
volatile double first_roll;
volatile double second_roll;

int main (void){
	system_initialize();
	while(1){
		acc_readings.previous = acc_readings.present;
		
		mma8451_read_all_axes(MMA8451_ID, X_AXIS_MSB, &acc_readings.raw);		// measure accelerometer values
		normalize(&acc_readings.raw, &acc_readings.present);		// normalize values to g
		filter(&acc_readings.present, &acc_readings.previous, &acc_readings.filtered);	// low-pass filter
		
		first_pitch = second_pitch;	// first motor responds pitch
		second_pitch = cal_pitch(&(acc_readings.filtered));		// calculate pitch angle
		
		pitch_angle += (first_pitch-second_pitch);	// accumulate angle
		rolls[PREVIOUS] = rolls[PRESENT];		// second motor responds roll
		rolls[PRESENT] = cal_roll(&(acc_readings.filtered));
		
		service_angle();
	}			// while
}

void HardFault_Handler(void){
	SystemInit();
}

void system_initialize(void){
	pitInitialize();
	motorInitialize();
	i2c_Initialize();
	uartInitialize();
	mma8451_initialize();
	i2c_Pause(100);
	UART0_Transmit_Poll(mma8451_read(MMA8451_ID, 0x0E));
	i2c_Pause(100);
	mma8451_read_all_axes(MMA8451_ID, X_AXIS_MSB, &acc_readings.raw);
	i2c_Pause(100);
	normalize(&acc_readings.raw, &acc_readings.present);
	mma8451_read_all_axes(MMA8451_ID, X_AXIS_MSB, &acc_readings.raw);
	normalize(&acc_readings.raw, &acc_readings.present);
	filter(&acc_readings.present, &acc_readings.previous, &acc_readings.filtered);
	second_pitch = cal_pitch(&(acc_readings.filtered));
	rolls[PRESENT] = cal_roll(&(acc_readings.filtered));
}

void service_angle(void){
	if(acc_read != 0x70){		// transmitting 'p' stops displaying data
			UART0_Transmit_Poll((uint8_t)(pitch_angle));
			if(pitch_angle > 5.0 && rotation != 1){
				rotation = 1;
				PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TEN_MASK;
				PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
			}
			else if(pitch_angle < -5.0 && rotation != 2){	
				rotation = 2;
				PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
				PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
			}
			if(1 == rotation){
				if(pitch_angle < 0.6){
					PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
					rotation = 0;
					turnMotorOff(MOTOR1);
				}
			}
			else if(2 == rotation){
				if(pitch_angle > -0.6){
					PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TEN_MASK;
					rotation = 0;
					turnMotorOff(MOTOR1);
				}
			}
		}		// if
}
