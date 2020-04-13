/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CAN.h"
#include "visEffect.h"
#include "ws2812b.h"
#include "LightAndButton.h"
#include "stdbool.h"

#include "DMS.h"

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
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

CAN_HandleTypeDef hcan;

osThreadId_t defaultTaskHandle;
osThreadId_t CANHandle;
osThreadId_t LEDStripsHandle;
osThreadId_t DMSHandle;
osThreadId_t BatteryOverLoadHandle;
/* USER CODE BEGIN PV */

uint8_t visMode = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN_Init(void);
static void MX_ADC2_Init(void);
void StartDefaultTask(void *argument);
void StartCAN(void *argument);
void StartLEDStrips(void *argument);
void StartDMS(void *argument);
void StartBatteryOverLoadProtection(void *argument);

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
  ///////////////////////////////////////////////////////////////////////////////////
  	visInit();
  	Lights_Init();
  	Button_Init();
  ///////////////////////////////////////////////////////////////////////////////////
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_CAN_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* definition and creation of CAN */
  const osThreadAttr_t CAN_attributes = {
    .name = "CAN",
    .priority = (osPriority_t) osPriorityAboveNormal,
    .stack_size = 128
  };
  CANHandle = osThreadNew(StartCAN, NULL, &CAN_attributes);

  /* definition and creation of LEDStrips */
  const osThreadAttr_t LEDStrips_attributes = {
    .name = "LEDStrips",
    .priority = (osPriority_t) osPriorityHigh,
    .stack_size = 512
  };
  LEDStripsHandle = osThreadNew(StartLEDStrips, NULL, &LEDStrips_attributes);

  /* definition and creation of DMS */
  const osThreadAttr_t DMS_attributes = {
    .name = "DMS",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128
  };
  DMSHandle = osThreadNew(StartDMS, NULL, &DMS_attributes);

  /* definition and creation of BatteryOverLoad */
  const osThreadAttr_t BatteryOverLoad_attributes = {
    .name = "BatteryOverLoad",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128
  };
  BatteryOverLoadHandle = osThreadNew(StartBatteryOverLoadProtection, NULL, &BatteryOverLoad_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */
  /** Common config 
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */

/* USER CODE BEGIN MX_CAN_Init */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 16;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

/* USER CODE END MX_CAN_Init */

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartCAN */
/**
* @brief Function implementing the CAN thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCAN */
void StartCAN(void *argument)
{
  /* USER CODE BEGIN StartCAN */
  /* Infinite loop */
	HAL_CAN_Start(&hcan);
	CAN_INIT(&hcan);
	uint32_t StartTick = HAL_GetTick();
	HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
  for(;;)
  {
	  //HAL_CAN_RxFifo0MsgPendingCallback()
	if((StartTick+80)<HAL_GetTick())
	{
		CAN_SEND_STATUS(&hcan);
		StartTick = HAL_GetTick();
	}

	CAN_RECEIVED_PACKAGE(&hcan);
//	HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData);
    osDelay(1);
  }
  /* USER CODE END StartCAN */
}

/* USER CODE BEGIN Header_StartLEDStrips */
/**
* @brief Function implementing the LEDStrips thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLEDStrips */
void StartLEDStrips(void *argument)
{
  /* USER CODE BEGIN StartLEDStrips */
  /* Infinite loop */

//init ledButton
  bool ledButtonFlag = 0;
  static bool LightsOn, PressedFlag = false;
  static uint8_t ButtonPressCnt, WaitCnt = 0; 				//visMode = 0-4 defines the active animation
  static uint8_t visMaxBirghtness = 255;							//defines the max brightness of the animation
  static uint16_t buttonCnt = 0;			//counts the time the button is being pressed
  for(;;)
  {
	if(LightsOn)
		UpdateLights();
	CheckDirection();
	CheckflashBrakelight();

	ledButtonFlag = HAL_GPIO_ReadPin(GPIOA, Button_Pin);
	visHandle(visMode, visMaxBirghtness);
    osDelay(10);
    if(ledButtonFlag == 1 && HAL_GPIO_ReadPin(GPIOA, Button_Pin) == 1)
    {
    	buttonCnt++;
    	ledButtonFlag = 0;
    }
    if(ledButtonFlag == 1 && !HAL_GPIO_ReadPin(GPIOA, Button_Pin))
    {
    	if(buttonCnt > 5 && buttonCnt < 100)
    	{
			ButtonPressCnt++;
			PressedFlag = 1;
			WaitCnt = 0;
    	}
    	ledButtonFlag = 0;		//reset ledButtonFlag at falling edge
    	buttonCnt = 0;
    }



    if(buttonCnt >= 100)
    {
    	PressedFlag = ButtonPressCnt = WaitCnt = 0;
    	uint8_t i;
    	for(i=0;i<7;i++)
    	{
    		if(buttonCnt >= (i+1)*100 && buttonCnt < (i+2)*100)
			{
    			visMaxBirghtness = 255-(i*51);
    			break;
			}
    		if(buttonCnt==700)
    		{
    			buttonCnt = 0;
    			break;
    		}
    	}
    }

    if(PressedFlag && buttonCnt == 0)
    {
    	WaitCnt++;
    	if(WaitCnt == 50)
    	{
    		switch (ButtonPressCnt)
    		{
				case 1:
				{
		    		if(visMode == 4) visMode = 0;
		    		else visMode++;
					break;
				}
				case 2:
				{
					if(LightsOn)
					{
						LightsOn = false;
						TurnOffLights();
					}
					else
					{
						LightsOn = true;
						TurnOnLights();
					}
					break;
				}
				case 3:
				{
					visMode = 4;
					break;
				}

    		}
    		PressedFlag = ButtonPressCnt = WaitCnt = 0;
    	}
    }

  }
  /* USER CODE END StartLEDStrips */
}

/* USER CODE BEGIN Header_StartDMS */
/**
* @brief Function implementing the DMS thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDMS */
void StartDMS(void *argument)
{

	/* USER CODE BEGIN StartDMS */
	HAL_ADC_Start(&hadc1);
	HAL_ADC_Start(&hadc2);
	bool DMS1_Step=false;
	bool DMS2_Step=false;
	bool DMS1_Alert=false;
	bool DMS2_Alert=false;
	uint32_t DMS1_timer=0;
	uint32_t DMS2_timer=0;
	bool DMS_Break = false;
	bool brake_Status = false;
	float BrakeCurrent=0;
	/* Infinite loop */

	for(;;)
	{
		if(visMode == 4)
		{
			volatile uint32_t DMS1_Val = ADC_Poll2(&hadc1)&0xfff; //get DMS Voltage
			volatile uint32_t DMS2_Val = ADC_Poll2(&hadc2)&0xfff;

			DMS1_Step=Check_Threshhold(DMS1_Val); //Check Value
			DMS2_Step=Check_Threshhold(DMS2_Val);

			if(DMS1_Step==true){ //person on board
				DMS1_timer=HAL_GetTick(); //get current timer value
				DMS1_Alert=false;
			}
			else{//no person on board
				uint32_t delta_time=HAL_GetTick();
				if(DMS1_timer+100<delta_time){
					DMS1_Alert=true;
				}
			}

			if(DMS2_Step==true){ //person on board
					DMS2_timer=HAL_GetTick(); //get current timer value
					DMS2_Alert=false;
				}
				else{//no person on board
					uint32_t delta_time=HAL_GetTick();
					if(DMS2_timer+1000<delta_time){
						DMS2_Alert=true;
					}
				}
			if(DMS1_Alert&&DMS2_Alert==true){
				DMS_Break=true;
				BrakeCurrent=(float)getBrakeCurrentRel()/1000;
				CAN_SEND_BRAKE_CURRENT(&hcan,BrakeCurrent);
			}
			else{
				DMS_Break=false;
				CAN_SEND_BRAKE_CURRENT(&hcan,0);
			}
		    osDelay(1);
		  }
		else
			DMS_Break=false;

		if(DMS_Break != brake_Status)
		{
			CAN_SEND_BRAKE_CURRENT(&hcan,DMS_Break*(-50));
			brake_Status = DMS_Break;
		}
	}

  /* USER CODE END StartDMS */
}

/* USER CODE BEGIN Header_StartBatteryOverLoadProtection */
/**
* @brief Function implementing the BatteryOverLoad thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBatteryOverLoadProtection */

bool EstimateRecuperation()
{
	volatile int32_t current=0;
	static int32_t ticks;
	current=getBrakeCurrent();
	uint32_t rpm = abs(getRPM());

	if(current>=0 || rpm < 100 )
	{
		ticks = HAL_GetTick();
		return false;
	}
	else
	{
		if((ticks+10)<HAL_GetTick())
			return true;
	}
	return false;
}

void StartBatteryOverLoadProtection(void *argument)
{
  /* USER CODE BEGIN StartBatteryOverLoadProtection */
	volatile int32_t voltage=0;
	volatile bool recuperation = false;
  /* Infinite loop */
  for(;;)
  {
	  voltage=getBatteryVoltage();
	  recuperation = EstimateRecuperation();
	  if(recuperation&&voltage>40800)
	  {
		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
	  }
	  else{
 		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	  }

    osDelay(1);
  }
  /* USER CODE END StartBatteryOverLoadProtection */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  /* USER CODE BEGIN Callback 0 */
/////////////////////////////////////////////////////////////////////////////////////
//  if(htim->Instance == TIM2)
//  {
//	  if (ws2812b.timerPeriodCounter < (uint8_t)WS2812_RESET_PERIOD)
//	  	{
//	  		// count the number of timer periods
//	  		ws2812b.timerPeriodCounter++;
//	  	}
//	  	else
//	  	{
//	  		ws2812b.timerPeriodCounter = 0;
//	  		__HAL_TIM_DISABLE(&Tim2Handle);
//	  		TIM2->CR1 = 0; // disable timer
//
//	  		// disable the TIM2 Update
//	  		__HAL_TIM_DISABLE_IT(&Tim2Handle, TIM_IT_UPDATE);
//	  		// set TransferComplete flag
//	  		ws2812b.transferComplete = 1;
//	  	}
//  }
/////////////////////////////////////////////////////////////////////////////////////
//  /* USER CODE END Callback 0 */
//  if (htim->Instance == TIM1) {
//    HAL_IncTick();
//  }
//  /* USER CODE BEGIN Callback 1 */
//
//  /* USER CODE END Callback 1 */
//}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
