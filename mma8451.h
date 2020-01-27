#ifndef mma8451_h
#define mma8451_h

#include "i2c.h"
#include "uart.h"

#define MMA8451_ID 0x3A
#define WHOAMI_REG 0x0D
#define WHOAMI 		 0x1A
#define CTRL_REG1  0x2A
#define CTRL_REG2  0x2B
#define CTRL_REG4  0x2D

#define X_AXIS_MSB 0x01
#define X_AXIS_LSB 0x02

#define Y_AXIS_MSB 0x03
#define Y_AXIS_LSB 0x04

#define Z_AXIS_MSB 0x05
#define Z_AXIS_LSB 0x06

#define INDEX(x) x-1

typedef struct acc_data_int{
	uint8_t X_MSB;
	uint8_t X_LSB;
	uint8_t Y_MSB;
	uint8_t Y_LSB;
	uint8_t Z_MSB;
	uint8_t Z_LSB;
} acc_data_int;

typedef struct acc_data_double{
	double X_AXIS;
	double Y_AXIS;
	double Z_AXIS;
} acc_data_double;

typedef struct acc_data_types{
	acc_data_int raw;
	acc_data_double previous;
	acc_data_double present;
	acc_data_double filtered;
} acc_data_types;

void mma8451_initialize(void);

extern uint8_t mma8451_read_adress;
extern uint8_t mma8451_write_adress;

void mma8451_write(uint8_t slave_address, uint8_t register_address, uint8_t data);
uint8_t mma8451_read(uint8_t slave_address, uint8_t register_address);
void mma8451_read_all_axes(uint8_t slave_address, uint8_t register_address, acc_data_int* result);

void axis_read(void);

#endif
