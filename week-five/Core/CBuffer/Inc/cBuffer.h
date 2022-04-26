/*
 * cBuffer.h
 *
 *  Created on: Apr 25, 2022
 *      Author: GraemeGets
 */

#ifndef CBUFFER_CBUFFER_H_
#define CBUFFER_CBUFFER_H_

#include "main.h"

/***********************************************************
 * ENUMS
 ***********************************************************/
typedef enum{
	CBUFFER_OK = 0,
	CBUFFER_FULL,
	CBUFFER_EMPTY,
	CBUFFER_NO_STRING,
}CBUFFER_ERR;


/***********************************************************
 * STRUCTS
 ***********************************************************/

typedef struct {
	uint8_t * buffer;
	uint8_t size;
	uint8_t read;
	uint8_t write;
} cBuffer_t;


/***********************************************************
 * PROTOTYPES
 ***********************************************************/
void cBuffer_init(cBuffer_t *cbuffer,uint8_t* buffer,uint8_t size);
CBUFFER_ERR cBuffer_Write(cBuffer_t * cbuffer, uint8_t data);
CBUFFER_ERR cBuffer_Read(cBuffer_t* cbuffer,uint8_t* data);
CBUFFER_ERR cBuffer_Length(cBuffer_t* cbuffer);
CBUFFER_ERR cBuffer_GetString(cBuffer_t* cbuffer,uint8_t *string, uint8_t sTerminator);
CBUFFER_ERR cBuffer_isFull(cBuffer_t* cbuffer);
CBUFFER_ERR cBuffer_Kill(cBuffer_t* cbuffer);
#endif /* CBUFFER_CBUFFER_H_ */
