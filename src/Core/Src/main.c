/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//Default library for JPEG images encoding and decoding
//#include "libjpeg.h"
#include "app_touchgfx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stm32746g_discovery_qspi.h>
#include "stdio.h"

//CMSIS ARM math library for optimized mathematical operations in Cortex-M
#include "arm_math.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

//"Defines" to use in the reading function of the MAX6675 module

//Set the state of pin "PIO (D10)" to a low state (reset)
#define CSen HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0, GPIO_PIN_RESET);
//Set the state of pin "PIO (D10)" to a high state (set)
#define CSdis HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0, GPIO_PIN_SET);
//Set the state of pin "PI1 (D13)" to a high state (set)
#define SCK_H HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1, GPIO_PIN_SET);
//Set the state of pin "PI1 (D13)" to a low state (reset)
#define SCK_L HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1, GPIO_PIN_RESET);

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define REFRESH_COUNT        1835

#define SDRAM_TIMEOUT                            ((uint32_t)0xFFFF)
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

CRC_HandleTypeDef hcrc;

DMA2D_HandleTypeDef hdma2d;

I2C_HandleTypeDef hi2c3;

LTDC_HandleTypeDef hltdc;

QSPI_HandleTypeDef hqspi;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim4;

SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */
static FMC_SDRAM_CommandTypeDef Command;

//Variable to receive temperature reading function
volatile float heatVal = 0;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_DMA2D_Init(void);
static void MX_FMC_Init(void);
static void MX_I2C3_Init(void);
static void MX_LTDC_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

//Function to calculate reflow curve
 void calculateReflowCurve();
 void calculateReflowCurve2();
 void calculateReflowCurve3();
 void calculateReflowCurve4();

//Declaration of function to reading temperature with MAX6675 module
float heatf();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//To indicate the current version of the system
#define config_version 130

//Struct "config" with the parameters to reflow process
typedef struct {

	float32_t KP;
	float32_t Ki;
	float32_t KD;

	float32_t firstHeatUpRate;
	uint32_t SoakTempeture;
	uint32_t SoakTime;
	float32_t secondHeatUpRate;
	uint32_t ReflowTempeture;
	uint32_t ReflowTime;

	float32_t firstHeatUpRate2;
	uint32_t SoakTempeture2;
	uint32_t SoakTime2;
	float32_t secondHeatUpRate2;
	uint32_t ReflowTempeture2;
	uint32_t ReflowTime2;

	float32_t firstHeatUpRate3;
	uint32_t SoakTempeture3;
	uint32_t SoakTime3;
	float32_t secondHeatUpRate3;
	uint32_t ReflowTempeture3;
	uint32_t ReflowTime3;

	float32_t firstHeatUpRate4;
	uint32_t SoakTempeture4;
	uint32_t SoakTime4;
	float32_t secondHeatUpRate4;
	uint32_t ReflowTempeture4;
	int32_t ReflowTime4;


	uint32_t version;

}	config;

// Declaration of the "config" structure named ReflowParameters, containing parameters related to reflow process
config ReflowParameters;

//Array with 4000 elements to each Reflow Curve
volatile uint8_t ReflowCurve[4000];
volatile uint8_t ReflowCurve2[4000];
volatile uint8_t ReflowCurve3[4000];
volatile uint8_t ReflowCurve4[4000];

//Variable to store current duty cycle
float duty;

//Set point variable used in pid error
volatile uint8_t setpoint;

//Variable to store current pid error
float pid_error;

//Variable auxiliary
volatile int teste=0;

//Index variables
volatile uint16_t ind = 0;
volatile uint16_t ind2 = 0;
volatile uint16_t ind3 = 0;
volatile uint16_t ind4 = 0;

//Control variable to Calculate Reflow curve
volatile uint8_t variavel_controle = 0;

volatile uint8_t cont = 1;

// Declaration of an ARM floating-point PID controller instance named PID
arm_pid_instance_f32 PID;

//Variable to activate the process
volatile uint8_t ReflowEnable = 0;

//Variable for index reflow process
volatile uint16_t ReflowIndex = 0;

//Control variable for process steps
volatile uint16_t PhaseIndex[5]={0};
volatile uint16_t PhaseIndex2[5]={0};
volatile uint16_t PhaseIndex3[5]={0};
volatile uint16_t PhaseIndex4[5]={0};


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

