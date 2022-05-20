/*
 * lc709203.c
 *
 *  Created on: 18 May 2022
 *      Author: GraemeGets
 */



#include "lc709203.h"
#include "i2c.h"

static eLC709203_Error_t lc709203_getRegisterValue (uint8_t reg,uint16_t *value);

/***********************************************************
 * lc709203_getCallVoltage
 * Get the current voltage of the Lipo Cell
 * @param: 16bit pointer to value
 * @return: Error Code
 *
 ***********************************************************/
eLC709203_Error_t lc709203_getCellVoltage(uint16_t *voltage)
{
	return lc709203_getRegisterValue (LC709302_REG_CELL_VOLTAGE,voltage);
}

eLC709203_Error_t lc709203_getCellTemp(uint16_t *voltage)
{
	return lc709203_getRegisterValue (LC709302_REG_CELL_TEMP,voltage);
}

eLC709203_Error_t lc709203_getId(uint16_t *version)
{
	return lc709203_getRegisterValue (LC709302_REG_IC_VERSION,version);
}



/***********************************************************
 * lc709203_getRegisterValue
 * Generic function to retrieve a word value from the device
 * @param: reg - the Register value
 * @param: value - a 16 bit pointer to the value to be used
 * @return: Error code foe the function
 *
 ***********************************************************/
eLC709203_Error_t lc709203_getRegisterValue (uint8_t reg,uint16_t *value)
{
	HAL_StatusTypeDef result = HAL_I2C_Mem_Read(&hi2c1, LC709302_ADRESS, reg, 1, (uint8_t*)value, 2, 100);

	if (result != HAL_OK)
	{
		return LC798_COMM_ERROR;
	}
	else
		return LC798_OK;

}
