#include "mma8451.h"

void mma8451_initialize(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;      
  PORTA->PCR[15] |= (0|PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x1)| PORT_PCR_IRQC(0xA));  // set interrupt 2 pin active, falling edge
	

	if(mma8451_read( MMA8451_ID, WHOAMI_REG) == WHOAMI){
		i2c_Pause(100);	
		mma8451_write(MMA8451_ID, CTRL_REG4, 0x01);	// set interrupt 2
		//i2c_Pause(100);
		//mma8451_write(MMA8451_ID, 0x0E, 0x10);	// set high-pass filter active
		i2c_Pause(100);
		mma8451_write(MMA8451_ID, CTRL_REG1, 0x01);	// set mma8451 active
	}		  
	else{
			UART0_Transmit_Poll('e');
	}
}
void mma8451_write(uint8_t slave_address, uint8_t register_address, uint8_t data){
	
	i2c_Start();
	i2c_Write(slave_address | I2C_WRITE);
	i2c_Wait();
	
	i2c_Write(register_address);
	i2c_Wait();
	
	i2c_Write(data);
	i2c_Wait();
	
	i2c_Stop();
	i2c_Pause(50);
}

uint8_t mma8451_read(uint8_t slave_address, uint8_t register_address){
	uint8_t result;
	
	i2c_Start();
	i2c_Write(slave_address);
	i2c_Wait();
	
	i2c_Write(register_address);
	i2c_Wait();
	
	i2c_ReStart;
	i2c_Write(MMA8451_ID | I2C_READ);
	i2c_Wait();
	
	i2c_Receive;
	i2c_Nack;
	
	result = i2c_Read();	// dummy read

	i2c_Wait();
	
	i2c_Stop();
	result = i2c_Read();

	return result;
}
void mma8451_read_all_axes(uint8_t slave_address, uint8_t register_address, uint8_t *dataptr){
	
	int i = 0;
	
	// read setup
	
	i2c_Start();
	i2c_Write(slave_address | I2C_WRITE);
	i2c_Wait();
	
	i2c_Write(register_address);
	i2c_Wait();
	
	i2c_ReStart;
	i2c_Write(MMA8451_ID | I2C_READ);
	i2c_Wait();
	
	i2c_Receive;
		
	
	for(i = 0; i < 6; i++){
		if(5 == i){		// last reading - do not send ACK
			i2c_Nack;
		}
		else{
			i2c_Ack;
		}
		*dataptr = i2c_Read();     // dummy read
		i2c_Wait();
		if(5 == i){		// last reading - send stop
			i2c_Stop();
		}
		*dataptr = i2c_Read(); 
		++dataptr;
	}
}



