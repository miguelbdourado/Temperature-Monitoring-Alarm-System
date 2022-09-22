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
#include <float.h>

#include "LCDText.h"
#include "FLASH.h"
#include "RTC.h"
#include "Wait.h"
#include "KEYPAD.h"
#include "BMP.h"

/* KEYBOARD */
#define UP 		0x5
#define DOWN 	0xD
#define LEFT 	0x8
#define RIGHT 	0xA
#define CONFIRM 0x9
#define NO_KEY	0x0
int layout[] = {
		NO_KEY, NO_KEY,  NO_KEY, NO_KEY,
		NO_KEY, UP, 	 NO_KEY, NO_KEY,
		LEFT, 	CONFIRM, RIGHT,  NO_KEY,
		NO_KEY, DOWN, 	 NO_KEY, NO_KEY
};
int lastPressedKey = 0x0;

/* STATES */
typedef void state_fn(void);
state_fn* state;

// General Modes
void alarmMode();
void normalMode();
void maintenanceMode();

//Maintenance Options
void temperatureUnit();
void temperatureLimits();
void dateAndTime();
void showAlarms();

/* MAINTENANCE */
bool maintenanceModeKeyLastPressed = false;
int maintenanceModeTimer = 2000;
int maintenanceModeCurrTimer = 0;
int8_t indexMenu = 0;
bool menuChanged = true;
const uint8_t menuCount = 4;
const char* menuItems[4] = { "Temp Unit", "Temp Limits", "Date & Time", "Alarms" };
state_fn* menuFunctions[4] = { temperatureUnit, temperatureLimits, dateAndTime, showAlarms };

/* SCREEN */
int showScreenTimer 	= 5000;
int showScreenCurrTimer = 0;
struct tm currentTime 	= {};

/* TEMPERATURE */
enum temperatureMode { CELSIUS = 0, FAHRENHEIT = 1 };
enum temperatureStatus { HIGH_TEMP, LOW_TEMP, NONE };
const char* temperatureModeNames[] = { "C", "F" };
enum temperatureMode currentTemperatureMode = CELSIUS;
enum temperatureStatus currentTemperatureStatus = NONE;
double temperature 		= 0;
//TODO: keep in mind FAHRENHEIT possibility
double minTemperature 	= 21.0f;
double maxTemperature 	= 25.0f;
double maxHitTemperature = DBL_MIN;
double minHitTemperature = DBL_MAX;

/* alarm */
bool justEnteredalarmMode 	= true;
int alarmModeTimer 			= 600000; // 10Minutes
int alarmModeCurrTimer 		= -1;

typedef struct _alarm
{
	struct tm time;
	enum temperatureStatus status;
} Alarm, * PAlarm;

typedef struct _flash_memory
{
	int magicWord;
	enum temperatureMode tempMode;
	int alarmCount;
} FlashMemory, * PFlashMemory;

/* Start address for sector 29 */
static void* sector_start_address = (void*)0x00078000;
int magicWord = 0xA124A2D2;
char flashMemory[4096];
FlashMemory initialFlashMemory;
Alarm newalarm = {};

void updateTemperature()
{
	if(currentTemperatureMode == CELSIUS)
		temperature = getTempCelsius();
	else
		temperature = getTempFahrenheit();
}

void updateScreen()
{
	RTC_GetValue(&currentTime);

	/* Display date and time on LCD */
	LCDText_SetCursor(0, 0);
	LCDText_Printf("%02hu:%02hu:%04hu %02hu:%02hu", currentTime.tm_mday, currentTime.tm_mon, currentTime.tm_year, currentTime.tm_hour, currentTime.tm_min );

	//alarm mode
	if(currentTemperatureStatus == HIGH_TEMP)
		LCDText_Printf("   %.1f%s HIGH   ", temperature, temperatureModeNames[currentTemperatureMode]);
	else if(currentTemperatureStatus == LOW_TEMP)
		LCDText_Printf("   %.1f%s LOW   ", temperature, temperatureModeNames[currentTemperatureMode]);
	else
		LCDText_Printf("%.1f%s           ", temperature, temperatureModeNames[currentTemperatureMode]);
}

bool isKeyBeingPressed()
{
	return isKeyPressed(DOWN) || isKeyPressed(UP) || isKeyPressed(RIGHT) || isKeyPressed(LEFT) || isKeyPressed(CONFIRM);
}

