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
#include "stdio.h"


stateControl_t stateTable[] = {
//											New Face Detect		Lipo Int	, 		RTC Int				Sleep Timeout, 		Done				Error
		{STATE_CONFIG, 		"Config",		STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_NULL,			STATE_ERROR},
		{STATE_IDLE, 		"Idle",			STATE_NULL,			STATE_BATTERY_TEST,	STATE_NULL, 		STATE_CHECK_OREN,	STATE_CHECK_OREN,	STATE_ERROR},
		{STATE_CHECK_OREN, 	"Orientation",	STATE_CHANGE_TASK,	STATE_NULL,			STATE_NULL,  		STATE_NULL,			STATE_SLEEP,		STATE_ERROR},
		{STATE_CHANGE_TASK, "Change Task",	STATE_NULL,			STATE_NULL,			STATE_NULL,  		STATE_NULL,			STATE_SLEEP,		STATE_ERROR},
		{STATE_SLEEP, 		"Sleep",		STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_IDLE,			STATE_IDLE,			STATE_ERROR},
		{STATE_BATTERY_TEST,"Battery Test",	STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_NULL,			STATE_ERROR},
		{STATE_ERROR,		"**ERROR**",	STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_IDLE,			STATE_ERROR},
		{STATE_BEGIN,		"BEGIN",		STATE_IDLE	,		STATE_IDLE	, 		STATE_IDLE, 		STATE_IDLE,			STATE_IDLE,			STATE_ERROR},
		// This must be the last Entry!
		{STATE_NULL,		"NONE!",		STATE_NULL	,		STATE_NULL	, 		STATE_NULL, 		STATE_NULL,			STATE_NULL,			STATE_ERROR},

};


#define STATE_CONTROLLER_ERROR_FACE_DETECT_FAIL 1<<0

#define  breakState(state) {lastState = (state);break;}

uint8_t errorFlags = 0;
eState_t lastState;

uint8_t stateActionRegister  = 0;
stateControl_t  *currentState;
dodecaItem_t *newDodecaDetected;
uint8_t detectedFace;
eStateControllerMode_t stateControllerMode;

