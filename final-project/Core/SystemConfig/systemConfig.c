/*
 * config.c
 *
 *  Created on: Jun 1, 2022
 *      Author: GraemeGets
 */
#include "systemConfig.h"
#include "crc.h"
#include "FLASH_SECTOR_F4.h"
#include "string.h"




__attribute__((__section__(".systemConfig"))) const char systemConfigROPtr; // TODO: Better if this was a struct


systemConfig_t *systemConfigRO = (systemConfig_t*)&systemConfigROPtr;
const uint32_t SYSTEM_CONFIG_ADDRESS = (uint32_t)&systemConfigROPtr;


/***********************************************************
 *
 * PRIVATE PROTOTYPES
 *
 ***********************************************************/
static eSYSConfig_t sysConfigValidateConfig();


systemConfig_t systemConfig= {};

static uint16_t SYS_CONFIG_SIZE ;
static uint16_t SYS_CONFIG_ALL_SIZE;
static uint16_t SYS_CONFIG_SIZE_TASK;
static uint16_t SYS_CONFIG_SIZE_DODEC;



void sysConfigInit()
{
	SYS_CONFIG_SIZE = sizeof(systemConfig.configItems)/4;
	SYS_CONFIG_ALL_SIZE = sizeof(systemConfig)/4;
	SYS_CONFIG_SIZE_TASK = sizeof(systemConfig.configItems.tasksConfig);
	SYS_CONFIG_SIZE_DODEC = sizeof(systemConfig.configItems.dodecaConfig);
}

systemConfig_t *systemConfigGet()
{
	return &systemConfig;
}

static eSYSConfig_t sysConfigValidateConfig()
{
	uint32_t crc = sysConfigCalcChecksum();
	uint32_t crcRO = systemConfigRO->checksum;
	if (crc == crcRO)
		return SYS_CONFIG_OK;
	else
		return SYS_CONFIG_BAD_CHECKSUM;
}

uint32_t sysConfigCalcChecksum()
{
	uint32_t crc;
	crc =  HAL_CRC_Calculate(&hcrc,  (uint32_t*)&systemConfig.configItems, SYS_CONFIG_SIZE);
	return crc;
}




eSYSConfig_t sysConfigRead()
{
	Flash_Read_Data(SYSTEM_CONFIG_ADDRESS,(uint32_t*) &systemConfig,SYS_CONFIG_ALL_SIZE );
	//Flash_Read_Data(SYSTEM_CONFIG_ADDRESS,(uint32_t*) &systemConfig,5 );
	if (SYS_CONFIG_BAD_CHECKSUM == sysConfigValidateConfig())
		return SYS_CONFIG_BAD_DATA;
	else
		return TASK_OK;
}

void sysConfigSave()
{
	uint32_t crc = sysConfigCalcChecksum();
	systemConfig.checksum = crc;

	Flash_Write_Data(SYSTEM_CONFIG_ADDRESS, (uint32_t *)&systemConfig, SYS_CONFIG_ALL_SIZE);
	//TODO: Handle a Confug Save Error - Hard Fault!
}

void sysConfigUpdateChecksum(uint32_t csm)
{
	systemConfig.checksum = csm;
}
