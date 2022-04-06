/*
 * debounceController.c
 *
 *  Created on: Apr 6, 2022
 *      Author: GraemeGets
 */

#include "main.h"
#include "ledController.h"
#include "debounceController.h"
#include "tim.h"
#include "usart.h"

static volatile uint32_t debounceValue = 1;
static volatile uint32_t pressed = 0;

// ****************************************************************//
// Returns the State of the button press.
// ****************************************************************//
uint8_t buttonPressed()
{
	return pressed;
}


// ****************************************************************//
// Resets the button state
// ****************************************************************//
void buttonReset()
{
	pressed = 0;
}


// ****************************************************************//
// Stops the Timer and re-eanbles the button interrupt
// ****************************************************************//
void stopDebounceFlow()
{
	// Stop the timer
	HAL_TIM_Base_Stop(&htim3);
	// Enable IRQ for blue button
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}


// ****************************************************************//
// This timer callback is triggered when the period of the timer is
// triggered. The button interrupt is disabled so that the interrupt
// cannot interfere with the state of the button. This call back then deals
// with any left over bounce.
// The timer is set to 5ms, so the button is sampled every 5ms. The denounce value is
// shifted left for every sample where the button is pressed (high) and incremented
// for every sample that is not pressed (low). If the debounce value
// after 10 samples (ie 50ms) then the button state is stable and
// is set to 'Pressed'
// ****************************************************************//
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// Disable the Button Interrupt
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	// Check that there is a value on the stack
	if (debounceValue==0)
		debounceValue =1;

	// read the Button Value
	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0)
	{
		debounceValue  = debounceValue << 1;
		HAL_GPIO_TogglePin(testTimer_GPIO_Port, testTimer_Pin);
	}
	else
	{
		debounceValue++;
		pressed = 0;
	}

	if ((debounceValue & msUnits) == 0)
	{
		// Stop the timer
		stopDebounceFlow();
		// reset the debounce value
		debounceValue = 1;
		pressed = 1;
	}
}


// ****************************************************************//
// Interrupt for when the button is pressed.
// ****************************************************************//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	HAL_TIM_Base_Start_IT(&htim3);
}
