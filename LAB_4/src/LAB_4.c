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

// TODO: insert other include files here
#include "LED.h"
#include "Wait.h"
#include "KEYPAD.h"
// TODO: insert other definitions and declarations here

#define NO_KEY -2
int layoutTest[] = {0, 1 , 2 , 3 , 4 , 5 , NO_KEY, 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 };

int main(void)
{

	KEYPAD_Init(layoutTest);
    LED_Init(false);
    WAIT_ChronosInit();
    WAIT_init();

    while(1)
    {
    	int key = KEYPAD_Read();
    	if(key < 0) continue;


    	for(int i = 0; i < key*2; i++)
    	{
    		LED_Toggle();

    		uint32_t start = WAIT_GetElapsedMillis(0);
    		WAIT_ChronoUs(500000);
    		uint32_t delta = WAIT_GetElapsedMillis(start);
    		printf("%d\n", delta);
    	}

    }

    return 0 ;
}
