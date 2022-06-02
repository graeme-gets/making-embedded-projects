/*
 * Tasks.h
 *
 *  Created on: Jun 2, 2022
 *      Author: GraemeGets
 */

#ifndef TASKS_INC_TASKS_H_
#define TASKS_INC_TASKS_H_

#include "main.h"

#define TASK_NAME_LENGHTH_MAX 30
#define TASK_NAME_LENGHTH_MIN 30
#define TASK_COUNT_MAX 20

#define TASK_DEFAULT_MIN_TIME 0xaa
#define TASK_DEFAULT_MAX_TIME 0xbb
#define TASK_DEFAULT_NAME "NOT Configured!"

typedef enum{
	TASK_OK = 0u,
	TASK_INVALID_ID =1u,
	TASK_NAME_TOO_LONG =2u,
	TASK_NAME_TOO_SHORT =3u
}eTaskError_t;

typedef struct
{
	uint8_t id;
	char name[TASK_NAME_LENGHTH_MAX];
	uint32_t colour;
	uint16_t defaultMinTime;
	uint16_t defaultMaxTime;
} taskItem_t;

typedef struct
{
	taskItem_t items[TASK_COUNT_MAX];
} taskItems_t;


/***********************************************************
 * PUBLIC PROTOTYPES
 ***********************************************************/
eTaskError_t taskInit(taskItems_t *taskItems);
eTaskError_t taskNameSet(uint8_t id,char* name);
eTaskError_t taskSetDefaultMaxTime(uint8_t id, uint16_t maxTime);
eTaskError_t taskSetDefaultMinTime(uint8_t id, uint16_t minTime);
eTaskError_t taskNameSet(uint8_t id,char* name);
eTaskError_t taskValidateTaskId(uint8_t id);
taskItem_t * taskGet(uint8_t id );
eTaskError_t taskSetDefault(uint8_t id);
eTaskError_t taskSetDefaultAll();

#endif /* TASKS_INC_TASKS_H_ */
