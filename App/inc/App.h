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
double minTemperature 	= 10.0f;
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
