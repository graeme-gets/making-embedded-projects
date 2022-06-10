/*
 * dataStore.c
 *
 *  Created on: 10 Jun 2022
 *      Author: GraemeGets
 */

#include "dodeca.h"
#include "dataStore.h"

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

	storage[recordPosition].dodecaId = record.dodecaId;
	storage[recordPosition].endTime = record.endTime;
	storage[recordPosition].startTime = record.startTime;
	storage[recordPosition].status = RECORD_STORE_USED;
	recordPosition++;
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


