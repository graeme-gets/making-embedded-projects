/*
 * config.h
 *
 *  Created on: Jun 1, 2022
 *      Author: GraemeGets
 */

#ifndef CONFIG_INC_CONFIG_H_
#define CONFIG_INC_CONFIG_H_

#include "stm32f4xx.h"

#define FACE_COUNT 12

typedef enum{
	FACE_ACTIVE = 0u,
	FACE_NOT_ACTIVE = 1u,
	FACE_NOT_CONFIGURED = 1u,
}eFaceStatus_t;


typedef enum{
	SYS_CONFIG_OK = 0u,
	SYS_CONFIG_BAD_CHECKSUM = 1u
}eSYSConfig_t;


typedef struct{
	uint8_t taskId;
	uint8_t faceStatus;
	uint8_t reserved;
	uint32_t colour;
	char name[16];
}configFace_t;

typedef struct
{
	configFace_t faceConfig[FACE_COUNT];
	uint32_t checksum;
}systemConfig_t;







uint32_t sysConfigCalcChecksum();
void sysConfigUpdateChecksum(uint32_t csm);
void sysConfigSetFace(uint8_t id,configFace_t face);
void sysConfigSave();
#endif /* CONFIG_INC_CONFIG_H_ */
