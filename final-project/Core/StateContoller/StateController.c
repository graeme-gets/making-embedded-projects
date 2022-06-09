/*
 * StateController.c
 *
 *  Created on: Jun 7, 2022
 *      Author: GraemeGets
 */

#include "main.h"
#include "StateController.h"
#include "tim.h"
#include "mpu6050.h"
#include "dodeca.h"
#include "orientation.h"
#include "console.h"
#include "ledController.h"


stateControl_t stateTable[] = {
//											New Face Detect		Lipo Int	, 		RTC Int				Sleep Timeout, 		Done				Error
		{STATE_CONFIG, 		"Config",		STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_NULL,			STATE_ERROR},
		{STATE_IDLE, 		"Idle",			STATE_NULL,			STATE_BATTERY_TEST,	STATE_UPDATE_TASK, 	STATE_CHECK_OREN,	STATE_SLEEP,		STATE_ERROR},
		{STATE_CHECK_OREN, 	"Orientation",	STATE_CHANGE_TASK,	STATE_NULL,			STATE_NULL,  		STATE_NULL,			STATE_SLEEP,		STATE_ERROR},
		{STATE_CHANGE_TASK, "Change Task",	STATE_NULL,			STATE_NULL,			STATE_NULL,  		STATE_NULL,			STATE_SLEEP,		STATE_ERROR},
		{STATE_SLEEP, 		"Sleep",		STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_IDLE,			STATE_CHECK_OREN,	STATE_ERROR},
		{STATE_END_TASK, 	"End Task",		STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_NULL,			STATE_ERROR},
		{STATE_START_TASK, 	"Start Task",	STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_IDLE,			STATE_ERROR},
		{STATE_UPDATE_TASK, "Update Task",	STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_NULL,			STATE_ERROR},
		{STATE_BATTERY_TEST,"Battery Test",	STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_NULL,			STATE_ERROR},
		{STATE_ERROR,		"**ERROR**",	STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_IDLE,			STATE_ERROR},
		{STATE_BEGIN,		"BEGIN",		STATE_IDLE	,		STATE_IDLE	, 		STATE_IDLE, 		STATE_IDLE,			STATE_IDLE,			STATE_ERROR},
		// This must be the last Entry!
		{STATE_NULL,		"NONE!",		STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_NULL,			STATE_ERROR},

};

uint8_t stateActionRegister  = 0;
stateControl_t  *currentState;
dodecaItem_t *newDodecaDetected;
uint8_t detectedFace;
eStateControllerMode_t stateControllerMode;

static dodecaItem_t *currentDodeca;
uint8_t currentFaceUp;
static eState_t oldState;
static stateControl_t *findState(const eState_t state);
static void displayState();

static stateControl_t *findState(const eState_t state)
{
	eState_t stateLoop = 0xff;
	uint8_t cnt = 0;
	while (STATE_NULL != stateLoop)
	{
		if (state == stateTable[cnt].state)
			return &stateTable[cnt];
		cnt++;
	}
	return &stateTable[cnt];
}

void setStateControllerMode(eStateControllerMode_t mode)
{
	stateControllerMode = mode;
}


void stateContollerInit(eState_t state)
{
	if (state == STATE_CONFIG)
	{
		stateControllerMode = STATE_CONT_MODE_CONFIG;
	}else
	{
		stateControllerMode  = STATE_CONT_MODE_RUN;
	}
	currentDodeca = dodecaGet(DODECA_STOP_FACE);
	currentDodeca->state = DODECA_STATE_STOPPED;
	currentState = findState(state);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim9)
	{

	}
}





static void displayState()
{
	if (oldState == currentState->state) return;
	oldState = currentState->state;
	ConsoleSendString("State :-> ");
	ConsoleSendLine(currentState->debugName);
}


