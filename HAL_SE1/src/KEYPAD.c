#include "KEYPAD.h"
/*
 * 	P2[4]	- ROW0
	P2[5]	- ROW1
	P2[6]	- ROW2
	P2[7]	- ROW3
	P2[0]	- COLUMN0
	P2[1]	- COLUMN1
	P2[2]	- COLUMN2
	P2[3]	- COLUMN3
 */
int KEYPAD_Init(int layout[16])
{
	//Set function to GPIO for pins P2.[0..7]
	LPC_PINCON->PINSEL4 &= ~(0xFFFF);

	//Set PINMODE to pull-up
	LPC_PINCON->PINMODE4 &= ~(0xFFFF);

	//Set output mode P2.[4..7] (ROWS)
	LPC_GPIO2->FIODIR |= (0xF << 4);

	//Set input mode P2.[0..3] (COLUMNS)
	LPC_GPIO2->FIODIR &= ~(0xF);

	memcpy(__layout, layout, sizeof(int) * 16);
	return 0;
}

bool KEYPAD_Hit()
{
	//Put all rows to 0 so we can read the columns
	LPC_GPIO2->FIOCLR |= (0xF << 4);

	return ((LPC_GPIO2->FIOPIN & 0xF) < 0xF);
}


uint16_t KEYPAD_Read()
{
	uint16_t bitmask = 0;

	for(int column = 0; column < 4; column++)
	{
		for(int row = 3; row >= 0; row--)
		{
			LPC_GPIO2->FIOSET = (0xF << 4);
			LPC_GPIO2->FIOCLR |= (1 << (4+row));
			if((LPC_GPIO2->FIOPIN & (1 << column)) == 0)
			{
				int index = row * 4 + column;
				bitmask |= (0x1 << index);
			}
		}
	}

	return bitmask;
}

bool isKeyPressed(int keyCode)
{
	uint16_t keys = KEYPAD_Read();
	size_t index = 0;

	while ( index < 16 && __layout[index] != keyCode ) ++index;

	return (0x1 << index) & keys;
}

bool isKeyDown(int keyCode)
{
	uint16_t keys = KEYPAD_Read();
	size_t index = 0;

	while ( index < 16 && __layout[index] != keyCode ) ++index;

	return ((0x1 << index) & keys) && !((0x1 << index) & lastTickKeys);
}

bool isKeyUp(int keyCode)
{
	uint16_t keys = KEYPAD_Read();
	size_t index = 0;

	while ( index < 16 && __layout[index] != keyCode ) ++index;

	return !((0x1 << index) & keys) && ((0x1 << index) & lastTickKeys);
}


void updateKeys()
{
	lastTickKeys = KEYPAD_Read();
}