int main(void)
{

	/* USER CODE BEGIN 1 */


	if (!(ReflowParameters.version == config_version))	{

			// Lead Sn/Pb/Ag 1%  - PROFILE 1
			ReflowParameters.firstHeatUpRate = 0.6;
			ReflowParameters.SoakTime = 150;
			ReflowParameters.SoakTempeture = 150;
			ReflowParameters.secondHeatUpRate = 0.6;
			ReflowParameters.ReflowTime = 41;
			ReflowParameters.ReflowTempeture = 230;

			// Sn63/Pb37 - https://www.chipquik.com/datasheets/TS391AX50.pdf - PROFILE 2
			ReflowParameters.firstHeatUpRate2 = 0.6;
			ReflowParameters.SoakTime2 = 100;
			ReflowParameters.SoakTempeture2 = 150;
			ReflowParameters.secondHeatUpRate2 = 0.6;
			ReflowParameters.ReflowTime2 = 100;
			ReflowParameters.ReflowTempeture2 = 230;

			// Sn96.5/Ag3.0/Cu0.5 - https://www.chipquik.com/datasheets/TS991SNL35T4.pdf - PROFILE 3
			ReflowParameters.firstHeatUpRate3 = 0.6;
			ReflowParameters.SoakTime3 = 100;
			ReflowParameters.SoakTempeture3 = 175;
			ReflowParameters.secondHeatUpRate3 = 0.6;
			ReflowParameters.ReflowTime3 = 100;
			ReflowParameters.ReflowTempeture3 = 240;

			// Sn42/Bi57.6/Ag0.4 - http://www.chipquik.com/datasheets/TS391LT50.pdf - PROFILE 4
			ReflowParameters.firstHeatUpRate4 = 0.6;
			ReflowParameters.SoakTime4 = 100;
			ReflowParameters.SoakTempeture4 = 130;
			ReflowParameters.secondHeatUpRate4 = 0.6;
			ReflowParameters.ReflowTime4 = 100;
			ReflowParameters.ReflowTempeture4 = 165;


			//KP. Ki, KD by Pedro (empirical method) (23.11.2023)

//			ReflowParameters.KP = 220;
//			ReflowParameters.Ki = 0.05;
//			ReflowParameters.KD = 210;

			//KP, Ki, KD by Ziegler-Nichols method (PID classic) to acquisiton time (1000ms) (04.12.2023)

//			ReflowParameters.KP = 240;
//			ReflowParameters.Ki = 4.8;
//			ReflowParameters.KD = 3000;

			//KP, Ki, KD by Ziegler-Nichols method (PID classic) to acquisiton time (1500ms) (04.12.2023)

			ReflowParameters.KP = 242;
			ReflowParameters.Ki = 7.2;
			ReflowParameters.KD = 2000;

			//Assign the value of config_version to the version member of the ReflowParameters structure
			ReflowParameters.version = config_version;

		}

	      //Call the function to calculate the reflow curve
		  calculateReflowCurve();
	  	  calculateReflowCurve2();
	  	  calculateReflowCurve3();
	  	  calculateReflowCurve4();

		//Assign the proportional (Kp), integral (Ki), and derivative (Kd) constants from ReflowParameters to the PID controller instance
		PID.Kp = ReflowParameters.KP;
		PID.Ki = ReflowParameters.Ki;
		PID.Kd = ReflowParameters.KD;

		//Initialize the PID controller instance (PID) with the assigned constants and a reset state (1)
		arm_pid_init_f32(&PID, 1);


  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_QUADSPI_Init();
  //MX_LIBJPEG_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_TouchGFX_Init();
  /* USER CODE BEGIN 2 */

  //Start the PWM signal generation on TIM1, specifically on channel 1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  //Set the capture/compare value for TIM1, channel 1, to 10
  htim1.Instance->CCR1 = 10;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



  while (1) //ever
  {
    /* USER CODE END WHILE */

  MX_TouchGFX_Process();

  /* USER CODE BEGIN 3 */

  	  	  	//Logic to configure which curve will be used in the control
  			if(variavel_controle == 1) setpoint = ReflowCurve[ReflowIndex];
  			if(variavel_controle == 2) setpoint = ReflowCurve2[ReflowIndex];
  			if(variavel_controle == 3) setpoint = ReflowCurve3[ReflowIndex];
  			if(variavel_controle == 4) setpoint = ReflowCurve4[ReflowIndex];


   	   if (ReflowEnable == 1) {

   		   	  //Variable heatVal receives the value in the read function
   		  	  heatVal = heatf();
   		  	  //Wait 1500ms
   		   	  HAL_Delay(1500);

   		   	    //Proportional error subtracted from the current heat value (heatVal) of the setpoint
   				pid_error = setpoint - heatVal;
   		   	   //Actuator size
   				duty =  arm_pid_f32(&PID, pid_error);

   				//Ensures that the duty cycle remains within a valid range. If the duty exceeds 1000, it will be limited to 1000
   				if (duty > 1000) {
   					duty = 1000;
   					PID.Ki = 0;
   				} else if (duty < 0) {
   					duty = 0;
   				}else{
   					PID.Ki = ReflowParameters.Ki;
   				}

   				//Dutycycle customize
   				htim1.Instance->CCR1 = 1000 - (uint16_t)duty;

   				ReflowIndex++;

   				//Finish condition
   				if (ReflowIndex == PhaseIndex[4])
   				{
   						ReflowEnable = 0;
   						//Variable heatVal receives the value in the read function
   						heatVal = heatf();
   						//Wait 180ms
   						HAL_Delay(180);
   				}
   	   }

   	   if (ReflowEnable == 0){
   		   	   	ReflowIndex = 0;
   				htim1.Instance->CCR1 = 1000;
   				duty = 1000;
   				//Variable heatVal receives the value in the read function
   				heatVal = heatf();
   				//Wait 1500ms
   				HAL_Delay(180);

   			}
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x00C0EAFF;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 40;
  hltdc.Init.VerticalSync = 9;
  hltdc.Init.AccumulatedHBP = 53;
  hltdc.Init.AccumulatedVBP = 11;
  hltdc.Init.AccumulatedActiveW = 533;
  hltdc.Init.AccumulatedActiveH = 283;
  hltdc.Init.TotalWidth = 565;
  hltdc.Init.TotalHeigh = 285;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0xC0000000;
  pLayerCfg.ImageWidth = 480;
  pLayerCfg.ImageHeight = 272;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 1;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = 24;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_6_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */
  BSP_QSPI_Init();

  BSP_QSPI_MemoryMappedMode();
  HAL_NVIC_DisableIRQ(QUADSPI_IRQn);
  /* USER CODE END QUADSPI_Init 2 */

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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 32000;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
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
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
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
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 64000;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 500;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 3;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */
  __IO uint32_t tmpmrd = 0;

    /* Step 1: Configure a clock configuration enable command */
    Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
    Command.CommandTarget          =  FMC_SDRAM_CMD_TARGET_BANK1;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);

    /* Step 2: Insert 100 us minimum delay */
    /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
    HAL_Delay(1);

    /* Step 3: Configure a PALL (precharge all) command */
    Command.CommandMode            = FMC_SDRAM_CMD_PALL;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);

    /* Step 4: Configure an Auto Refresh command */
    Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    Command.AutoRefreshNumber      = 8;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);

    /* Step 5: Program the external memory mode register */
    tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 | \
             SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL    | \
             SDRAM_MODEREG_CAS_LATENCY_3            | \
             SDRAM_MODEREG_OPERATING_MODE_STANDARD  | \
             SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = tmpmrd;

    /* Send the command */
    HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);

    /* Step 6: Set the refresh rate counter */
    /* Set the device refresh rate */
    HAL_SDRAM_ProgramRefreshRate(&hsdram1, REFRESH_COUNT);

    //Deactivate speculative/cache access to first FMC Bank to save FMC bandwidth
    FMC_Bank1->BTCR[0] = 0x000030D2;
  /* USER CODE END FMC_Init 2 */
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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1|GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LCD_BL_CTRL_Pin */
  GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PI1 LCD_DISP_Pin PI0 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|LCD_DISP_Pin|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

