/*
 * dataStore.h
 *
 *  Created on: 10 Jun 2022
 *      Author: GraemeGets
 */

#ifndef DATASTORE_H_
#define DATASTORE_H_

#define DATASTORE_MAX_RECORDS 30

typedef enum {
	RECORD_STORE_EMPTY = 0u,
	RECORD_STORE_USED = 1u
}eRecordStatus_t;

typedef struct
{
	uint8_t dodecaId;
	time_t startTime;
	time_t endTime;
	eRecordStatus_t status;
}recordDodeca_t;

typedef enum{
	DATA_STORAGE_ERR_OK,
	DATA_STORAGE_ERR_FULL,
	DATA_STORAGE_ERR_NOT_EXIST
}eDataStorage_t;

eDataStorage_t dataStoreInit();
eDataStorage_t dataStoreAdd(recordDodeca_t record);
eDataStorage_t dataStoreGetCurrentPosition(uint8_t *position);
recordDodeca_t * dataStoreGet( uint8_t id);


#endif /* DATASTORE_H_ */