static dodecaItem_t *currentDodeca;
uint8_t currentFaceUp;
char msg[70];


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
		lastState = STATE_NULL;
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
		HAL_PWR_DisableSleepOnExit ();
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
			lastState = STATE_BEGIN;
			displayState();
			if (STATE_CONT_MODE_CONFIG ==  stateControllerMode)
			{
				currentState = findState(STATE_CONFIG);
				breakState(STATE_BEGIN);
			}
			currentFaceUp = detectFaceUp();
			if (0xff == currentFaceUp )
			{
				// TODO Set Error reason
				currentState = findState(currentState->error);
				errorFlags |= STATE_CONTROLLER_ERROR_FACE_DETECT_FAIL;
				breakState(STATE_BEGIN);
			}
			currentDodeca = dodecaGet(currentFaceUp);
			if (DODECA_DISABLED == currentDodeca->enabled)
			{
				ledSetFaceColour(currentDodeca->id, currentDodeca->colour,colourFindByid(COLOUR_RED_ID)->code,LED_FACE_MODE_HALF);
				ledRender();
			}
			else
			{
				currentDodeca->state = DODECA_STATE_STOPPED;
				ledSetFaceColour(currentDodeca->id, currentDodeca->colour,colourFindByid(COLOUR_WHITE_ID)->code,LED_FACE_MODE_HALF);

				ledRender();
			}
			currentState = findState(currentState->done);
			breakState(STATE_BEGIN);;

	case STATE_CONFIG:
		displayState();
		if (STATE_CONT_MODE_RUN ==  stateControllerMode)
		{
			if (lastState == STATE_NULL)
				currentState = findState(STATE_BEGIN);
			else
				currentState = findState(STATE_IDLE);
		}
		breakState(STATE_CONFIG);
	case STATE_IDLE:
		lastState = STATE_IDLE;
			displayState();
			if (STATE_CONT_MODE_CONFIG ==  stateControllerMode)
				currentState = findState(STATE_CONFIG);
			else
				currentState = findState(currentState->done);
		breakState(STATE_IDLE);
	case STATE_CHECK_OREN:

			displayState();
			detectedFace = detectFaceUp();
			if (detectedFace == 0xff)
			{
				ConsoleSendLine("ERROR - Face not detected");
				currentState =  findState(currentState->error);
				errorFlags |= STATE_CONTROLLER_ERROR_FACE_DETECT_FAIL;

				breakState(STATE_CHECK_OREN);
			}

			newDodecaDetected = dodecaGet(detectedFace);
			//sprintf(msg,"Current: [%i] %s, New [%i] %s",currentDodeca->id,currentDodeca->name,newDodecaDetected->id,newDodecaDetected->name);
			//ConsoleSendLine(msg);


			if (newDodecaDetected->id == currentDodeca->id) // If its the same face then exit
			{
				currentState =  findState(currentState->done);

			}
			else
			{
				currentState = findState(currentState->newFaceDetect);
			}
			breakState(STATE_CHECK_OREN);

	case STATE_CHANGE_TASK: // Chnage Task only happens if there is a Valid new face
		displayState();
		currentState = findState(currentState->done);

		// Deal with the Current Dodeca

		if (currentDodeca->state == DODECA_STATE_ACTIVE)
		{
			dodecaStop(currentDodeca->id);
		}
		else if (currentDodeca->state == DODECA_STATE_STOPPED)
		{
			ledSetFaceColour(currentDodeca->id, colourFindByid(COLOUR_BLACK_ID)->code,0x0, LED_FACE_MODE_NORMAL);
			ledRender();
		}

		// Change the current Dodeca for the new Dodeca
		currentDodeca = newDodecaDetected;

		// Now deal with the new DoDeca
		if (DODECA_DISABLED == currentDodeca->enabled)
		{
			currentDodeca->state = DODECA_STATE_STOPPED;
			ledSetFaceColour(currentDodeca->id, currentDodeca->colour, colourFindByid(COLOUR_ERROR_ID)->code, LED_FACE_MODE_HALF);
			ledRender();
		}
		else if (currentDodeca->id == DODECA_STOP_FACE)
		{
			ledSetFaceColour(currentDodeca->id, currentDodeca->colour, colourFindByid(COLOUR_BLACK_ID)->code, LED_FACE_MODE_HALF);
			ledRender();
		}
		else if (currentDodeca->state == DODECA_STATE_STOPPED)
		{
			dodecaStart(currentDodeca->id);

		}


		breakState(STATE_CHANGE_TASK);
	case STATE_ERROR:
			displayState();
			currentState = findState(currentState->done);
			if (STATE_CONTROLLER_ERROR_FACE_DETECT_FAIL && errorFlags )
			{
				errorFlags &= ~ STATE_CONTROLLER_ERROR_FACE_DETECT_FAIL; // Clear the error flag
				if (STATE_BEGIN == lastState)
				{
					currentState = findState(STATE_BEGIN);
				}
			}
			else
			{
				ledSetFaceColour(currentDodeca->id,currentDodeca->colour,0x0,LED_FACE_MODE_ERROR);
				ledRender();
			}
			breakState(STATE_ERROR);
	case STATE_BATTERY_TEST:

			displayState();
			breakState(STATE_ERROR);

	case STATE_SLEEP:
			displayState();
			currentState = findState(currentState->done);
			// TODO : Goto Sleep
			HAL_SuspendTick();
			HAL_TIM_Base_Start_IT(&htim9);
			HAL_PWR_EnableSleepOnExit();
			HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
			 HAL_ResumeTick();
			breakState(STATE_SLEEP);
	case STATE_NULL:

		displayState();
		breakState(STATE_NULL);
	}


}
