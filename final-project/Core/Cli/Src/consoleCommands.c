

// ConsoleCommands.c
// This is where you add commands:
//		1. Add a protoype
//			static eCommandResult_T ConsoleCommandVer(const char buffer[]);
//		2. Add the command to mConsoleCommandTable
//		    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
//		3. Implement the function, using ConsoleReceiveParam<Type> to get the parameters from the buffer.

#include <string.h>
#include <stdlib.h>
#include "consoleCommands.h"
#include "console.h"
#include "consoleIo.h"
#include "version.h"
#include "mpu6050.h"
#include "main.h"
#include "stdio.h"
#include "lc709203.h"
#include "stringHelpers.h"
#include "orientation.h"
#include "ledController.h"
#include "systemConfig.h"
#include "ws2812.h"
#include "spi.h"

#include "Tasks.h"


//ToDo: Make this configurable
//TODO: Make module for RTC control
extern RTC_HandleTypeDef hrtc;
extern ledColours_t ledColours;

uint8_t uninitGlobalVar;
uint8_t initGlobalVar = 'A';
const uint8_t constVar;
static uint8_t staticVar;

#define IGNORE_UNUSED_VARIABLE(x)     if ( &x == &x ) {}


static eCommandResult_T ConsoleCommandVer(const char buffer[]);
static eCommandResult_T ConsoleCommandHelp(const char buffer[]);

static eCommandResult_T ConsoleCommandTimeQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandDateQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandDateSet(const char buffer[]);
static eCommandResult_T ConsoleCommandTimeSet(const char buffer[]);
static eCommandResult_T ConsoleCommandAccelQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandLipoQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandCPUQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandMemTest(const char buffer[]);
static eCommandResult_T ConsoleCommandLEDSet(const char buffer[]);
static eCommandResult_T ConsoleCommandFaceUpQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandTaskSet(const char buffer[]);
static eCommandResult_T ConsoleCommandTaskQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandSaveConfig(const char buffer[]);
static eCommandResult_T ConsoleCommandDodecaQuery(const char buffer[]);
static eCommandResult_T ConsoleCommandDodecaSet(const char buffer[]);

static void displayDodeca(uint8_t id);
static void displayTask(uint8_t id);


static const sConsoleCommandTable_T mConsoleCommandTable[] =
{

    {"help", &ConsoleCommandHelp, HELP("Lists the commands available")},
    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
	{"time?", &ConsoleCommandTimeQuery, HELP("Get the current time")},
	{"time", &ConsoleCommandTimeSet, HELP("Set the current time (HH:MM:SS)")},
	{"date?", &ConsoleCommandDateQuery, HELP("Get the current date")},
	{"date", &ConsoleCommandDateSet, HELP("Set the current date (DD-MM-YY)")},
	{"acc?", &ConsoleCommandAccelQuery, HELP("Get Accelerometer Data [r n - Read n times]")},
	{"lipo?", &ConsoleCommandLipoQuery, HELP("Get Info on Lipo Battery ")},
	{"cpu?", &ConsoleCommandCPUQuery, HELP("Get Info on The CPU")},
	{"mem?", &ConsoleCommandMemTest, HELP("Test the SPI memory")},
	{"led", &ConsoleCommandLEDSet, HELP("Set a face to a colour")},
	{"faceup?", &ConsoleCommandFaceUpQuery, HELP("Detect Face Up")},
	{"task?", &ConsoleCommandTaskQuery, HELP("Display Task(s), No Param = list all, param = list specific")},
	{"task", &ConsoleCommandTaskSet, HELP("Update Task \n\t n- name\n\tm - max duration\n\ti-min duration")},
	{"save", &ConsoleCommandSaveConfig, HELP("Saves the current config to Flash")},
	{"dodeca?", &ConsoleCommandDodecaQuery, HELP("Displays Dodeca Task, No Param = list all, param = list specific")},
	{"dodeca", &ConsoleCommandDodecaSet, HELP("Set Dodeca Task, t-set task, m-set Max, i-set min")},


	CONSOLE_COMMAND_TABLE_END // must be LAST
};