//Function to read temperature from MAX6675 and return the temperature in Celsius
float heatf()
{

	float heat;        //Variable to store temperature
	uint16_t data[16]; //Array to store the 16 bits of data read from MAX6675


	CSen;  // Enable MAX6675 by setting chip select (CS) pin low
	SCK_L; // Ensure clock (SCK) is initially low

	//This range is
	for (int x = 15; x >= 0; x--)
	{

		SCK_H;                                          // Set clock high to read a bit
		data[x] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14); // Read the bit and store it
		SCK_L;                                          // Set clock low for the next bit

	}

	CSdis; // Disable MAX6675 by setting chip select (CS) pin high

	// Calculate the temperature from the received bits
	heat = data[14]*2048 + data[13]*1024 + data[12]*512 + data[11]*256 + data[10]*128+ data[9]*64+ data[8]*32+ data[7]*16+ data[6]*8+ data[5]*4 + data[4]*2 + data[3];

	// Convert the raw temperature to Celsius (0.25 degrees resolution)
	return heat*0.25;
}


/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */

	if (htim->Instance == TIM6){

	HAL_IncTick();

	}
}


//Function to calculate reflow curve
void calculateReflowCurve()
{
	//Initialize the ReflowCurve array with zeros
	for(int i =0;i<4000;i++)
	{
		ReflowCurve[i]=0;
	}

	// Define the time step for the reflow curve calculation
	float timestep = 0.5;

	//First Heat Up:
	while (24 + timestep * ReflowParameters.firstHeatUpRate <= ReflowParameters.SoakTempeture)
	{
	ReflowCurve[ind] = 24 + timestep * ReflowParameters.firstHeatUpRate;
	ind++;
	timestep = timestep + 0.5;
	}

	PhaseIndex[1]=ind;

	//Soak
	int Soakduration = ReflowParameters.SoakTime;

	for(int i=0; i < Soakduration;i++){
		ReflowCurve[ind+i]=ReflowParameters.SoakTempeture;
	}

	//Second Heat Up
	ind = ind + Soakduration;
	PhaseIndex[2]=ind;
	timestep = 0.5;

	while (ReflowParameters.SoakTempeture + timestep * ReflowParameters.secondHeatUpRate <= ReflowParameters.ReflowTempeture)
	{
	ReflowCurve[ind] = ReflowParameters.SoakTempeture + (uint8_t)timestep * ReflowParameters.secondHeatUpRate;
	ind++;
	timestep = timestep + 0.5;
	}

	PhaseIndex[3]=ind;


	//Reflow
	int Reflowduration = ReflowParameters.ReflowTime;

	for(int i=0; i < Reflowduration; i++)
	{
		ReflowCurve[ind+i]=ReflowParameters.ReflowTempeture;
	}

	ind = ind + Reflowduration;
	ReflowCurve[ind]=0;

	PhaseIndex[4]=ind;


	//Cooldown
	timestep = 0.5;
	while (ReflowParameters.ReflowTempeture - timestep * 1.8 >= 24)
	{
	ReflowCurve[ind] = ReflowParameters.ReflowTempeture - timestep * 1.8;
	ind++;
	timestep = timestep + 0.5;
	}

}


