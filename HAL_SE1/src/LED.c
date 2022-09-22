/*
 * LED.c
 *
 *  Created on: 28 Oct 2021
 *      Author: Daniel Dron		46101
 *      		Miguel Dourado	46063
 *      		Diogo Cunha		46105
 */
#include "LED.h"

void LED_Init(bool state)
{
    LPC_PINCON->PINSEL1 &= ~(0x3 << (22 - 16));
    LPC_GPIO0->FIODIR |= (1 << 22);

    if(state) LED_On();
    else LED_Off();
}

bool LED_GetState()
{
	return __curr_state;
}

void LED_On()
{
	__curr_state = true;
	LPC_GPIO0->FIOSET = (1 << 22);
}

void LED_Off()
{
	__curr_state = false;
	LPC_GPIO0->FIOCLR = (1 << 22);
}

void LED_Toggle()
{
	__curr_state = !__curr_state;
	if(__curr_state) LED_On();
	else LED_Off();
}
