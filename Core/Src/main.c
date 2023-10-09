/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#include"software_timer.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
void display7SEG(int);
void update7SEG(int);
const int MAX_LED;
int index_led;
int led_buffer[4];
int hour, minute, second;
int index_led_matrix = 0;
uint16_t matrix_buffer[52] = {0x0000,
		0xF000,
		0x4800,0xF000,
		0x4400,0x4800,0xF000,
		0x4400,0x4400,0x4800,0xF000,
		0x4800,0x4400,0x4400,0x4800,0xF000,
		0xF000,0x4800,0x4400,0x4400,0x4800,0xF000,
		0x0000,0xF000,0x4800,0x4400,0x4400,0x4800,0xF000,
		0x0000,0x0000,0xF000,0x4800,0x4400,0x4400,0x4800,0xF000,
		0xF000,0x4800,0x4400,0x4400,0x4800,
		0xF000,0x4800,0x4400,0x4400,
		0xF000,0x4800,0x4400,
		0xF000,0x4800,
		0xF000
};
void updateClockBuffer();
void setTimer1(int);
void timer_run ();
void setTimer2();
void updateLEDMatrix(int);

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
   HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, RESET);
   HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, RESET);
   HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, RESET);
   HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, RESET);
   HAL_GPIO_WritePin(l1_GPIO_Port, l1_Pin, RESET);
   HAL_GPIO_WritePin(dot_GPIO_Port, dot_Pin, SET);

