

// ConsoleCommands.c
// This is where you add commands:
//		1. Add a protoype
//			static eCommandResult_T ConsoleCommandVer(const char buffer[]);
//		2. Add the command to mConsoleCommandTable
//		    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
//		3. Implement the function, using ConsoleReceiveParam<Type> to get the parameters from the buffer.

#include <string.h>
#include "consoleCommands.h"
#include "console.h"
#include "consoleIo.h"
#include "version.h"
#include "gy521.h"
#include "main.h"
#include "stdio.h"

//ToDo: Make this configurable
//TODO: Make module for RTC control
extern RTC_HandleTypeDef hrtc;



#define IGNORE_UNUSED_VARIABLE(x)     if ( &x == &x ) {}


static eCommandResult_T ConsoleCommandVer(const char buffer[]);
static eCommandResult_T ConsoleCommandHelp(const char buffer[]);
static eCommandResult_T ConsoleCommandLedToggle(const char buffer[]);
static eCommandResult_T ConsoleCommandLedQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandGyroQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandTimeQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandDateQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandDateSet(const char buffer[]);
static eCommandResult_T ConsoleCommandTimeSet(const char buffer[]);



static const sConsoleCommandTable_T mConsoleCommandTable[] =
{

    {"help", &ConsoleCommandHelp, HELP("Lists the commands available")},
    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
	{"led", &ConsoleCommandLedToggle, HELP("Turns the Blue LED on or off (1==ON; 0=OFF")},
	{"led?", &ConsoleCommandLedQuery, HELP("Get the state of the LED")},
	{"gyro?", &ConsoleCommandGyroQuery, HELP("Get the state of the GYRO")},
	{"time?", &ConsoleCommandTimeQuery, HELP("Get the current time")},
	{"time", &ConsoleCommandTimeSet, HELP("Set the current time (HH:MM:SS)")},
	{"date?", &ConsoleCommandDateQuery, HELP("Get the current date")},
	{"date", &ConsoleCommandDateSet, HELP("Set the current date (DD-MM-YY)")},
	CONSOLE_COMMAND_TABLE_END // must be LAST
};

/***********************************************************
 * Set RTC Date
 * Paramater format : DD MM YY
 *
 ***********************************************************/
static eCommandResult_T ConsoleCommandDateSet(const char buffer[])
{
	eCommandResult_T result;
	int day;
	int month;
	int year;
	uint32_t startIndex;
	RTC_DateTypeDef sDate;

	result = ConsoleParamFindN(buffer,1,&startIndex);
	if (result != COMMAND_SUCCESS)
		return COMMAND_PARAMETER_ERROR;

	sscanf(&buffer[startIndex],"%d-%d-%d",&day,&month,&year);

	if (!IS_RTC_DATE(day) )
	{
		ConsoleSendLine("** ERROR *** Day value incorrect");
		return COMMAND_PARAMETER_ERROR;
	}



	if (!IS_RTC_MONTH(month) )
	{
		ConsoleSendLine("** ERROR *** Month value incorrect");
		return COMMAND_PARAMETER_ERROR;
	}

	if (!IS_RTC_YEAR(year) )
	{
		ConsoleSendLine("** ERROR *** Year value incorrect");
		return COMMAND_PARAMETER_ERROR;
	}


	sDate.Date = day;
	sDate.Month = month;
	sDate.Year = year;
	sDate.WeekDay = 7;


	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) // Must Set using BIN else Year is incorrect!
	{
		ConsoleSendLine("** ERROR *** Date format incorrect");
		return COMMAND_PARAMETER_ERROR;
	}
	else
	{
		ConsoleSendLine("Date Set");
		return COMMAND_SUCCESS;
	}
}

/***********************************************************
 * Set RTC Time
 * Paramater format : HH:MM:SS
 *
 ***********************************************************/
