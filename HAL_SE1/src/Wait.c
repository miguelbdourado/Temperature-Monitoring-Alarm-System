/*
 * Wait.c
 *
 *  Created on: 28 Oct 2021
 *      Author: Daniel Dron		46101
 *      		Miguel Dourado	46063
 *      		Diogo Cunha		46105
 */

#include "Wait.h"

int WAIT_init()
{
	SystemCoreClockUpdate();
	if (SysTick_Config(SYSTICK_FREQ) == 1) return -1;	//Failed
	return 0;	//Succeeded
}

void SysTick_Handler(void)
{
	__ms++;
}

void WAIT_Milliseconds(uint32_t milliseconds)
{
	uint32_t start = __ms;

	while((__ms - start) < milliseconds)
	{
		__WFI();
	}
}

uint32_t WAIT_GetElapsedMillis(uint32_t start)
{
	return __ms - start;
}

void WAIT_ChronosInit()
{
	LPC_SC->PCONP |= TIMER0_BIT;	//Enable Timer 0
	LPC_SC->PCLKSEL0 &= ~(0x3 << 2);	//1/4 CCLK
	LPC_TIM0->PR = (SystemCoreClock/4)/1000000 - 1;
	LPC_TIM0->TCR = COUNTER_OFF | RESET_ON; //Counter disabled and Reset Enabled
	LPC_TIM0->IR = CLEAR_MR0I;  //Reset the interrupt for match channel 0
	NVIC_EnableIRQ(TIMER0_IRQn);
}

int WAIT_ChronoUs(uint32_t waitUs)
{
	__CHEGUEI = 0;
	LPC_TIM0->MR0 = waitUs;
	LPC_TIM0->MCR = MR0I | MR0S; //Enable interrupt on match 0 and disable counter on match 0
	LPC_TIM0->TCR = COUNTER_ON | RESET_OFF; //Counter enabled and Reset disabled
	while(!__CHEGUEI)
	{
		__WFI();
	}

	return 0;
}

void TIMER0_IRQHandler()
{
	__CHEGUEI = 1;
	LPC_TIM0->IR = CLEAR_MR0I;  //Reset the interrupt for match channel 0
	LPC_TIM0->TCR = COUNTER_OFF | RESET_ON; //Enable counter reset
}


