/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define CE_PORT		GPIOB		// PB6 chip enable (aka slave select)
#define CE_PIN 		GPIO_PIN_6

#define DC_PORT 	GPIOA		// PA0 data/control
#define DC_PIN 		GPIO_PIN_0

#define RESET_PORT	GPIOA		// PA1 reset
#define RESET_PIN   GPIO_PIN_1

#define GLCD_WIDTH 84
#define GLCD_HEIGHT 48
#define NUM_BANKS 6

#define ME_PORT    	GPIOB		// PB10 motor enable
#define ME_PIN		GPIO_PIN_10
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
// Character Table:
const char font_table[][6] = {
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // space
		{0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00}, // 'A'
		{0x7F, 0x49, 0x49, 0x49, 0x36, 0x00}, // 'B'
		{0x3E, 0x41, 0x41, 0x41, 0x22, 0x00}, // 'C'
		{0x7F, 0x41, 0x41, 0x21, 0x3E, 0x00}, // 'D'
		{0x7F, 0x49, 0x49, 0x29, 0x41, 0x00}, // 'E'
		{0x7F, 0x09, 0x09, 0x09, 0x01, 0x00}, // 'F'
		{0x7F, 0x81, 0x91, 0x51, 0x71, 0x00}, // 'G'
		{0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00}, // 'H'
		{0x41, 0x41, 0x7F, 0x41, 0x41, 0x00}, // 'I'
		{0x31, 0x41, 0x61, 0x1F, 0x01, 0x00}, // 'J'
		{0x7F, 0x08, 0x14, 0x22, 0x41, 0x00}, // 'K'
		{0x7F, 0x40, 0x40, 0x40, 0x40, 0x00}, // 'L'
		{0x7E, 0x03, 0x7E, 0x01, 0x7E, 0x00}, // 'M'
		{0x7F, 0x02, 0x1C, 0x20, 0x7F, 0x00}, // 'N'
		{0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00}, // 'O'
		{0x7F, 0x09, 0x09, 0x09, 0x06, 0x00}, // 'P'
		{0x3E, 0x41, 0x41, 0x61, 0x7E, 0x80}, // 'Q'
		{0x7F, 0x09, 0x19, 0x29, 0x46, 0x00}, // 'R'
		{0x26, 0x49, 0x49, 0x49, 0x32, 0x00}, // 'S'
		{0x01, 0x01, 0x7F, 0x01, 0x01, 0x00}, // 'T'
		{0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00}, // 'U'
		{0x07, 0x38, 0x40, 0x38, 0x07, 0x00}, // 'V'
		{0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00}, // 'W'
		{0x41, 0x36, 0x08, 0x36, 0x41, 0x00}, // 'X'
		{0x01, 0x06, 0x78, 0x06, 0x01, 0x00}, // 'Y'
		{0x61, 0x51, 0x49, 0x45, 0x43, 0x00}, // 'Z'
		{0x60, 0x60, 0x00, 0x00, 0x00, 0x00}, // '.'
		{0x6F, 0x6F, 0x00, 0x00, 0x00, 0x00}, // '!'
		{0x06, 0x51, 0x09, 0x06, 0x00, 0x00}, // '?'
		{0x00, 0x00, 0x7E, 0x91, 0xAD, 0xA1}, // 'smiley left'
		{0xA1, 0xAD, 0x91, 0x7E, 0x00, 0x00}  // 'smiley right'
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void SPI_write(unsigned char data);
void GLCD_data_write(unsigned char data);
void GLCD_command_write(unsigned char data);
void GLCD_init(void);
void GLCD_setCursor(unsigned char x, unsigned char y);
void GLCD_clear(void);
void GLCD_putchar(int font_table_row);
void MOTOR_enable(int enable);
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
  MX_SPI1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  // Display Welcome Message
  GLCD_init(); // initialize the screen
  GLCD_clear(); // clears the screen
  GLCD_putchar(23); // W
  GLCD_putchar(1); // A
  GLCD_putchar(19); // S
  GLCD_putchar(8); // H
  GLCD_putchar(0); // ' '
  GLCD_putchar(20); // T
  GLCD_putchar(9); // I
  GLCD_putchar(13); // M
  GLCD_putchar(5); // E
  GLCD_putchar(28); // !
  GLCD_putchar(30); // smiley left
  GLCD_putchar(31); // smiley right
  HAL_Delay(2000); // Pause for 2 seconds

  // Activate PWM to drive the motor:
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  MOTOR_enable(0);
  int PWM_PERIOD = 80000; // how many clock cycles for each PWM period
  float duty_cycle = 0.99;
  TIM1->CCR1 = (int) (duty_cycle * PWM_PERIOD); // store new duty cycle

  // Wash Cycle
  GLCD_clear(); // clears the screen
  GLCD_putchar(23); // W
  GLCD_putchar(1); // A
  GLCD_putchar(19); // S
  GLCD_putchar(8); // H

  for(int i = 0; i < 9; i++){
	  MOTOR_enable(1);
	  HAL_Delay(750); // Agitation from repeated start and stop
	  MOTOR_enable(0);
	  HAL_Delay(250);
  }

  // Rinse Cycle
  GLCD_clear(); // clears the screen
  GLCD_putchar(18); // R
  GLCD_putchar(9); // I
  GLCD_putchar(14); // N
  GLCD_putchar(19); // S
  GLCD_putchar(5); // E

  for(int i = 0; i < 9; i++){
  	  MOTOR_enable(1);
  	  HAL_Delay(750); // Agitation from repeated start and stop
  	  MOTOR_enable(0);
  	  HAL_Delay(250);
  }
  // Spin Cycle
  GLCD_clear(); // clears the screen
  GLCD_putchar(19); // S
  GLCD_putchar(16); // P
  GLCD_putchar(9); // I
  GLCD_putchar(14); // N

  MOTOR_enable(1);
  HAL_Delay(10000);
  MOTOR_enable(0);

  // Finished Message
  GLCD_clear(); // clears the screen
  GLCD_putchar(1); // A
  GLCD_putchar(12); // L
  GLCD_putchar(12); // L
  GLCD_putchar(0); // ' '
  GLCD_putchar(4); // D
  GLCD_putchar(15); // O
  GLCD_putchar(14); // N
  GLCD_putchar(5); // E
  GLCD_putchar(28); // !
  GLCD_putchar(30); // smiley left
  GLCD_putchar(31); // smiley right
  HAL_Delay(2000); // Pause for 2 seconds
  GLCD_clear(); // clears the screen
  /* USER CODE END 2 */

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|Motor_Enable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 Motor_Enable_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_10|Motor_Enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// Uses SPI to write to the GLCD screen:
void SPI_write(unsigned char data){

	// Chip Enable (low is asserted)
	HAL_GPIO_WritePin(CE_PORT, CE_PIN, GPIO_PIN_RESET);

	// Send data over SPI1
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &data, 1, HAL_MAX_DELAY);

	// Chip Disable
	HAL_GPIO_WritePin(CE_PORT, CE_PIN, GPIO_PIN_SET);
}