// check if the left and right key are being pressed for more than 2 seconds
bool maintenancePressed()
{
	if(isKeyPressed(LEFT) && isKeyPressed(RIGHT))
	{
		if(maintenanceModeKeyLastPressed == false)
		{
			maintenanceModeCurrTimer = WAIT_GetElapsedMillis(0);
			maintenanceModeKeyLastPressed = true;
		} else if((WAIT_GetElapsedMillis(0) - maintenanceModeCurrTimer) > maintenanceModeTimer)
		{
			maintenanceModeCurrTimer = 0;
			maintenanceModeKeyLastPressed = false;

			return true;
		}
	}
	else {
		maintenanceModeCurrTimer = 0;
		maintenanceModeKeyLastPressed = false;
	}

	return false;
}

bool shouldShowScreen()
{
	if(isKeyBeingPressed())
		showScreenCurrTimer = showScreenTimer + WAIT_GetElapsedMillis(0);
	else if( (int)(showScreenCurrTimer - WAIT_GetElapsedMillis(0)) <= 0)
		return false;

	return true;
}

double getMaxTemperature()
{
	if(currentTemperatureMode == CELSIUS)
		return maxTemperature;

	return (maxTemperature * 9 / 5) + 32;
}

double getMinTemperature()
{
	if(currentTemperatureMode == CELSIUS)
		return minTemperature;

	return (minTemperature * 9 / 5) + 32;
}

enum temperatureStatus getTemperatureLimitStatus()
{
	if(temperature > getMaxTemperature()) return HIGH_TEMP;
	else if(temperature < getMinTemperature()) return LOW_TEMP;

	return NONE;
}

void normalMode()
{
	updateTemperature();

	if(shouldShowScreen())
	{
		//Turn on LCD if it was turned off
		if(!LCDText_GetStatus())
			LCDText_SetStatus(true);

		updateScreen();
	}
	//if the LCD was on but we shouldn't draw turn it off
	else if(LCDText_GetStatus())
	{
		LCDText_Clear();
		LCDText_SetStatus(false);
	}

	if( (currentTemperatureStatus = getTemperatureLimitStatus()) != NONE )
	{
		state = alarmMode;
		justEnteredalarmMode = true;
	}

	if(maintenancePressed())
		state = maintenanceMode;

}

void saveNewalarm()
{
	//create new alarm
	newalarm.status = currentTemperatureStatus;
	memcpy(&newalarm.time, &currentTime, sizeof(struct tm));

	int alarmCount = 0;
	memcpy(&alarmCount, sector_start_address + sizeof(int) + sizeof(CELSIUS), sizeof(int));

	//Copy from flash
	int sizeToCopy = sizeof(FlashMemory) + sizeof(Alarm) * (alarmCount+1);
	memcpy(flashMemory, sector_start_address, sizeToCopy);

	//Copy new alarm to allocated memory
	((PFlashMemory)flashMemory)->alarmCount += 1;
	unsigned int offset = sizeof(FlashMemory) + (((PFlashMemory)flashMemory)->alarmCount - 1) * sizeof(Alarm);
	memcpy((void*)(flashMemory + offset), &newalarm, sizeof(Alarm));

	FLASH_EraseSector(29);
	FLASH_WriteData(sector_start_address, flashMemory, 4096);
}

void alarmMode()
{
	//if just entered alarm mode, setup beginning of 10 minutes timer
	if(justEnteredalarmMode)
	{
		maxHitTemperature = DBL_MIN;
		minHitTemperature = DBL_MAX;

		justEnteredalarmMode = false;
		alarmModeCurrTimer = alarmModeTimer + WAIT_GetElapsedMillis(0);

		saveNewalarm();
	}


	//Turn on LCD if it was turned off
	if(!LCDText_GetStatus())
		LCDText_SetStatus(true);

	updateTemperature();


	//Clamp temperature to its max/min to show on alarm
	if(currentTemperatureStatus == HIGH_TEMP)
	{
		if(temperature > maxHitTemperature)
		{
			maxHitTemperature = temperature;
		}

		//render only the max hit temperature at alarm time
		temperature = maxHitTemperature;

	} else if(currentTemperatureStatus == LOW_TEMP)
	{
		if(temperature < maxHitTemperature)
		{
			minHitTemperature = temperature;
		}

		//render only the max hit temperature at alarm time
		temperature = minHitTemperature;

	}

	updateScreen();

	if(isKeyBeingPressed())
		state = normalMode;

	//count down timer of 10 minutes
	if( (int)(alarmModeCurrTimer - WAIT_GetElapsedMillis(0)) <= 0 )
		state = normalMode;
}

// Celsius <
// Fahrneintent

