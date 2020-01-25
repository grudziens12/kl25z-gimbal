#include "MKl25Z4.h"
#include "stepper.h"
#include "uart.h"
#include "mma8451.h"


int main (void){
	uint8_t axis_data[6];
	motorInitialize();
	i2c_Initialize();
	uartInitialize();
	mma8451_initialize();

	UART0_Transmit_Poll(acc_read);
	while(1){

		i2c_Pause(100);

		mma8451_read_all_axes(MMA8451_ID, X_AXIS_MSB, axis_data); 
		if(acc_read != 0x70){		// transmitting 'p' stops displaying data
			for(int i = 0; i<6; i++){
				UART0_Transmit_Poll(axis_data[i]);
			}	// for
		}		// if
	}			// while
}


