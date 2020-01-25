#ifndef mma8451_h
#define mma8451_h

#include "i2c.h"
#include "uart.h"

#define MMA8451_ID 0x3A
#define WHOAMI_REG 0x0D
#define WHOAMI 		 0x1A
#define CTRL_REG1  0x2A
#define CTRL_REG4  0x2D

#define X_AXIS_MSB 0x01
#define X_AXIS_LSB 0x02

#define Y_AXIS_MSB 0x03
#define Y_AXIS_LSB 0x04

#define Z_AXIS_MSB 0x05
#define Z_AXIS_LSB 0x06

void mma8451_initialize(void);

extern uint8_t mma8451_read_adress;
extern uint8_t mma8451_write_adress;

void mma8451_write(uint8_t slave_address, uint8_t register_address, uint8_t data);
uint8_t mma8451_read(uint8_t slave_address, uint8_t register_address);
void mma8451_read_all_axes(uint8_t slave_address, uint8_t register_address, uint8_t *dataptr);

void axis_read(void);

#endif