bool firstSelected = true;
void temperatureUnit()
{
	LCDText_SetCursor(0, 0);
	LCDText_Printf("CELSIUS");
	if(firstSelected) LCDText_Printf(" <");
	LCDText_SetCursor(1, 0);
	LCDText_Printf("FAHRENHEIT");
	if(!firstSelected) LCDText_Printf(" <");

	if(isKeyDown(UP) || isKeyDown(DOWN))
	{
		firstSelected = !firstSelected;
		LCDText_Clear();
	}

	if(isKeyDown(CONFIRM))
	{
		if(firstSelected)
			currentTemperatureMode = CELSIUS;
		else
			currentTemperatureMode = FAHRENHEIT;

		//TODO: write on flash
		memcpy(flashMemory, sector_start_address, 4096);
		((PFlashMemory)flashMemory)->tempMode = currentTemperatureMode;

		FLASH_EraseSector(29);
		FLASH_WriteData(sector_start_address, flashMemory, 4096);

		state = maintenanceMode;
		return;
	}

	if(isKeyPressed(LEFT) && isKeyPressed(RIGHT))
		state = maintenanceMode;
}

bool index = 0;
double newMaxTemperature, newMinTemperature;
bool justEntered = true;
void temperatureLimits()
{
	if(justEntered)
	{
		justEntered = false;
		newMaxTemperature = getMaxTemperature();
		newMinTemperature = getMinTemperature();
	}

	if(index) //HIGH
	{
		LCDText_SetCursor(0, 0);
		LCDText_Printf( "MAX" );
		LCDText_SetCursor(1, 0);
		LCDText_Printf("%.1f%s", newMaxTemperature, temperatureModeNames[currentTemperatureMode]);
	} else
	{
		LCDText_SetCursor(0, 0);
		LCDText_Printf( "MIN" );
		LCDText_SetCursor(1, 0);
		LCDText_Printf("%.1f%s", newMinTemperature, temperatureModeNames[currentTemperatureMode]);
	}

	if(isKeyDown(LEFT) || isKeyDown(RIGHT))
		index = !index;

	if(isKeyUp(UP))
	{
		if(index) newMaxTemperature += 0.1f;
		else newMinTemperature += 0.1f;
	} else if(isKeyUp(DOWN))
	{
		if(index) newMaxTemperature -= 0.1f;
		else newMinTemperature -= 0.1f;
	}

	if(isKeyDown(CONFIRM))
	{
		justEntered = true;
		state = maintenanceMode;
	}

	if(isKeyPressed(LEFT) && isKeyPressed(RIGHT))
	{
		justEntered = true;
		state = maintenanceMode;

		//TODO: save new temps on flash
		maxTemperature = newMaxTemperature;
		minTemperature = newMinTemperature;
	}

}

int jumpTable[] = {9, 4, 1, 12, 15 };
int indexDaT = 0;
struct tm newTime = {};

void clampDate(struct tm* time)
{
	if(time->tm_year < 0) time->tm_year = 0;

	if(time->tm_mon < 1) time->tm_mon = 1;
	else if(time->tm_mon > 12) time->tm_mon = 12;

	if(time->tm_mday < 1) time->tm_mday = 1;
	else if(time->tm_mday > 31) time->tm_mday = 31;

	// month has 31 days
	if(time->tm_mday == 31 && ((time->tm_mon % 3) != 0))
		time->tm_mday = 30;

	//February 				28+
	if(time->tm_mon == 2 && time->tm_mday > 28)
	{
		//if not leap year
		if(((time->tm_year % 4) != 0) )
			time->tm_mday = 28;
		else
			time->tm_mday = 29;
	}

	if(time->tm_hour < 0) time->tm_hour = 0;
	else if(time->tm_hour > 23) time->tm_hour = 23;

	if(time->tm_min < 0) time->tm_min = 0;
	else if(time->tm_min > 59) time->tm_min = 59;
}

bool canGoToNextField = true;
void dateAndTime()
{
	static bool justEntered = true;
	if(justEntered)
	{
		indexDaT = 0;
		justEntered = false;
		RTC_GetValue(&newTime);
	}

	LCDText_SetCursor(0, 0);
	//20:07:2020 17:56
	LCDText_Printf("%02hu:%02hu:%04hu %02hu:%02hu", newTime.tm_mday, newTime.tm_mon, newTime.tm_year, newTime.tm_hour, newTime.tm_min );
	LCDText_Printf("                ");
	LCDText_SetCursor(1, jumpTable[indexDaT]);
	LCDText_Printf("^");

	int* currValue = (int*)&newTime + (5 - indexDaT);

	if(isKeyDown(UP))
	{
		*currValue = (*currValue) + 1;
		clampDate(&newTime);
	}
	else if(isKeyDown(DOWN))
	{
		*currValue = (*currValue) - 1;
		clampDate(&newTime);
	}

	if(!isKeyPressed(LEFT) || !isKeyPressed(RIGHT))
		canGoToNextField = true;

	if(isKeyDown(CONFIRM))
		state = maintenanceMode;

	if(isKeyPressed(LEFT) && isKeyPressed(RIGHT) && canGoToNextField)
	{
		indexDaT++;
		canGoToNextField = false;

		if(indexDaT == 5)
		{
			justEntered = true;
			RTC_SetValue(&newTime);
			state = maintenanceMode;
		}
	}
}

