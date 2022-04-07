/*
 * ledController.c
 *
 *  Created on: Apr 6, 2022
 *      Author: GraemeGets
 */

#include "main.h"

static uint8_t ledControlEnabled;


// ****************************************************************//
// Sets the state of the LED. If 0 then LED is not enabled, if 1
// then LED is enabled and can be switched of or toggled
// ****************************************************************//
void setLedControlState(uint8_t state)
{
	ledControlEnabled = state;
}


// ****************************************************************//
// Toggles the control state between Enabled and Disabled
// Returns the new state
// ****************************************************************//
uint8_t ledControlStateToggle()
{
	if (ledControlEnabled)
		ledControlEnabled = 0;
	else
		ledControlEnabled = 1;

	return ledControlEnabled;
}



void ledToggle()
{
	if (ledControlEnabled)
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

void ledOff(uint8_t force)
{
	if (ledControlEnabled || force)
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_RESET);
}

void ledOn()
{
	if (ledControlEnabled)
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);
}
