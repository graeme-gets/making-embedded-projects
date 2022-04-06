/*
 * debounceController.c
 *
 *  Created on: Apr 6, 2022
 *      Author: GraemeGets
 */

#include "main.h"
#include "ledController.h"
#include "tim.h"
#include "usart.h"

static volatile uint32_t debounceValue = 1;
static volatile uint32_t pressed = 0;

uint8_t buttonPressed()
{
	return pressed;
}

void buttonReset()
{
	pressed = 0;
}

void stopDebounceFlow()
{
	// Stop the timer
	HAL_TIM_Base_Stop(&htim3);
	// Enable IRQ for blue button
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// Disable the Button Interrupt
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	// Check that there is a value on the stack
	if (debounceValue==0)
		debounceValue =1;

	// read the Button Value
	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 1)
	{
		debounceValue  = debounceValue << 1;
	}
	else
	{
		debounceValue++;
		pressed = 0;
	}



	// If debounce value overflows then reset and start again (stop timer and enabel IRQ)

	if (debounceValue == (1<<10))
	{
		// Stop the timer
		stopDebounceFlow();
		// reset the debounce value
		debounceValue = 1;
		pressed = 1;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	HAL_TIM_Base_Start_IT(&htim3);
}