hour = 15 , minute = 8 , second = 50;
setTimer1(5);
setTimer2(5);
setTimer3(5);
setTimer4(5);
  while (1)
  {

	  if(timer1_flag == 1){
	  	  setTimer1(100);
	  	  HAL_GPIO_TogglePin(dot_GPIO_Port, dot_Pin);}

	  if(timer2_flag == 1){
	  	 setTimer2(50);
	  	 update7SEG(index_led++);}
	  if(index_led > 3) {index_led = 0;}

	  if(timer3_flag == 1) {
	  	setTimer3(100);
	  	second ++;
	  	if ( second >= 60){
	  		  second = 0;
	  		  minute ++;
	  		  	  }
	  	else if( minute >= 60){
	  		 minute = 0;
	  		 hour ++;
	  		  	  }
	  	else if( hour >=24){
	  		 hour = 0;
	  		  	  }
	  		updateClockBuffer();
	  	  }
	  if(timer4_flag == 1) {
		 updateLEDMatrix (index_led_matrix++);
		 if(index_led_matrix == 52) index_led_matrix = 0;
		 	 	 setTimer4(5);
	  }


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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

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
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|dot_Pin|l1_Pin
                          |en0_Pin|en1_Pin|en2_Pin|en3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|d_Pin|e_Pin|f_Pin
                          |g_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin dot_Pin l1_Pin
                           en0_Pin en1_Pin en2_Pin en3_Pin
                           ENM2_Pin ENM3_Pin ENM4_Pin ENM5_Pin
                           ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|dot_Pin|l1_Pin
                          |en0_Pin|en1_Pin|en2_Pin|en3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : a_Pin b_Pin c_Pin ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin d_Pin e_Pin f_Pin
                           g_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = a_Pin|b_Pin|c_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|d_Pin|e_Pin|f_Pin
                          |g_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void updateClockBuffer(){
	led_buffer[0] = hour / 10;
	led_buffer[1] = hour % 10;
	led_buffer[2] = minute / 10;
	led_buffer[3] = minute % 10;

}

 const int MAX_LED = 4;
 int index_led = 0;
 int led_buffer[4] = {1, 2, 3, 4};
 void update7SEG(int index){
	  switch (index){
	  case 0:
		  display7SEG(led_buffer[0]);
		  HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, RESET);
		  HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, SET);
		  HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, SET);
		  HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, SET);
		  break;
	  case 1:
		  display7SEG(led_buffer[1]);
		  HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, SET);
		  HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, RESET);
		  HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, SET);
		  HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, SET);
		  break;
	  case 2:
		  display7SEG(led_buffer[2]);
		  HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, SET);
		  HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, SET);
		  HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, RESET);
		  HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, SET);
		  break;
	  case 3:
		  display7SEG(led_buffer[3]);
		  HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, SET);
		  HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, SET);
		  HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, SET);
		  HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, RESET);
		  break;
	  default:
		  break;
	  }

}
 void display7SEG(int decimalVal) {
 	switch (decimalVal) {
 		case 0:
 			HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
 			HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
 			HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
 			HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
 			HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, RESET);
 			HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
 			HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, SET);
 			break;
 		case 1:
 			HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, SET);
 			HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
 			HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
 			HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, SET);
 			HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
 			HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SET);
 			HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, SET);
 			break;
 		case 2:
 			HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
 			HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
 			HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, SET);
 			HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
 			HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, RESET);
 			HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SET);
 			HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
 			break;
 		case 3:
 			HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
 			HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
 			HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
 			HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
 			HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
 			HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SET);
 			HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
 			break;
 		case 4:
 			HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, SET);
 			HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
 			HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
 			HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, SET);
 			HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
 			HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
 			HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
 			break;
 		case 5:
 			HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
 			HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, SET);
 			HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
 			HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
 			HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
 			HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
 			HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
 			break;
 		case 6:
 			HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
 			HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, SET);
 			HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
 			HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
 			HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, RESET);
 			HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
 			HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
 			break;
 		case 7:
 			HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
 			HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
 			HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
 			HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, SET);
 			HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
 			HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SET);
 			HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, SET);
 			break;
 		case 8:
 			HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
 			HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
 			HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
 			HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
 			HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, RESET);
 			HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
 			HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
 			break;
 		case 9:
 			HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
 			HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
 			HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
 			HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
 			HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
 			HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
 			HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
 			break;
 		default:
 			break;
 		}


 }
 void rstMatrix(){
 	HAL_GPIO_WritePin(GPIOB, ROW2_Pin|ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin|ROW7_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_SET);
 	HAL_GPIO_WritePin(GPIOA,ENM0_Pin|ENM1_Pin|ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin|ENM6_Pin|ENM7_Pin, GPIO_PIN_SET);
 }
 void updateLEDMatrix (int index ){
	rstMatrix();
	HAL_GPIO_WritePin(GPIOB,matrix_buffer[index], GPIO_PIN_RESET);
	switch ( index ){
	case 0:
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOA,ENM0_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOA,ENM0_Pin, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOA,ENM1_Pin, GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOA,ENM0_Pin, GPIO_PIN_RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOA,ENM1_Pin, GPIO_PIN_RESET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOA,ENM2_Pin, GPIO_PIN_RESET);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOA,ENM0_Pin, GPIO_PIN_RESET);
		break;
	case 8:
		HAL_GPIO_WritePin(GPIOA,ENM1_Pin, GPIO_PIN_RESET);
		break;
	case 9:
		HAL_GPIO_WritePin(GPIOA,ENM2_Pin, GPIO_PIN_RESET);
		break;
	case 10:
		HAL_GPIO_WritePin(GPIOA,ENM3_Pin, GPIO_PIN_RESET);
		break;
	case 11:
		HAL_GPIO_WritePin(GPIOA,ENM0_Pin, GPIO_PIN_RESET);
		break;
	case 12:
		HAL_GPIO_WritePin(GPIOA,ENM1_Pin, GPIO_PIN_RESET);
		break;
	case 13:
		HAL_GPIO_WritePin(GPIOA,ENM2_Pin, GPIO_PIN_RESET);
		break;
	case 14:
		HAL_GPIO_WritePin(GPIOA,ENM3_Pin, GPIO_PIN_RESET);
		break;
	case 15:
		HAL_GPIO_WritePin(GPIOA,ENM4_Pin, GPIO_PIN_RESET);
		break;
	case 16:
		HAL_GPIO_WritePin(GPIOA,ENM0_Pin, GPIO_PIN_RESET);
		break;
	case 17:
		HAL_GPIO_WritePin(GPIOA,ENM1_Pin, GPIO_PIN_RESET);
		break;
	case 18:
		HAL_GPIO_WritePin(GPIOA,ENM2_Pin, GPIO_PIN_RESET);
		break;
	case 19:
		HAL_GPIO_WritePin(GPIOA,ENM3_Pin, GPIO_PIN_RESET);
		break;
	case 20:
		HAL_GPIO_WritePin(GPIOA,ENM4_Pin, GPIO_PIN_RESET);
		break;
	case 21:
		HAL_GPIO_WritePin(GPIOA,ENM5_Pin, GPIO_PIN_RESET);
		break;
	case 22:
		HAL_GPIO_WritePin(GPIOA,ENM0_Pin, GPIO_PIN_RESET);
		break;
	case 23:
		HAL_GPIO_WritePin(GPIOA,ENM1_Pin, GPIO_PIN_RESET);
		break;
	case 24:
		HAL_GPIO_WritePin(GPIOA,ENM2_Pin, GPIO_PIN_RESET);
		break;
	case 25:
		HAL_GPIO_WritePin(GPIOA,ENM3_Pin, GPIO_PIN_RESET);
		break;
	case 26:
		HAL_GPIO_WritePin(GPIOA,ENM4_Pin, GPIO_PIN_RESET);
		break;
	case 27:
		HAL_GPIO_WritePin(GPIOA,ENM5_Pin, GPIO_PIN_RESET);
		break;
	case 28:
		HAL_GPIO_WritePin(GPIOA,ENM6_Pin, GPIO_PIN_RESET);
		break;
	case 29:
		HAL_GPIO_WritePin(GPIOA,ENM0_Pin, GPIO_PIN_RESET);
		break;
	case 30:
		HAL_GPIO_WritePin(GPIOA,ENM1_Pin, GPIO_PIN_RESET);
		break;
	case 31:
		HAL_GPIO_WritePin(GPIOA,ENM2_Pin, GPIO_PIN_RESET);
		break;
	case 32:
		HAL_GPIO_WritePin(GPIOA,ENM3_Pin, GPIO_PIN_RESET);
		break;
	case 33:
		HAL_GPIO_WritePin(GPIOA,ENM4_Pin, GPIO_PIN_RESET);
		break;
	case 34:
		HAL_GPIO_WritePin(GPIOA,ENM5_Pin, GPIO_PIN_RESET);
		break;
	case 35:
		HAL_GPIO_WritePin(GPIOA,ENM6_Pin, GPIO_PIN_RESET);
		break;
	case 36:
		HAL_GPIO_WritePin(GPIOA,ENM7_Pin, GPIO_PIN_RESET);
		break;
	case 37:
		HAL_GPIO_WritePin(GPIOA,ENM3_Pin, GPIO_PIN_RESET);
		break;
	case 38:
		HAL_GPIO_WritePin(GPIOA,ENM4_Pin, GPIO_PIN_RESET);
		break;
	case 39:
		HAL_GPIO_WritePin(GPIOA,ENM5_Pin, GPIO_PIN_RESET);
		break;
	case 40:
		HAL_GPIO_WritePin(GPIOA,ENM6_Pin, GPIO_PIN_RESET);
		break;
	case 41:
		HAL_GPIO_WritePin(GPIOA,ENM7_Pin, GPIO_PIN_RESET);
		break;
	case 42:
		HAL_GPIO_WritePin(GPIOA,ENM4_Pin, GPIO_PIN_RESET);
		break;
	case 43:
		HAL_GPIO_WritePin(GPIOA,ENM5_Pin, GPIO_PIN_RESET);
		break;
	case 44:
		HAL_GPIO_WritePin(GPIOA,ENM6_Pin, GPIO_PIN_RESET);
		break;
	case 45:
		HAL_GPIO_WritePin(GPIOA,ENM7_Pin, GPIO_PIN_RESET);
		break;
	case 46:
		HAL_GPIO_WritePin(GPIOA,ENM5_Pin, GPIO_PIN_RESET);
		break;
	case 47:
		HAL_GPIO_WritePin(GPIOA,ENM6_Pin, GPIO_PIN_RESET);
		break;
	case 48:
		HAL_GPIO_WritePin(GPIOA,ENM7_Pin, GPIO_PIN_RESET);
		break;
	case 49:
		HAL_GPIO_WritePin(GPIOA,ENM6_Pin, GPIO_PIN_RESET);
		break;
	case 50:
		HAL_GPIO_WritePin(GPIOA,ENM7_Pin, GPIO_PIN_RESET);
		break;
	case 51:
		HAL_GPIO_WritePin(GPIOA,ENM7_Pin, GPIO_PIN_RESET);
		break;
	default:
		break;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	timer1Run();
	timer2Run();
	timer3Run();
	timer4Run();
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
