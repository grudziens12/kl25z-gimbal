#ifndef pit_h
#define pit_h

#include "MKL25Z4.h"   /* Device header */
#include "stepper.h"
#include "main.h"

void pitInitialize(void);
void PIT_IRQHandler(void);

#endif
