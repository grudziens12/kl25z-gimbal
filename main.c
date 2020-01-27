#include "main.h"

volatile uint8_t acc_read = 30;
double pitch_angle = 0.0;
double roll_angle = 0.0;
uint8_t pitch_rotation = 0;
uint8_t roll_rotation = 0;
acc_data_types acc_readings;
volatile double previous_pitch;
volatile double present_pitch;
volatile double present_roll;
volatile double previous_roll;

int main (void){
	system_initialize();
	while(1){
		acc_readings.previous = acc_readings.present;
		
		mma8451_read_all_axes(MMA8451_ID, X_AXIS_MSB, &acc_readings.raw);		// measure accelerometer values
		normalize(&acc_readings.raw, &acc_readings.present);		// normalize values to g
		filter(&acc_readings.present, &acc_readings.previous, &acc_readings.filtered);	// low-pass filter
		
		previous_pitch = present_pitch;	// first motor responds pitch
		present_pitch = cal_pitch(&(acc_readings.filtered));		// calculate pitch angle
		
		pitch_angle += (present_pitch-previous_pitch);	// accumulate angle
		previous_roll = present_roll;		// second motor responds roll
		present_roll = cal_roll(&(acc_readings.filtered));
		
		roll_angle += (present_roll-previous_roll);
		service_angle(&pitch_rotation, &pitch_angle, 0, 12.0, 0.6);
		service_angle(&roll_rotation, &roll_angle, 0, 8.0, 0.6);
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
	present_pitch = cal_pitch(&(acc_readings.filtered));
	present_roll = cal_roll(&(acc_readings.filtered));
}

void service_angle(uint8_t* rotation, double* angle, uint8_t pit_channel, float high_threshold, float low_threshold){
	if(acc_read != 0x70){		// transmitting 'p' stops displaying data
			UART0_Transmit_Poll((uint8_t)(*angle));
			if(*angle > high_threshold && *rotation != 1){
				*rotation = 1;
				PIT->CHANNEL[pit_channel].TCTRL |= PIT_TCTRL_TEN_MASK;
			}
			else if(*angle < -high_threshold && *rotation != 2){	
				*rotation = 2;
				PIT->CHANNEL[pit_channel].TCTRL |= PIT_TCTRL_TEN_MASK;
			}
			if(1 == *rotation){
				if(*angle < low_threshold){
					//PIT->CHANNEL[pit_channel].TCTRL &= ~PIT_TCTRL_TEN_MASK;
					*rotation = 0;
					//turnMotorOff(MOTOR1);
				}
			}
			else if(2 == *rotation){
				if(*angle > -low_threshold){
					//PIT->CHANNEL[pit_channel].TCTRL &= ~PIT_TCTRL_TEN_MASK;
					*rotation = 0;
					//turnMotorOff(MOTOR1);
				}
			}
		}		// if
}
