#ifndef main_h
#define main_h

#include "MKl25Z4.h"
#include "stepper.h"
#include "uart.h"
#include "mma8451.h"
#include "angle.h"
#include "pit.h"

int main (void);

extern double pitch_angle;
extern double roll_angle;
extern uint8_t pitch_rotation;
extern uint8_t roll_rotation;
void HardFault_Handler(void);
void system_initialize(void);
void service_angle(uint8_t* rotation, double* angle, uint8_t pit_channel, float high_threshold, float low_threshold);
#endif