//Function to calculate Reflow Curve to PROFILE 2
void calculateReflowCurve2()
{
	//Initialize the ReflowCurve array with zeros
	for(int i =0;i<4000;i++)
	{
		ReflowCurve2[i]=0;
	}

	// Define the time step for the reflow curve calculation
	float timestep = 0.5;

	//First Heat Up:
	while (24 + timestep * ReflowParameters.firstHeatUpRate2 <= ReflowParameters.SoakTempeture2)
	{
	ReflowCurve2[ind2] = 24 + timestep * ReflowParameters.firstHeatUpRate2;
	ind2++;
	timestep = timestep + 0.5;
	}

	PhaseIndex2[1]=ind2;

	//Soak
	int Soakduration = ReflowParameters.SoakTime2;

	for(int i=0; i < Soakduration;i++){
		ReflowCurve2[ind2+i]=ReflowParameters.SoakTempeture2;
	}

	//Second Heat Up
	ind2 = ind2 + Soakduration;
	PhaseIndex2[2]=ind2;
	timestep = 0.5;

	while (ReflowParameters.SoakTempeture2 + timestep * ReflowParameters.secondHeatUpRate2 <= ReflowParameters.ReflowTempeture2)
	{
	ReflowCurve2[ind2] = ReflowParameters.SoakTempeture2 + (uint8_t)timestep * ReflowParameters.secondHeatUpRate2;
	ind2++;
	timestep = timestep + 0.5;
	}

	PhaseIndex2[3]=ind2;


	//Reflow
	int Reflowduration = ReflowParameters.ReflowTime2;

	for(int i=0; i < Reflowduration; i++)
	{
		ReflowCurve2[ind2+i]=ReflowParameters.ReflowTempeture2;
	}

	ind2 = ind2 + Reflowduration;
	ReflowCurve2[ind2]=0;

	PhaseIndex2[4]=ind2;


	//Cooldown
	timestep = 0.5;
	while (ReflowParameters.ReflowTempeture2 - timestep * 1.8 >= 24)
	{
	ReflowCurve2[ind2] = ReflowParameters.ReflowTempeture2 - timestep * 1.8;
	ind2++;
	timestep = timestep + 0.5;
	}

}