// Writes data to GLCD screen:
void GLCD_data_write(unsigned char data){

	// Switch to 'data' mode (D/C pin high)
	HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);

	// Send data over SPI
	SPI_write(data);
}

// Writes command to GLCD screen:
void GLCD_command_write(unsigned char data){

	// Switch to 'command' mode (D/C pin low)
	HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET);

	// Send data over SPI
	SPI_write(data);
}

// Initializes GLCD screen:
void GLCD_init(void){

	// Keep CE high when not transmitting
	HAL_GPIO_WritePin(CE_PORT, CE_PIN, GPIO_PIN_SET);

	// Reset the screen (low pulse - down and up)
	HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, GPIO_PIN_SET);

	// Configure the screen (according to the datasheet)
	GLCD_command_write(0x21); // enter extended command mode
	GLCD_command_write(0xB0); // set LCD Vop for contrast
	GLCD_command_write(0x04); // set temp coefficient
	GLCD_command_write(0x15); // set LCD bias mode
	GLCD_command_write(0x20); // return to normal command mode
	GLCD_command_write(0x0C); // set display mode normal
}

// Sets screen cursor position:
void GLCD_setCursor(unsigned char x, unsigned char y){
	GLCD_command_write(0x80 | x); // column
	GLCD_command_write(0x40 | y); // bank
}

// Clears screen:
void GLCD_clear(void){
	int i;
	for(i = 0; i < (GLCD_WIDTH * NUM_BANKS); i++){
		GLCD_data_write(0x00); // write zeros
	}
	GLCD_setCursor(0,0); // return cursor to top left
}

// Writes character to screen:
void GLCD_putchar(int font_table_row){
	int i;
	for (i = 0; i < 6; i++){
		GLCD_data_write(font_table[font_table_row][i]);
	}
}

// Enable motor (enable: 1 -> enable, 0 -> disable):
void MOTOR_enable(int enable){
	if (enable){
		HAL_GPIO_WritePin(ME_PORT, ME_PIN, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(ME_PORT, ME_PIN, GPIO_PIN_RESET);
	}
}

// ISR triggered when lid is opened:
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 if(GPIO_Pin == GPIO_PIN_4)
 {
	 MOTOR_enable(0);
 while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 1);
 	 MOTOR_enable(1);
 }
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
