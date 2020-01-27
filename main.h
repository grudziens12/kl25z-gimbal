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
void HardFault_Handler(void);
void system_initialize(void);
void service_angle(void);
#endif
