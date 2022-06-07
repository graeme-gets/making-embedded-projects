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


stateControl_t stateTable[] = {
//											New Face Detec		Lipo Int	, 		RTC Int				Sleep Timeout, 		Done/TimeOut
		{STATE_CONFIG, 		"Config",		STATE_NONE	,		STATE_NONE	, 		STATE_NONE, 		STATE_NONE,			STATE_NONE},
		{STATE_IDLE, 		"Idle",			STATE_NONE,			STATE_BATTERY_TEST,	STATE_UPDATE_TASK, 	STATE_CHECK_OREN,	STATE_SLEEP},
		{STATE_CHECK_OREN, 	"Orientation",	STATE_CHANGE_TASK,	STATE_NONE,			STATE_NONE,  		STATE_NONE,			STATE_SLEEP},
		{STATE_SLEEP, 		"Sleep",		STATE_NONE	,		STATE_NONE	, 		STATE_NONE, 		STATE_IDLE,			STATE_NONE},
		{STATE_CHANGE_TASK, "Change Task",	STATE_NONE	,		STATE_NONE	, 		STATE_NONE, 		STATE_NONE,			STATE_NONE},
		{STATE_END_TASK, 	"End Task",		STATE_NONE	,		STATE_NONE	, 		STATE_NONE, 		STATE_NONE,			STATE_NONE},
		{STATE_UPDATE_TASK, "Update Task",	STATE_NONE	,		STATE_NONE	, 		STATE_NONE, 		STATE_NONE,			STATE_NONE},
		{STATE_BATTERY_TEST,"Battery Test",	STATE_NONE	,		STATE_NONE	, 		STATE_NONE, 		STATE_NONE,			STATE_NONE},
		{STATE_NONE,		"NONE!",		STATE_NONE	,		STATE_NONE	, 		STATE_NONE, 		STATE_NONE,			STATE_NONE},
};

uint8_t stateActionRegister  = 0;
eState_t  currentState = STATE_IDLE;

static stateControl_t *findState(const eState_t state);


static stateControl_t *findState(const eState_t state)
{
	eState_t stateLoop = 0xff;
	uint8_t cnt = 0;
	while (STATE_NONE != stateLoop)
	{
		if (state == stateTable[cnt].state)
			return &stateTable[cnt];
		cnt++;
	}
	return &stateTable[cnt];
}


void stateContollerInit(eState_t state)
{
	currentState = state;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim9)
	{
		stateActionRegister |= STATE_ACTIONFLAG_SLEEP_OUT;
	}
}


void stateController()
{
	stateControl_t *nextState;
	MPU6050_t imuData;
	uint8_t detectedFace;

	switch (currentState)
	{
	case STATE_CONFIG:

		break;
	case STATE_IDLE:

			if (stateActionRegister && STATE_ACTIONFLAG_SLEEP_OUT )
			{
				nextState = findState(currentState);
				currentState = nextState->SleepTimeout;
			}
		break;
	case STATE_CHECK_OREN:
			MPU6050ReadStable(&imuData);
			detectedFace = detectFace(imuData.KalmanAngleX, imuData.KalmanAngleY);
		break;
	case STATE_CHANGE_TASK:
		break;
	case STATE_UPDATE_TASK:
			break;
	case STATE_BATTERY_TEST:
		break;
	case STATE_END_TASK:
		break;
	case STATE_SLEEP:
		if (stateActionRegister && STATE_ACTIONFLAG_NEW_FACE )
					{
						nextState = findState(stateTable->state);
						stateTable->state = nextState->newFaceDetected;
					}
		break;
	case STATE_NONE:
		break;
	}


}
