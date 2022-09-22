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
#include "LCDText.h"

#include "LED.h"
#include "Wait.h"
#include "KEYPAD.h"
// TODO: insert other definitions and declarations here

#define NO_KEY -2
int layoutTest[] = {0x30, 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , NO_KEY, 0x37 , 0x38 , 0x39 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 };
int lastPressedKey = 0x0;

int main(void)
{
	LCDText_Init();
	KEYPAD_Init(layoutTest);
    LED_Init(false);
    WAIT_ChronosInit();
    WAIT_init();

    char str[] = "Pressione qualquer tecla!";
    LCDText_WriteString(str);

    while(1)
    {
    	int key = KEYPAD_Read();

    	if(key == lastPressedKey) continue;
    	lastPressedKey = key;

    	if(key == -2) LCDText_Clear();
    	if(key < 0) continue;

    	LCDText_WriteChar(key);
    }

    return 0 ;
}
