/*
===============================================================================
 Name        : LAB_1.c
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

// TODO: insert other definitions and declarations here

int main(void) {
    LPC_PINCON->PINSEL1 &= ~(0x3 << (22 - 16)); //SET INPUT/OUTPUT FOR PIN P0.22 //page 118
    LPC_GPIO0->FIODIR |= (1 << 22);				//SET OUTPUT MODE FOR PIN P0.22	 //page 132

    for(;;)
    {
    	LPC_GPIO0->FIOSET = (1 << 22); //TURN ON LED2		//page 132

    	//WAIT 500ms
    	for(int i = 0; i < 500; i++)
    		for(int j=0;j<20000;j++);


    	LPC_GPIO0->FIOCLR = (1 << 22); //TURN OFF LED2		//page 132

    	//WAIT 500ms
    	for(int i = 0; i < 500; i++)
    		for(int j=0;j<20000;j++);
    }


    return 0 ;
}
