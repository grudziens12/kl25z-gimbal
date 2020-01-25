#include "i2c.h"

void i2c_Initialize(void){
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM -> SCGC4 |= SIM_SCGC4_I2C0_MASK;

	PORTE -> PCR[24] = PORT_PCR_MUX(5);
  PORTE -> PCR[25] = PORT_PCR_MUX(5);
	
	I2C0 -> F  |= I2C_F_ICR(0x94); //SCL start hold time = 4.25us SCL stop hold time = 5.125us SDA hold time = 2.125us,
	I2C0 -> C1 |= I2C_C1_IICEN_MASK; // 0x80
	I2C0 -> C1 |= I2C_C1_IICIE_MASK;
	I2C0 -> C2 |= (I2C_C2_HDRS_MASK);
}
void i2c_Deinitialize(void){
    I2C0 -> C1 = 0x00;
	
    SIM -> SCGC4 &= ~SIM_SCGC4_I2C0_MASK;
}
void i2c_Wait(void){
	while((I2C0->S & I2C_S_IICIF_MASK)==0){}
		I2C0->S |= I2C_S_IICIF_MASK;
}
void i2c_Pause(uint32_t cycles){
	volatile uint32_t i;
for( i = cycles*1000; i > 0; i--){}
}
void i2c_Start(void){
	i2c_MasterStart;
	i2c_Transmit;
}
void i2c_Stop(void){
	i2c_SlaveStart;
	i2c_Receive;
}
void i2c_Write(uint8_t data){
	I2C0 -> D = data;
}
uint8_t i2c_Read(void){
	return I2C0 -> D;
}
