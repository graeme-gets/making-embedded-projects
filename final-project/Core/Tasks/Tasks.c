/*
 * Tasks.c
 *
 *  Created on: Jun 2, 2022
 *      Author: GraemeGets
 */
#include "Tasks.h"
#include "main.h"
#include <string.h>

static taskItems_t * taskItemsPtr = 0x0;

eTaskError_t taskInit(taskItems_t *taskItems)
{
	taskItemsPtr = taskItems;
	return TASK_OK;
}


eTaskError_t taskValidateTaskId(uint8_t id)
{
	if (id > TASK_COUNT_MAX)
		return TASK_INVALID_ID;
	else
		return TASK_OK;
}

eTaskError_t taskSetDefaultAll()
{
	for (uint8_t t=0;t<TASK_COUNT_MAX;t++)
	{
		taskSetDefault(t);
	}
	return TASK_OK;
}

eTaskError_t taskSetDefault(uint8_t id)
{
	if (TASK_INVALID_ID == taskValidateTaskId(id))
		return TASK_INVALID_ID;

	taskItem_t *item = taskGet(id);

	item->id = id;
	item->colour = 0;
	item->defaultMinTime = TASK_DEFAULT_MIN_TIME;
	item->defaultMaxTime = TASK_DEFAULT_MAX_TIME;
	strcpy(item->name,TASK_DEFAULT_NAME);

	return TASK_OK;
}

eTaskError_t taskSetDefaultMinTime(uint8_t id, uint16_t minTime)
{
	if (TASK_INVALID_ID == taskValidateTaskId(id))
			return TASK_INVALID_ID;
	taskItem_t *item = taskGet(id);
	item->defaultMinTime = minTime;
	return TASK_OK;
}

eTaskError_t taskSetDefaultMaxTime(uint8_t id, uint16_t maxTime)
{
	if (TASK_INVALID_ID == taskValidateTaskId(id))
			return TASK_INVALID_ID;
	taskItem_t *item = taskGet(id);
	item->defaultMaxTime = maxTime;
	return TASK_OK;
}

eTaskError_t taskNameSet(uint8_t id,char* name)
{

	// Check the is not too long or too small
	if (strlen(name) > TASK_NAME_LENGHTH_MAX)
	{
		return TASK_NAME_TOO_LONG;
	}
	if (strlen(name) < TASK_NAME_LENGHTH_MIN)
	{
		return TASK_NAME_TOO_SHORT;
	}

	taskItem_t *item = taskGet(id);

	strcpy(item->name,name);
	return TASK_OK;
}

taskItem_t * taskGet(uint8_t id )
{
	if (TASK_INVALID_ID == taskValidateTaskId(id))
		return 0x0;

	return &taskItemsPtr->items[id];

}

