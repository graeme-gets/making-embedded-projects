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


#define SYSTEM_CONFIG_ADDRESS 0x807FC00 // TODO: There must be a way to reference this from the linker file? Extern?

__attribute__((__section__(".systemConfig"))) const char systemConfigROPtr; // TODO: Better if this was a struct

systemConfig_t *systemConfigRO = (systemConfig_t*)&systemConfigROPtr;



/***********************************************************
 *
 * PRIVATE PROTOTYPES
 *
 ***********************************************************/
static eSYSConfig_t sysConfigValidateConfig();
static void sysConfigRead();

systemConfig_t systemConfig= {};

const uint8_t SYS_CONFIG_SIZE = sizeof(systemConfig.faceConfig)/4;

void sysConfigInit()
{
	sysConfigRead();
	if (SYS_CONFIG_BAD_CHECKSUM ==  sysConfigValidateConfig())
	{
		for (uint8_t i=0;i<FACE_COUNT;i++)
		{
			systemConfig.faceConfig[i].colour = 0;
			systemConfig.faceConfig[i].faceStatus = FACE_NOT_CONFIGURED;
			strcpy(systemConfig.faceConfig[i].name,"NOT CONFIGURED!");
			systemConfig.faceConfig[i].reserved = 0;
			systemConfig.faceConfig[i].taskId = 0;
		}
		sysConfigSave();
	}
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
	crc =  HAL_CRC_Calculate(&hcrc, (uint32_t*)systemConfig.faceConfig, SYS_CONFIG_SIZE);
	return crc;
}

void sysConfigSetFace(uint8_t id,configFace_t face)
{
	systemConfig.faceConfig[id] = face;
}

void sysConfigGetFace(uint8_t id, configFace_t * face)
{
//	*face =  systemConfigRO->faceConfig[id];
}

static void sysConfigRead()
{
	Flash_Read_Data(SYSTEM_CONFIG_ADDRESS,(uint32_t*) &systemConfig,SYS_CONFIG_SIZE );
}

void sysConfigSave()
{
	uint32_t crc = sysConfigCalcChecksum();
	systemConfig.checksum = crc;
	Flash_Write_Data(SYSTEM_CONFIG_ADDRESS, (uint32_t *)&systemConfig, SYS_CONFIG_SIZE+1);
}

void sysConfigUpdateChecksum(uint32_t csm)
{
	systemConfig.checksum = csm;
}
