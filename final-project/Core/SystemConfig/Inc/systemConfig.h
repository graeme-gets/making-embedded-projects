/*
 * config.h
 *
 *  Created on: Jun 1, 2022
 *      Author: GraemeGets
 */

#ifndef CONFIG_INC_CONFIG_H_
#define CONFIG_INC_CONFIG_H_

#include <Dodeca.h>
#include "stm32f4xx.h"
#include "Tasks.h"
#define FACE_COUNT 12



typedef enum{
	SYS_CONFIG_OK = 0u,
	SYS_CONFIG_BAD_CHECKSUM = 1u,
	SYS_CONFIG_BAD_DATA = 2u
} eSYSConfig_t;


typedef struct
{
	taskItems_t tasksConfig;
	dodecaItems_t dodecaConfig;
}configItems_t;

typedef struct
{
	configItems_t configItems;
	uint32_t checksum;
}systemConfig_t;




uint32_t sysConfigCalcChecksum();
void sysConfigUpdateChecksum(uint32_t csm);
void sysConfigSave();
eSYSConfig_t sysConfigRead();
void sysConfigInit();
systemConfig_t *systemConfigGet();
#endif /* CONFIG_INC_CONFIG_H_ */
