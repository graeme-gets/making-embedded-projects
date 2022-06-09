/*
 * StateController.h
 *
 *  Created on: Jun 7, 2022
 *      Author: GraemeGets
 */

#ifndef STATECONTOLLER_INC_STATECONTROLLER_H_
#define STATECONTOLLER_INC_STATECONTROLLER_H_


#define ACTION_NEW_FACE_DETECTED

typedef enum
{
	STATE_NULL,
	STATE_CONFIG,
	STATE_IDLE,
	STATE_SLEEP,
	STATE_CHANGE_TASK,
	STATE_CHECK_OREN,
	STATE_END_TASK,
	STATE_START_TASK,
	STATE_UPDATE_TASK,
	STATE_BATTERY_TEST,
	STATE_ERROR,
	STATE_BEGIN
}eState_t;

typedef enum {
	STATE_CONT_MODE_RUN,
	STATE_CONT_MODE_CONFIG
} eStateControllerMode_t;
typedef struct {
	eState_t state;
	char *debugName;
	eState_t newFaceDetect;
	eState_t lipoInt;
	eState_t RTCInt;
	eState_t SleepTimeout;
	eState_t done;
	eState_t error;

}stateControl_t;

#define STATE_ACTIONFLAG_LIPO_INT 1<<1
#define STATE_ACTIONFLAG_RTC_INT  1<<2
#define STATE_ACTIONFLAG_SLEEP_OUT 1<<3
#define STATE_ACTIONFLAG_EXIT_CONFIG 1<<4








void stateController();
void stateContollerInit(eState_t state);
void setStateControllerMode(eStateControllerMode_t mode);

#endif /* STATECONTOLLER_INC_STATECONTROLLER_H_ */
