/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "cBuffer.h"
#include "console.h"
#include "ws2812.h"
#include "mpu6050.h"
#include "ledController.h"
#include "orientation.h"
#include "systemConfig.h"
#include "StateController.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_CRC_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */

  ledAllOff();
  ledRender();
  sysConfigInit();
  systemConfig_t * config = systemConfigGet();
  dodecaItems_t *dodecaItems = &config->configItems.dodecaConfig;

  dodecaInit(dodecaItems);

/*  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGPERR);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
*/

  ConsoleInit();


  if (SYS_CONFIG_BAD_DATA == sysConfigRead())
  {
	  ConsoleSendLine("*** CONFIG INVALID ***\nResetting to default");
	  dodecaReset();
	  sysConfigSave();
  }



  HAL_Delay(100);
  while(1 == MPU6050_Init(&hi2c1) )
  {
	  ConsoleSendString("** ERROR Initialising MNP6050!\n");
	  hi2c1.Instance->CR1 &= ~(I2C_CR1_PE);

	  HAL_I2C_MspDeInit(&hi2c1);
	  hi2c1.Instance->SR2 &=~(I2C_SR2_BUSY);

	  GPIO_InitTypeDef GPIO_Init;
	  GPIO_Init.Pin = GPIO_PIN_7;
	  GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_Init.Pull = GPIO_PULLUP;
	  GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_Init);
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	  HAL_Delay(50);
	  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
	  MX_I2C1_Init();

	  //__HAL_RCC_GPIOB_CLK_ENABLE();
	  HAL_Delay(100);

  }
  // run the Accel To get initial angle ready. Seems to need to be run a numerb of times for the karman angle to settle???

detectFaceUp();

  ConsoleSendString("MPU6050 Initialised\n");

  //HAL_TIM_Base_Start_IT(&htim9);

  stateContollerInit(STATE_BEGIN);
  ledAllOff();


  ConsolePrintPrompt();
/////////////////////////////  uint8_t lastFace = 255;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  ConsoleProcess();
	  stateController();
/*
	  MPU6050_t data;
	uint8_t face;
	char msg[30];
	for (uint8_t cnt=0;cnt<20;cnt++)
	{
		MPU6050_Read_All(&I2C_MPU6050, &data);
	}

	face = detectFace(data.KalmanAngleX, data.KalmanAngleY);
	if (255 == face)
	{
		ledAllOff();
	}

	else if (face != lastFace)
	{
		lastFace = face;
		sprintf(msg,"Angle X: %f Y: %f",data.KalmanAngleX, data.KalmanAngleY);
		ConsoleSendLine(msg);

		uint32_t rgb_color = hsl_to_rgb((face*30), 255, 127);

		ledAllOff();
		ledSetFaceColour(face, (rgb_color >> 16) & 0xFF, (rgb_color >> 8) & 0xFF, rgb_color & 0xFF);
		ledRender();

		sprintf(msg,"Detected face %i is up",face);
		ConsoleSendLine(msg);
	}

	*/
	 HAL_Delay(200);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