int alarmIndex = 0;
int alarmCount = 0;
PAlarm alarms;
void showAlarms()
{
	static bool justEntered = true;
	if(justEntered)
	{
		justEntered = false;
		memcpy(&alarmCount, sector_start_address + sizeof(int) + sizeof(CELSIUS), sizeof(int));

		//Copy from flash
		int sizeToCopy = sizeof(FlashMemory) + sizeof(Alarm) * (alarmCount+1);
		memcpy(flashMemory, sector_start_address, sizeToCopy);

		alarms = (PAlarm)(flashMemory + sizeof(FlashMemory));

		if(alarmCount <= 0)
			LCDText_Printf("There are no alarms!");
	}

	if(alarmCount > 0)
	{
		Alarm alarm = alarms[alarmIndex];
		struct tm time = alarm.time;

		LCDText_SetCursor(0, 0);
		LCDText_Printf("%02hu:%02hu:%04hu %02hu:%02hu", time.tm_mday, time.tm_mon, time.tm_year, time.tm_hour, time.tm_min );

		LCDText_SetCursor(1, 0);
		if(alarm.status == HIGH_TEMP)
			LCDText_Printf("HIGH");
		else
			LCDText_Printf("LOW");

		if(isKeyDown(UP))
		{
			alarmIndex--;
			if(alarmIndex < 0) alarmIndex = 0;
			LCDText_Clear();
		}
		else if(isKeyDown(DOWN))
		{
			alarmIndex++;
			if(alarmIndex == alarmCount) alarmIndex = alarmCount - 1;
			LCDText_Clear();
		}
	}

	if(isKeyDown(CONFIRM))
	{
		justEntered = true;
		state = maintenanceMode;
	}
}

void handleMenu()
{
	if(isKeyDown(UP))
	{
		indexMenu--;
		if(indexMenu < 0) indexMenu = menuCount - 1;
		menuChanged = true;
	}
	else if(isKeyDown(DOWN))
	{
		indexMenu++;
		if(indexMenu == menuCount) indexMenu = 0;
		menuChanged = true;
	}

	int secondIndex = indexMenu + 1;
	if(secondIndex == 4) secondIndex = 0;

	if(menuChanged)
	{
		LCDText_Clear();
		LCDText_SetCursor(0, 0);
		LCDText_Printf("%s <", (char*)menuItems[indexMenu]);
		LCDText_SetCursor(1, 0);
		LCDText_Printf("%-16s", (char*)menuItems[secondIndex]);
		menuChanged = false;
	}

}

bool canLeave = false;
void maintenanceMode()
{
	if(isKeyUp(LEFT) || isKeyUp(RIGHT) || isKeyUp(CONFIRM))
		canLeave = true;

	//go back to normal mode
	if(isKeyPressed(LEFT) && isKeyPressed(RIGHT) && canLeave)
	{
		canLeave = false;
		indexMenu = 0;
		state = normalMode;
	}

	if(isKeyDown(CONFIRM))
	{
		LCDText_Clear();
		canLeave = false;
		state = menuFunctions[indexMenu];
	}

	handleMenu();

}

void init()
{
	LCDText_Init();
	WAIT_ChronosInit();
    WAIT_init();
	RTC_Init(0);
	BMP_Init();
	FLASH_Init();
	KEYPAD_Init(layout);

	//First time initialisation
	unsigned int result = FLASH_VerifyData(sector_start_address, &magicWord, sizeof(int));
	if(result != 0)
	{
		FLASH_EraseSector(29);

		initialFlashMemory.magicWord = magicWord;
		initialFlashMemory.tempMode = CELSIUS;
		initialFlashMemory.alarmCount = 0;

		FLASH_WriteData(sector_start_address, &initialFlashMemory, 256);
	}

	memcpy(&currentTemperatureMode, sector_start_address + 0x4, sizeof(enum temperatureMode));
	showScreenCurrTimer = showScreenTimer + WAIT_GetElapsedMillis(0);

	state = normalMode;
}

int main(void)
{
	state = init;
	state_fn* lastState = state;
	while(state)
	{
		if(lastState != state)
		{
			LCDText_Clear();
			LCDText_SetCursor(0, 0);
			lastState = state;
			menuChanged = true;
		}
		state();
		updateKeys();
	}

	return 0;
}

