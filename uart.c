/*
Initialize UART0 on pins A1 - RX, A2 - TX
Baud Rate = 9600
*/
#include "uart.h"

volatile uint8_t acc_read = 30;

void uartInitialize(){
	SIM->SCGC4 |= (SIM_SCGC4_UART0_MASK);
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK);
	PORTA->PCR[1] = PORT_PCR_MUX(2);               
  PORTA->PCR[2] = PORT_PCR_MUX(2);
	SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2);
	
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);
	UART0->C4 &= ~(UART0_C4_OSR_MASK);
	UART0->C4 |= UART0_C4_OSR(31);
	
	UART0->BDH &= ~UART_BDH_SBR_MASK;
	UART0->BDH |= UART_BDH_SBR(0x00);
	UART0->BDL &= ~UART_BDL_SBR_MASK;
	UART0->BDL |= UART_BDL_SBR(26);
	UART0->BDH &= ~UART_BDH_SBNS_MASK;
	
	UART0->C1 &= ~(UART0_C1_M_MASK | UART0_C1_PE_MASK);
	NVIC_SetPriority    (UART0_IRQn, 2); //0, 1, 2, or 3
  NVIC_ClearPendingIRQ(UART0_IRQn   );
  NVIC_EnableIRQ      (UART0_IRQn   );
	UART0->C2 |= (UART0_C2_RIE_MASK);
	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);
}

void UART0_Transmit_Poll(uint8_t data){
	while(!(UART0_S1 & UART_S1_TDRE_MASK));
	UART0->D = data;
}

uint8_t UART0_Receive_Poll(void){
	uint8_t received;
	while(!(UART0_S1 & UART_S1_RDRF_MASK));
	received = UART0->D;
	return received;
}

void UART0_IRQHandler(void){
	if(UART0->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK |
                        UART_S1_FE_MASK | UART_S1_PF_MASK)){
          //clear the error flags
          UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
                       UART0_S1_FE_MASK | UART0_S1_PF_MASK;
	}
	acc_read = UART0->D;
	UART0_Transmit_Poll(acc_read);
}