static void displayTask(uint8_t id)
{
	char msg[120];
	char colourName[10];
	taskItem_t *task = taskGet(id);
	ledFindColour(task->colour,colourName);
	sprintf(msg,"Task: %d - %s\n\tColour: %s %#08x\n\tDefault Min Time: %i\n\tDefault Max Time: %i\n",id,task->name,colourName,(unsigned int)task->colour,task->defaultMinTime,task->defaultMaxTime);
	ConsoleSendLine(msg);
}

static void displayDodeca(uint8_t id)
{
	char msg[120];
	char statename[20];
	dodecaItem_t *dodeca = 0;
	taskItem_t *task;
	dodeca = dodecaGet(id);
	task = taskGet(dodeca->taskId);

	dodecaGetStateName(dodeca->state,statename);
	sprintf(msg,"Dodeca: %i - %s\n\t \n\tState: %s\n\tMin Time: %d\n\tMax Time: %d",id,task->name,statename,dodeca->minTimeMins,dodeca->maxTimeMins);
	ConsoleSendLine(msg);
}

static eCommandResult_T ConsoleCommandDodecaSet(const char buffer[])
{
	// get the Dodeca Id to set
	int16_t dodecaId;
	int16_t value;
	MPU6050_t data;
	dodecaItem_t *dodeca;
	taskItem_t *task;
	char msg[50];

		// get the command
		uint32_t cmdIndex;
		if (COMMAND_SUCCESS != ConsoleParamFindN(buffer, 1, &cmdIndex))
		{
			ConsoleSendLine("Please supply a command");
			return COMMAND_PARAMETER_ERROR;
		}

		MPU6050ReadStable(&data);
		dodecaId = detectFace(data.KalmanAngleX, data.KalmanAngleY);


		if (dodecaId< 0)
		{
			ConsoleSendLine("Face not detected");
			return COMMAND_ERROR;
		}
		sprintf(msg,"Detected face: %i",dodecaId);
		ConsoleSendLine(msg);

		dodeca = dodecaGet(dodecaId);

		switch (buffer[cmdIndex])
		{
		case 't' :
				// Sets a task to the dodeca current facing up
				if (DODECA_STATE_ACTIVE == dodeca->state)
				{
					ConsoleSendLine("Cannot change an Active Task");
					return COMMAND_SUCCESS;
				}


				// Set the Task Id
				if (COMMAND_SUCCESS != ConsoleReceiveParamInt16(buffer, 2, &value))
				{
					return COMMAND_PARAMETER_ERROR;
				}

				if (value > TASK_COUNT_MAX)
				{
					ConsoleSendLine("Invalid Task Id");
					return COMMAND_PARAMETER_ERROR;
				}

				task = taskGet(value);

				dodecaInitItem(dodecaId,task->defaultMinTime,task->defaultMaxTime,value);
			break;

		case 'm':
				ConsoleReceiveParamInt16(buffer, 2, &value);
				if (value <0)
				{
					ConsoleSendLine("Invalid Max time");
					return COMMAND_PARAMETER_ERROR;
				}
				dodeca->maxTimeMins = value;
			break;
		case 'i':
				ConsoleReceiveParamInt16(buffer, 2, &value);
				if (value <0)
				{
					ConsoleSendLine("Invalid Min time");
					return COMMAND_PARAMETER_ERROR;
				}
				dodeca->minTimeMins = value;
			break;
		}
		// Show the current face info
		displayDodeca(dodecaId);
		return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommandDodecaQuery(const char buffer[])
{
	uint32_t param1;

		if (COMMAND_SUCCESS != ConsoleParamFindN(buffer, 1, &param1))
		{
			// List all tasks
			for (uint8_t f=0;f<DODECA_COUNT_MAX;f++)
			{
				displayDodeca(f);
			}
		}
		else
		{
			int16_t taskId;
			ConsoleReceiveParamInt16(buffer, 1, &taskId );

			if (taskId < 0 || taskId > DODECA_COUNT_MAX-1)
			{
				ConsoleSendLine("Invalid Task number");
				return COMMAND_PARAMETER_ERROR;
			}

			displayDodeca(taskId);

		}
		return COMMAND_SUCCESS;
}


static eCommandResult_T ConsoleCommandSaveConfig(const char buffer[])
{
	sysConfigSave();
	ConsoleSendLine("Config Saved!");
	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommandTaskSet(const char buffer[])
{

	// get the task Id to set
	int16_t taskId;
	char name[TASK_NAME_LENGHTH_MAX];
	if (COMMAND_SUCCESS != ConsoleReceiveParamInt16(buffer, 1, &taskId))
	{
		return COMMAND_PARAMETER_ERROR;
	}

	if (taskId > TASK_COUNT_MAX)
	{
		ConsoleSendLine("Invalid Task ID");
		return COMMAND_PARAMETER_ERROR;
	}

	// get the command
	uint32_t cmdIndex;
	if (COMMAND_SUCCESS != ConsoleParamFindN(buffer, 2, &cmdIndex))
	{
		ConsoleSendLine("Please supply a command");
		return COMMAND_PARAMETER_ERROR;
	}

	taskItem_t *task;
	int16_t colour;
	int16_t minmax;
	task = taskGet(taskId);

	switch (buffer[cmdIndex])
	{
	case 'n':
			// Get the task Name
			ConsoleReceiveParamString(buffer, 3, name,TASK_NAME_LENGHTH_MAX );
			// Check Length
			if (strlen(name)< TASK_NAME_LENGHTH_MIN)
			{
				ConsoleSendLine("Name too short");
				return COMMAND_PARAMETER_ERROR;
			}
			strcpy(task->name,name);
			break;
	case 'c':
			// get the colour number

			ConsoleReceiveParamInt16(buffer, 3, &colour);
			if (colour > FACE_COUNT)
			{
				ConsoleSendLine("Invalid Colour");
				return COMMAND_PARAMETER_ERROR;
			}
			task->colour = ledColours.colour[colour].code;
			ConsoleSendString("Colour set : ");
			ConsoleSendLine(ledColours.colour[colour].name);

		break;
	case 'm':
			ConsoleReceiveParamInt16(buffer, 3, &minmax);
			if (minmax <0)
			{
				ConsoleSendLine("Invalid Max time");
				return COMMAND_PARAMETER_ERROR;
			}
			task->defaultMaxTime = minmax;
		break;
	case 'i':
			ConsoleReceiveParamInt16(buffer, 3, &minmax);
			if (minmax <0)
			{
				ConsoleSendLine("Invalid Min time");
				return COMMAND_PARAMETER_ERROR;
			}
			task->defaultMinTime = minmax;
		break;
	}


	// Show the current face info
	displayTask(taskId);
	return COMMAND_SUCCESS;
}
static eCommandResult_T ConsoleCommandTaskQuery(const char buffer[])
{
	uint32_t param1;

	if (COMMAND_SUCCESS != ConsoleParamFindN(buffer, 1, &param1))
	{
		// List all tasks
		for (uint8_t f=0;f<TASK_COUNT_MAX;f++)
		{
			displayTask(f);
		}
	}
	else
	{
		int16_t taskId;
		ConsoleReceiveParamInt16(buffer, 1, &taskId );

		if (taskId < 0 || taskId > TASK_COUNT_MAX-1)
		{
			ConsoleSendLine("Invalid Task number");
			return COMMAND_PARAMETER_ERROR;
		}

		displayTask(taskId);

	}
	return COMMAND_SUCCESS;
}


static eCommandResult_T ConsoleCommandFaceUpQuery(const char buffer[])
{
	IGNORE_UNUSED_VARIABLE(buffer);

	uint8_t face;
	char msg[30];

	MPU6050_t data;
	MPU6050ReadStable(&data);


	sprintf(msg,"Angle X: %f Y: %f",data.KalmanAngleX, data.KalmanAngleY);
	ConsoleSendLine(msg);

	face = detectFace(data.KalmanAngleX, data.KalmanAngleY);

	uint32_t rgb_color = hsl_to_rgb((face*30), 255, 127);

	ledAllOff();
	ledSetFaceColour(face, (rgb_color >> 16) & 0xFF, (rgb_color >> 8) & 0xFF, rgb_color & 0xFF);
	ledRender();

	sprintf(msg,"Detected face %i is up",face);
	ConsoleSendLine(msg);

	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommandLEDSet(const char buffer[])
{
	uint32_t startIndex;
	eCommandResult_T result;
	int16_t faceNumber;
	result = ConsoleParamFindN(buffer,1,&startIndex);

	if (result != COMMAND_SUCCESS)
		return result;

	uint8_t command = buffer[startIndex];
	switch (command)
	{
		case 'f':

			result = ConsoleReceiveParamInt16(buffer, 2, &faceNumber);
			if (result != COMMAND_SUCCESS)
			{
				ConsoleSendLine("Invalid Face ID");
				return COMMAND_PARAMETER_ERROR;
			}

			if (faceNumber >= PIXEL_RINGS)
			{
				ConsoleSendLine("Invalid Face ID - Must be Zero Index");
				return COMMAND_PARAMETER_ERROR;
			}


			result = ConsoleParamFindN(buffer,3,&startIndex);
			uint8_t colour = buffer[startIndex];
			if ('r' == colour)
				ledSetFaceColour(faceNumber,0xAA,00,0);
			else if ('g' == colour)
				ledSetFaceColour(faceNumber,0x0,0xAA,0);
			else if ('b' == colour)
				ledSetFaceColour(faceNumber,0x0,0x0,0xAA);

			ledRender();

			break;
		case 'o':
			ledAllOff();
			ledRender();
			break;
		case'd':
				ledDance();
			break;
		default :
			return CONSOLE_ERROR;
			break;
	}


	return CONSOLE_SUCCESS;

}

static eCommandResult_T ConsoleCommandMemTest(const char buffer[])
{
//	char msg[50];
//	uint32_t Temp;
//	uint8_t temp0 = 0, temp1 = 0, temp2 = 0;
//	uint8_t reg = 0x4B;
//	uint8_t id[8];
//	HAL_GPIO_WritePin(SPI_MEM_CS_GPIO_Port, SPI_MEM_CS_Pin, 0);
//
//	HAL_SPI_TransmitReceive(&hspi1, &reg, &temp0, 1, 100);
//	for (uint8_t i = 0; i < 4; i++)
//		HAL_SPI_TransmitReceive(&hspi1, &reg, &temp0, 1, 100);
//
//	for (uint8_t i = 0; i < 8; i++)
//		HAL_SPI_TransmitReceive(&hspi1, &reg, &id[i], 1, 100);
//
//
//	HAL_GPIO_WritePin(SPI_MEM_CS_GPIO_Port, SPI_MEM_CS_Pin, 1);
//	Temp = (temp0 << 16) | (temp1 << 8) | temp2;
//	sprintf(msg,"Flash Mem Id \t%#02x %#02x %#02x %#02x %#02x %#02x %#02x %#02x ",id[0],id[1],id[2],id[3],id[4],id[5],id[6],id[7]);
//	ConsoleSendLine(msg);
	ConsoleSendLine("Commented out");
	return CONSOLE_SUCCESS;
}

static eCommandResult_T ConsoleCommandCPUQuery(const char buffer[])
{
	uint8_t funcVar;

	char msg[30];
	register int SP __asm("r13");


	sprintf(msg,"-- CPU Details --");
	ConsoleSendLine(msg);

	sprintf(msg,"Stack Pointer \t\t%#08x",SP);
	ConsoleSendLine(msg);

	unsigned int* HP = malloc(1);
	*HP=0xAB;

	sprintf(msg,"Heap Pointer \t\t%#08x",(unsigned int)HP-1);
	ConsoleSendLine(msg);
	free(HP);

//	sprintf(msg,"Initialised global variable \t\t%#08x",(unsigned int)(&initGlobalVar));
//	ConsoleSendLine(msg);

//	sprintf(msg,"Un-initialised global variable \t\t%#08x",(unsigned int)&uninitGlobalVar);
//	ConsoleSendLine(msg);

	sprintf(msg,"Const variable \t\t%#08x",(unsigned int)&constVar);
		ConsoleSendLine(msg);

	sprintf(msg,"Static variable \t\t%#08x",(unsigned int)&staticVar);
	ConsoleSendLine(msg);

	sprintf(msg,"Function variable \t\t%#08x",(unsigned int)&funcVar);
	ConsoleSendLine(msg);

	return CONSOLE_SUCCESS;
}

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


static eCommandResult_T ConsoleCommandLipoQuery(const char buffer[])
{
	uint16_t voltage;
	uint16_t temp;
	uint16_t version;
	char msg[50];
	lc709203_getCellVoltage(&voltage);
	lc709203_getCellTemp(&temp);
	lc709203_getId(&version);

	sprintf(msg,"-- Lipo Cell Details --");
	ConsoleSendLine(msg);

	sprintf(msg,"Monitor Version: %d",version);
	ConsoleSendLine(msg);

	sprintf(msg,"Cell Voltage: %1.4f v",((double)voltage/1000));
	ConsoleSendLine(msg);

	sprintf(msg,"Cell Temp: %1.2f v",((double)temp/100));
	ConsoleSendLine(msg);



	return COMMAND_SUCCESS;

}


/***********************************************************
 * ConsoleCommandAccelQuery
 * Get the current values for the Accelerometer
 * @param: Buffer
 * @return: eCommandResult_T
 *
 ***********************************************************/
static eCommandResult_T ConsoleCommandAccelQuery(const char buffer[])
{
	int16_t n;
	MPU6050_t data;
	char msg[100];
	uint32_t paramIndex;
	float rawAveX = 0;
	float rawAveY = 0;
	float rawAveZ = 0;
	ConsoleParamFindN(buffer, 1 , &paramIndex);

	if (0 == paramIndex)
	{
		MPU6050_Read_All(&I2C_MPU6050, &data);
		sprintf(msg,"X Angle\t: %f \t\t Y Angle\t: %f\n",data.KalmanAngleX, data.KalmanAngleY);
		ConsoleSendLine(msg);
		return COMMAND_SUCCESS;
	}


	// Read Raw n Times (Param r n where n = number of reads
	switch (buffer[paramIndex])
	{
		default:
				return COMMAND_PARAMETER_ERROR;
		case 'r':
				ConsoleReceiveParamInt16(buffer, 2, &n);
				for (uint8_t cnt = 0;cnt<n;cnt++)
				{
					MPU6050_Read_Accel(&I2C_MPU6050, &data);
					rawAveX += data.Accel_X_RAW;
					rawAveY += data.Accel_Y_RAW;
					rawAveZ += data.Accel_Z_RAW;
					sprintf(msg,"RAW:[X:%5i Y:%5i Z:5%i] G:[X:%2f Y:%2f Z:%2f]",data.Accel_X_RAW, data.Accel_Y_RAW,data.Accel_Z_RAW, data.Ax,data.Ay,data.Az);
					ConsoleSendLine(msg);
				}
				rawAveX = rawAveX/n;
				rawAveY = rawAveY/n;
				rawAveZ = rawAveZ/n;
				sprintf(msg,"\nRAW Average :[X:%5f Y:%5f Z:5%f]\n",rawAveX,rawAveY,rawAveZ);
				ConsoleSendLine(msg);
				break;
		case 'a':
				ConsoleReceiveParamInt16(buffer, 2, &n);
				for (uint8_t cnt = 0;cnt<n;cnt++)
				{
					MPU6050_Read_All(&I2C_MPU6050, &data);
					sprintf(msg,"A:[X:%2f Y:%2f]",data.KalmanAngleX, data.KalmanAngleY);
					ConsoleSendLine(msg);

				}
				break;
		case 'k':
				ConsoleSendLine("--* Kalman Angle *--");
				for (uint8_t cnt = 0;cnt<50;cnt++)
				{
					MPU6050_Read_All(&I2C_MPU6050, &data);
				}
				sprintf(msg,"[X:%2f Y:%2f]\n",data.KalmanAngleX, data.KalmanAngleY);
				ConsoleSendLine(msg);

			break;
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


