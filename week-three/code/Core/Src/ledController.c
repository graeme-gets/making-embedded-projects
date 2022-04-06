/*
 * ledController.c
 *
 *  Created on: Apr 6, 2022
 *      Author: GraemeGets
 */

#include "main.h"

static uint8_t GreenLedEnabled;


// ****************************************************************//
// Sets the state of the LED. If 0 then LED is not enabled, if 1
// then LED is enabled and can be switched of or toggled
// ****************************************************************//
void SetGreenLedState(uint8_t state)
{
	GreenLedEnabled = state;
}

void GreenLedStateToggle()
{
	if (GreenLedEnabled)
		GreenLedEnabled = 0;
	else
		GreenLedEnabled = 1;
}

void GreenLedToggle()
{
	if (GreenLedEnabled)
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

void GreenLedOff()
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_RESET);
}

void GreenLedOn()
{
	if (GreenLedEnabled)
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);
}
