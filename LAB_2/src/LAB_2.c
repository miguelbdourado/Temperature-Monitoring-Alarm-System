/*
===============================================================================
 Name        : LAB_2.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

#include "LED.h"
#include "Wait.h"

int main(void) {
    LED_Init(false);
    WAIT_init();

	uint32_t start_time = WAIT_GetElapsedMillis(0);
    for(;;)
    {
    	LED_Toggle();
    	WAIT_Milliseconds(500);

    	//passed 5 seconds
    	if(WAIT_GetElapsedMillis(start_time) >= 5000)
    	{
    		LED_Off();
    		WAIT_Milliseconds(2000);
    		start_time = WAIT_GetElapsedMillis(0);
    	}
    }

    return 0;
}