//Function to calculate Reflow Curve to PROFILE 3
void calculateReflowCurve3()
{
	//Initialize the ReflowCurve array with zeros
	for(int i =0;i<4000;i++)
	{
		ReflowCurve3[i]=0;
	}

	// Define the time step for the reflow curve calculation
	float timestep = 0.5;

	//First Heat Up:
	while (24 + timestep * ReflowParameters.firstHeatUpRate3 <= ReflowParameters.SoakTempeture3)
	{
	ReflowCurve3[ind3] = 24 + timestep * ReflowParameters.firstHeatUpRate3;
	ind3++;
	timestep = timestep + 0.5;
	}

	PhaseIndex3[1]=ind3;

	//Soak
	int Soakduration = ReflowParameters.SoakTime3;

	for(int i=0; i < Soakduration;i++){
		ReflowCurve3[ind3+i]=ReflowParameters.SoakTempeture3;
	}

	//Second Heat Up
	ind3 = ind3 + Soakduration;
	PhaseIndex3[2]=ind3;
	timestep = 0.5;

	while (ReflowParameters.SoakTempeture3 + timestep * ReflowParameters.secondHeatUpRate3 <= ReflowParameters.ReflowTempeture3)
	{
	ReflowCurve3[ind3] = ReflowParameters.SoakTempeture3 + (uint8_t)timestep * ReflowParameters.secondHeatUpRate3;
	ind3++;
	timestep = timestep + 0.5;
	}

	PhaseIndex3[3]=ind3;


	//Reflow
	int Reflowduration = ReflowParameters.ReflowTime3;

	for(int i=0; i < Reflowduration; i++)
	{
		ReflowCurve3[ind3+i]=ReflowParameters.ReflowTempeture3;
	}

	ind3 = ind3 + Reflowduration;
	ReflowCurve3[ind3]=0;

	PhaseIndex3[4]=ind3;


	//Cooldown
	timestep = 0.5;
	while (ReflowParameters.ReflowTempeture3 - timestep * 1.8 >= 24)
	{
	ReflowCurve3[ind3] = ReflowParameters.ReflowTempeture3 - timestep * 1.8;
	ind3++;
	timestep = timestep + 0.5;
	}

}


//Function to calculate Reflow Curve to PROFILE 4
void calculateReflowCurve4()
{
	//Initialize the ReflowCurve array with zeros
	for(int i =0;i<4000;i++)
	{
		ReflowCurve4[i]=0;
	}

	// Define the time step for the reflow curve calculation
	float timestep = 0.5;

	//First Heat Up:
	while (24 + timestep * ReflowParameters.firstHeatUpRate4 <= ReflowParameters.SoakTempeture4)
	{
	ReflowCurve4[ind4] = 24 + timestep * ReflowParameters.firstHeatUpRate4;
	ind4++;
	timestep = timestep + 0.5;
	}

	PhaseIndex4[1]=ind4;

	//Soak
	int Soakduration = ReflowParameters.SoakTime4;

	for(int i=0; i < Soakduration;i++){
		ReflowCurve4[ind4+i]=ReflowParameters.SoakTempeture4;
	}

	//Second Heat Up
	ind4 = ind4 + Soakduration;
	PhaseIndex4[2]=ind4;
	timestep = 0.5;

	while (ReflowParameters.SoakTempeture4 + timestep * ReflowParameters.secondHeatUpRate4 <= ReflowParameters.ReflowTempeture4)
	{
	ReflowCurve4[ind4] = ReflowParameters.SoakTempeture4 + (uint8_t)timestep * ReflowParameters.secondHeatUpRate4;
	ind4++;
	timestep = timestep + 0.5;
	}

	PhaseIndex4[3]=ind4;


	//Reflow
	int Reflowduration = ReflowParameters.ReflowTime4;

	for(int i=0; i < Reflowduration; i++)
	{
		ReflowCurve4[ind4+i]=ReflowParameters.ReflowTempeture4;
	}

	ind4 = ind4 + Reflowduration;
	ReflowCurve4[ind4]=0;

	PhaseIndex4[4]=ind4;


	//Cooldown
	timestep = 0.5;
	while (ReflowParameters.ReflowTempeture4 - timestep * 1.8 >= 24)
	{
	ReflowCurve4[ind4] = ReflowParameters.ReflowTempeture4 - timestep * 1.8;
	ind4++;
	timestep = timestep + 0.5;
	}

}





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

