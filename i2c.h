#ifndef i2c_h
#define i2c_h

#include "MKL25Z4.h"

#define I2C_READ  1
#define I2C_WRITE 0

#define i2c_MasterStart		I2C0->C1 |= I2C_C1_MST_MASK
#define i2c_SlaveStart 		I2C0->C1 &= ~I2C_C1_MST_MASK
#define i2c_ReStart				I2C0->C1 |= I2C_C1_RSTA_MASK

#define i2c_Transmit 		I2C0->C1 |= I2C_C1_TX_MASK
#define i2c_Receive			  I2C0->C1 &= ~I2C_C1_TX_MASK

#define i2c_Ack 				I2C0->C1 &= ~I2C_C1_TXAK_MASK
#define i2c_Nack				I2C0->C1 |= I2C_C1_TXAK_MASK

#define BUSY_ACK 					while(I2C0->S & 0x01)
#define TRANS_COMP 				while(!(I2C0->S & 0x80))

										
void i2c_Initialize(void);
void i2c_Deinitialize(void);
	
void i2c_Start(void);
void i2c_Stop(void);

void i2c_read_setup(void);
uint8_t i2c_Read(void);
void i2c_Write(uint8_t data);	

void i2c_Wait(void);
void i2c_Pause(uint32_t cycles);
						
#endif