static eCommandResult_T ConsoleCommandTimeSet(const char buffer[])
{
	eCommandResult_T result;
	int min;
	int hour;
	int sec;
	uint32_t startIndex;
	RTC_TimeTypeDef sTime;

	result = ConsoleParamFindN(buffer,1,&startIndex);
	if (result != COMMAND_SUCCESS)
		return COMMAND_PARAMETER_ERROR;

	sscanf(&buffer[startIndex],"%d:%d:%d",&hour,&min,&sec);

	if (!IS_RTC_MINUTES(min) )
	{
		ConsoleSendLine("** ERROR *** Minute value incorrect");
		return COMMAND_PARAMETER_ERROR;
	}



	if (!IS_RTC_SECONDS(sec) )
	{
		ConsoleSendLine("** ERROR *** Seconds value incorrect");
		return COMMAND_PARAMETER_ERROR;
	}

	if (!IS_RTC_HOUR24(hour) )
	{
		ConsoleSendLine("** ERROR *** Hour value incorrect");
		return COMMAND_PARAMETER_ERROR;
	}


	sTime.Hours = hour;
	sTime.Minutes = min;
	sTime.Seconds = sec;
	//sTime.TimeFormat = RTC_FORMAT_B


	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) // Must Set using BIN else Year is incorrect!
	{
		ConsoleSendLine("** ERROR *** Time format incorrect");
		return COMMAND_PARAMETER_ERROR;
	}
	else
	{
		ConsoleSendLine("Time Set");
		return COMMAND_SUCCESS;
	}
}


static eCommandResult_T ConsoleCommandLedToggle(const char buffer[])
{
	eCommandResult_T result;
	int16_t parameterInt;
	result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
	if (result == COMMAND_PARAMETER_ERROR)
			return result;
	if (0 == parameterInt )
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,1);
	else
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,0);

	return COMMAND_SUCCESS;

}

static eCommandResult_T ConsoleCommandGyroQuery(const char buffer[])
{
	uint8_t state = gy521_ready();
	if (state)
	{
		ConsoleIoSendString("GYRO is Spinning :-)");
	}
	else
	{
			ConsoleIoSendString("GYRO is on holiday :-(");
	}
	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommandTimeQuery(const char buffer[])
{
	char time[30];
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); // There is a bug in HAL where the time is only returned if the date is also read (even after the fact!)
	sprintf(time,"Time: %02d.%02d.%02d\r\n",sTime.Hours,sTime.Minutes,sTime.Seconds);
	ConsoleSendString(time);
	return COMMAND_SUCCESS;
}


static eCommandResult_T ConsoleCommandDateQuery(const char buffer[])
{
	char date[30];
	RTC_DateTypeDef sDate;
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	sprintf(date,"Date: %02d.%02d.%02d\t",sDate.Date,sDate.Month,sDate.Year);
	ConsoleSendString(date);
	return COMMAND_SUCCESS;
}


static eCommandResult_T ConsoleCommandLedQuery(const char buffer[])
{
	if (HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin)==0)
			ConsoleIoSendString("LED is ON ");
	else
		ConsoleIoSendString("LED is OFF ");
	ConsoleIoSendString(STR_ENDLINE);
	return COMMAND_SUCCESS;
}



static eCommandResult_T ConsoleCommandHelp(const char buffer[])
{
	uint32_t i;
	uint32_t tableLength;
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	tableLength = sizeof(mConsoleCommandTable) / sizeof(mConsoleCommandTable[0]);
	for ( i = 0u ; i < tableLength - 1u ; i++ )
	{
		ConsoleIoSendString(mConsoleCommandTable[i].name);
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		ConsoleIoSendString(" : ");
		ConsoleIoSendString(mConsoleCommandTable[i].help);
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}
/*
static eCommandResult_T ConsoleCommandParamExampleInt16(const char buffer[])
{
	int16_t parameterInt;
	eCommandResult_T result;
	result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
	if ( COMMAND_SUCCESS == result )
	{
		ConsoleIoSendString("Parameter is ");
		ConsoleSendParamInt16(parameterInt);
		ConsoleIoSendString(" (0x");
		ConsoleSendParamHexUint16((uint16_t)parameterInt);
		ConsoleIoSendString(")");
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}
static eCommandResult_T ConsoleCommandParamExampleHexUint16(const char buffer[])
{
	uint16_t parameterUint16;
	eCommandResult_T result;
	result = ConsoleReceiveParamHexUint16(buffer, 1, &parameterUint16);
	if ( COMMAND_SUCCESS == result )
	{
		ConsoleIoSendString("Parameter is 0x");
		ConsoleSendParamHexUint16(parameterUint16);
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}
*/

static eCommandResult_T ConsoleCommandVer(const char buffer[])
{
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	ConsoleIoSendString(VERSION_STRING);
	ConsoleIoSendString(STR_ENDLINE);
	return result;
}


const sConsoleCommandTable_T* ConsoleCommandsGetTable(void)
{
	return (mConsoleCommandTable);
}


