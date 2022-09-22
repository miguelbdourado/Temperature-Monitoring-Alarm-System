/*
===============================================================================
 Name        : LAB_8.c
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

#include "LCDText.h"
#include "FLASH.h"
#include "KEYPAD.h"
#include "BMP.h"

int main(void)
{



	BMP_Init();

	while(1)
	{
		unsigned short digT1 = GetReg16(0x88);
		short digT2 = GetReg16(0x8A);
		short digT3 = GetReg16(0x8C);

		short FA = GetReg8(0xFA);
		short FB = GetReg8(0xFB);
		short FC = GetReg8(0xFC);

		unsigned long temperature = (FA << 12) | (FB << 4) | (FC >> 4);

		double var1 = (((double)temperature) / 16384.0 - ((double)digT1) / 1024) *
				((double) digT2);

		double var2 = ((((double)temperature) / 131072.0 - ((double) digT1) / 8192.0) *
		         (((double)temperature) / 131072.0 - ((double) digT1) / 8192.0)) *
		        ((double) digT3);

		double final_temperature = (var1 + var2) / 5120.0;

		double temperature_2 = getTempCelsius();

		printf( "> temperature %f\n", final_temperature );
		printf( "> temperature2 %f\n", temperature_2 );
	}

	return 0;
}
