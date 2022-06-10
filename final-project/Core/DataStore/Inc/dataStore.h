/*
 * dataStore.h
 *
 *  Created on: 10 Jun 2022
 *      Author: GraemeGets
 */

#ifndef DATASTORE_H_
#define DATASTORE_H_

eDataStorage_t dataStoreInit();
eDataStorage_t dataStoreAdd(recordDodeca_t record);
eDataStorage_t dataStoreGetCurrentPosition(uint8_t *position);
recordDodeca_t * dataStoreGet( uint8_t id);


#endif /* DATASTORE_H_ */
