#ifndef uart_h
#define uart_h

#include "MKL25Z4.h"   /* Device header */


void uartInitialize(void);
void UART0_Transmit_Poll(uint8_t data);
uint8_t UART0_Receive_Poll(void);

extern volatile uint8_t acc_read;

void UART0_IRQHandler(void);
#endif
