/*
===============================================================================
 Name        : LAB_7.c
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
#include "FLASH.h"
#include "KEYPAD.h"

static char value1[] = "Hello from FLASH\0";	//0x10000

/* Start address for sector 29 */
static void* sector_start_address = 0x00078000;

#define NO_KEY -2
int layoutTest[] = {0x30, 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , NO_KEY, 0x37 , 0x38 , 0x39 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 };

void save_data();
void read_data();
void clear_data();

int lastPressedKey = 0x0;

int main(void)
{
	/* Initialisations */
	LCDText_Init();
	KEYPAD_Init(layoutTest);
	FLASH_Init( );

	LCDText_WriteString("Welcome!");

	for(;;)
	{
		/* 0 - Guardar */
		/* 1 - Ler */
		/* 2 - Apagar */
		int key = KEYPAD_Read();

    	if(key == lastPressedKey) continue;
    	lastPressedKey = key;

		switch(key)
		{
		case 0x30:
			save_data();
			break;
		case 0x31:
			read_data();
			break;
		case 0x32:
			clear_data();
			break;
		default:
			break;
		}
	}

	/*
	int status = FLASH_EraseSector(29);

	status = FLASH_WriteData(pValueOnFlash, &value1, 256);

	if (FLASH_VerifyData(pValueOnFlash, &value1, 4) == CMD_SUCCESS)
	{
		printf("Values are the same!!!\n");
	}

	printf("Value: %c at Address: 0x%lx\n\n", *pValueOnFlash, pValueOnFlash);


	status = FLASH_WriteData(pValueOnFlash, &value2, 256);*/


    return 0;
}


void save_data()
{
	FLASH_EraseSector(29);
	FLASH_WriteData(sector_start_address, &value1, 256);
}

void read_data()
{
	LCDText_Clear();
	LCDText_WriteString((char*)sector_start_address);
}

void clear_data()
{
	FLASH_EraseSector(29);
	LCDText_Clear();
	LCDText_WriteString("Sector 29 has been cleared");
}