void stateController()
{



	switch (currentState->state)
	{
	case STATE_BEGIN:
			displayState();
			if (STATE_CONT_MODE_CONFIG ==  stateControllerMode)
			{
				currentState = findState(STATE_CONFIG);
				break;
			}
			currentFaceUp = detectFaceUp();
			if (0xff == currentFaceUp )
			{
				// TODO Set Error reason
				currentState = findState(currentState->error);
				break;
			}
			currentDodeca = dodecaGet(currentFaceUp);
			if (DODECA_STATE_NOT_CONFIGURED == currentDodeca->state)
			{
				// TODO Set Error reason
				currentState = findState(currentState->error);
			}
			else 				{
				currentDodeca = dodecaGet(currentFaceUp);
				currentDodeca->state = DODECA_STATE_STOPPED;
				currentState = findState(currentState->done);
				ledSetFaceColour(currentDodeca->id, colourFindByid(COLOUR_RED_ID)->code,LED_FACE_MODE_NORMAL);
				ledRender();
			}

			break;

	case STATE_CONFIG:
		displayState();
		if (STATE_CONT_MODE_RUN ==  stateControllerMode)
		{
			currentState = findState(STATE_IDLE);
			break;
		}
		break;
	case STATE_IDLE:
			displayState();
			if (STATE_CONT_MODE_CONFIG ==  stateControllerMode)
				currentState = findState(STATE_CONFIG);
			else
				currentState = findState(currentState->done);
		break;
	case STATE_CHECK_OREN:
		displayState();
			detectedFace = detectFaceUp();
			if (detectedFace == 0xff)
			{
				ConsoleSendLine("ERROR - Face not detected");
				currentState =  findState(currentState->error);
				break;
			}
			newDodecaDetected = dodecaGet(detectedFace);
			if (detectedFace == currentDodeca->id) // If its the same face then exit
			{
				currentState =  findState(currentState->done);

			}

			else if (newDodecaDetected->state == DODECA_STATE_NOT_CONFIGURED || DODECA_STATE_ERROR == newDodecaDetected->state)
			{

				ledSetFaceColour(newDodecaDetected->id,newDodecaDetected->colour , LED_FACE_MODE_ERROR);
				ledRender();

				if (DODECA_STATE_ACTIVE == currentDodeca->state)
				{
					dodecaStop(currentDodeca->id);
					currentDodeca = newDodecaDetected;
				}
				currentState = findState(currentState->error);

			}

			else
			{
				currentState = findState(currentState->newFaceDetect);
			}
		break;
	case STATE_CHANGE_TASK: // Chnage Task only happens if there is a Valid new face
		displayState();

		if (currentDodeca->state == DODECA_STATE_ACTIVE)
		{
			dodecaStop(currentDodeca->id);
			currentDodeca = newDodecaDetected;
			dodecaStart(currentDodeca->id);
		}
		else if (currentDodeca->state == DODECA_STATE_NOT_CONFIGURED)
		{
			ledSetFaceColour(currentDodeca->id, colourFindByid(COLOUR_BLACK_ID)->code, LED_FACE_MODE_NORMAL);
			ledRender();
			currentDodeca = newDodecaDetected;
			dodecaStart(currentDodeca->id);
		}


		if (currentDodeca->id != DODECA_STOP_FACE)
		{
			dodecaStart(currentDodeca->id);
		}

		currentState = findState(currentState->done);

		break;
	case STATE_UPDATE_TASK:
			displayState();
			break;
	case STATE_ERROR:
			displayState();
			ledSetFaceColour(currentDodeca->id,currentDodeca->colour,LED_FACE_MODE_ERROR);
			ledRender();
			currentState = findState(currentState->done);
			break;
	case STATE_BATTERY_TEST:
			displayState();
		break;
	case STATE_START_TASK:
			displayState();
		break;
	case STATE_END_TASK:
			displayState();
		break;
	case STATE_SLEEP:
			displayState();
			// TODO : Goto Sleep
			if (STATE_CONT_MODE_CONFIG ==  stateControllerMode)
				currentState = findState(STATE_CONFIG);
			else
				currentState = findState(currentState->done);
		break;
	case STATE_NULL:
		displayState();
		break;
	}


}
