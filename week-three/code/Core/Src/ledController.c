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

void ledControlStateToggle()
{
	if (ledControlEnabled)
		ledControlEnabled = 0;
	else
		ledControlEnabled = 1;
}

void ledToggle()
{
	if (ledControlEnabled)
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

void ledOff()
{
	if (ledControlEnabled)
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_RESET);
}

void ledOn()
{
	if (ledControlEnabled)
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);
}
