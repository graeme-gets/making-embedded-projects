/*
 * dataStore.c
 *
 *  Created on: 10 Jun 2022
 *      Author: GraemeGets
 */

#include "dodeca.h"

#define DATASTORE_MAX_RECORDS 30


typedef enum {
	RECORD_STORE_EMPTY = 0u,
	RECORD_STORE_USED = 1u
}eRecordStatus_t;

typedef enum{
	DATA_STORAGE_ERR_OK,
	DATA_STORAGE_ERR_FULL,
	DATA_STORAGE_ERR_NOT_EXIST
}eDataStorage_t;

typedef struct
{
	uint8_t dodecaId;
	time_t startTime;
	time_t endTime;
	eRecordStatus_t status;
}recordDodeca_t;


uint8_t recordPosition;



recordDodeca_t storage[DATASTORE_MAX_RECORDS];



eDataStorage_t dataStoreInit()
{
	for (uint8_t i=0;i<DATASTORE_MAX_RECORDS;i++)
	{
		storage[i].status = RECORD_STORE_EMPTY;
	}
	return DATA_STORAGE_ERR_OK;
}


eDataStorage_t dataStoreAdd(recordDodeca_t record)
{
	if (DATASTORE_MAX_RECORDS-1 == recordPosition )
		return DATA_STORAGE_ERR_OK;

	storage[recordPosition] = record;
	return DATA_STORAGE_ERR_OK;
}

eDataStorage_t dataStoreGetCurrentPosition(uint8_t *position)
{
	*position = recordPosition;
	return DATA_STORAGE_ERR_OK;
}

recordDodeca_t * dataStoreGet( uint8_t id)
{
	if (id > recordPosition)
		return NULL;
	else
		return &storage[id];

}


