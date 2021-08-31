/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"	
#include "bsp_dht11.h"
#include "R305.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "rc522.h"
#include "STM_MY_LCD16X2.h"
#include "fatfs_sd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/// RFID
unsigned char CardID[5];
unsigned char MyID1[5] = {0x87, 0xD5, 0x89, 0xC9, 0x12};
unsigned char MyID2[5] = {0x67, 0xD1, 0x01, 0xD9, 0x6E};
unsigned char MyID3[5] = {0x09, 0xA1, 0x51, 0xB3, 0x4A};
uint8_t dataUDP_permit[]="Ma the hop le ";
uint8_t dataUDP_deny[]="Ma the khong hop le ";
char abd[50];
///THOI GIAN
uint8_t thu;
//VANTAY
uint8_t id=0;
int8_t y;
//CAC KY TU GUI LEN UART
char TG[40],TIMENAME[50];
char ngay[40],gio[40];
char name1[10]="Dung";
char name23[10]="Viet";
char name4[10]="Hung";
char n[3]="\n";
////KHAI BAO SDCARD
FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
UINT br, bw; 
char buffer[100];
char hoang[50] ="vanchhshsjosdosdoo";
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
volatile uint16_t a[2];
volatile uint16_t b[2];
 RTC_TimeTypeDef sTime;
 RTC_DateTypeDef sDate;
 RTC_AlarmTypeDef sAlarm;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_UART4_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */
#ifdef __GNUC__
 /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
 #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
 #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
 
 /**
 * @brief Retargets the C library printf function to the USART.
 * @param None
 * @retval None
 */
PUTCHAR_PROTOTYPE 
{
 /* Place your implementation of fputc here */
 /* e.g. write a character to the USART */
 HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 100);


 return ch;
}
void send_uart (char *string)		// CT CON GUI UART CAC GTRI NHDO
{
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart6, (uint8_t *) string, len, 1000);  
}
void send_uart1 (char *string)		// CT CON GUI UART TEN DE DIEM DANH
{
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart4, (uint8_t *) string, len, 1000);  
}
void modevantay()///LCD
{LCD1602_clear();
LCD1602_1stLine();
LCD1602_print("DiemDanhVanTay");
}
void moderfid()///LCD
{LCD1602_clear();
LCD1602_1stLine();
LCD1602_print("DiemDanhRFID");
}
void modecontrol()
{
LCD1602_clear();
LCD1602_1stLine();
LCD1602_print("Garden-mode");
}

void tatthietbi()//////// mode 01
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,0);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
}

void get_time()//laytg
{		HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
		thu=sDate.WeekDay +1;
		HAL_Delay(200);
}
void themvantay()
{
	if(verifyPassword() == 1 )
			{
				if(fingerEnroll(6) == 1)
										{
												HAL_Delay(1000);
										}
										else 
										{

										HAL_Delay(1500);
										}
}
}
void set_time()
{ sTime.Hours = 8;
  sTime.Minutes = 40;
  sTime.Seconds = 0;
	HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	//set date
	sDate.WeekDay = 4;
  sDate.Month = 8;
  sDate.Date = 12;
  sDate.Year = 21;
	HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
}
void hienthidefaut()////LCD
{
	LCD1602_clear();
	get_time();
	sprintf(gio,"%02d.%02d.%02d",sTime.Hours,sTime.Minutes,sTime.Seconds);
	sprintf(ngay,"T%d-%02d-%02d-%02d ",thu,sDate.Date,sDate.Month,sDate.Year);
	LCD1602_1stLine();
	LCD1602_print(gio);
	LCD1602_2ndLine();
	LCD1602_print(ngay);
	HAL_Delay(2000);
}
int bufsize (char *buf)/////ham dem ky tu
{
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}
void luusdcard()
{
	fres = f_mount(&fs, "", 0);
fres = f_open(&fil, "log-file.txt", FA_OPEN_APPEND | FA_WRITE | FA_READ);
fres = f_write(&fil, TIMENAME, bufsize(TIMENAME), &bw);
fres = f_close(&fil);
f_mount(NULL, "", 1);
}
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
DHT11_DATA_TypeDef DHT11_DATA ;
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  MX_USART6_UART_Init();
  MX_UART4_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	set_time();
	DHT11_Init();
	UART3_FINGER_Init();
	LCD1602_Begin4BIT(RS_GPIO_Port,RS_Pin,E_Pin,D4_GPIO_Port,D4_Pin,D5_Pin,D6_Pin,D7_Pin);
	LCD1602_noCursor();
	LCD1602_noBlink();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_7)==0 && HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9)==0)/////MODE gioithieu
		{
			hienthidefaut();
		}
		
	   if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_7)==0 && HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9)==1)/////MODE VANTAY
		{
		tatthietbi();
		modevantay();

		//		HAL_Delay(1000);
					if(verifyPassword() == 1 )
						{ 
							y = fingerIDSearch();  // ID
							if(y == -1)
							{
								// LOI
								LCD1602_setCursor(2,1);
								LCD1602_print("loi                ");
								HAL_Delay(1000);
							}
							else if (y == -2)
							{
								// KHONG TIM THAY
								LCD1602_setCursor(2,1);
								LCD1602_print("loi ko tim thay ");
								HAL_Delay(1000);
							}
							else 
							{				
								get_time();
								sprintf(TG,".%02d.%02d.%02d-T%d-%02d-%02d-%02d ",sTime.Hours,sTime.Minutes,sTime.Seconds,thu,sDate.Date,sDate.Month,sDate.Year);
								if(y==1 || y==0)
								{strcpy(TIMENAME, name1);	// NOI CHUOI TEN VA THOI GIAN
									strcat(TIMENAME, TG);
									send_uart1(TIMENAME);
									strcat(TIMENAME, n);
									luusdcard();
									LCD1602_setCursor(2,1);
									LCD1602_print("found-Dung      ");	
									HAL_Delay(2000);
								}
							
							  if(y==2 || y==3)
								{strcpy(TIMENAME, name23);	// NOI CHUOI TEN VA THOI GIAN
									strcat(TIMENAME, TG);
									send_uart1(TIMENAME);
									strcat(TIMENAME, n);
									luusdcard();
									LCD1602_setCursor(2,1);
									LCD1602_print("found-Viet      ");
									HAL_Delay(2000);
								}
								if(y==4 || y==5 || y==6)
								{strcpy(TIMENAME, name4);	// NOI CHUOI TEN VA THOI GIAN
									strcat(TIMENAME, TG);
									send_uart1(TIMENAME);
									strcat(TIMENAME, n);
									luusdcard();
									LCD1602_setCursor(2,1);
									LCD1602_print("found-Hung      ");
									HAL_Delay(2000);
								}
							}
					}
	}
	else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_7)==1 && HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9)==1)
	{
		modecontrol();
		if(Read_TempAndHumidity (&DHT11_DATA)==SUCCESS)
		{
		HAL_ADC_Start_DMA(&hadc1,(uint32_t*)a,2);
		HAL_Delay(500);
		}
		else 
		{LCD1602_print("loi me r");
		}
		b[1]=(((a[1]-2100)*100)/1995)-1;
		b[0]=(((a[0]-20)*100)/4075)-1;
			sprintf(abd, "%02d-%02d-%02d-%02d",b[0],b[1],DHT11_DATA.temp_int,DHT11_DATA.humi_int);
			send_uart(abd);
			HAL_Delay(2000);
		if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)==0) //che do tu dong
		{
				if(b[1]>60)////MAY BOM
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,1);
				else
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,0);
				if(b[0]>65)///DEN
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
				else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
				if(DHT11_DATA.temp_int>33)///QUAT
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,1);
				else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
		}
		else if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)==1) //che do control
		{
			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)==1)////may bom
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,1);
			else if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)==0)
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,0);
			
			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5)==1)//////den
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
			else if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5)==0)
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
			
			if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6)==1)//////quat
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,1);
			else if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6)==0)
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
		}
	}	
		else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_7)==1 && HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9)==0)///mode rfid
		{ 
			moderfid();
			MFRC522_Init();
			if (MFRC522_Check(CardID) == MI_OK) ///RFID
			{				
				if (MFRC522_Compare(CardID, MyID1) == MI_OK) 
				{
				get_time();
				sprintf(TG,".%02d.%02d.%02d-T%d-%02d-%02d-%02d ",sTime.Hours,sTime.Minutes,sTime.Seconds,thu,sDate.Date,sDate.Month,sDate.Year);
					LCD1602_setCursor(2,1);
					LCD1602_print("found-Dung      ");					
					strcpy(TIMENAME, name1);	// NOI CHUOI TEN VA THOI GIAN
					strcat(TIMENAME, TG);
					send_uart1(TIMENAME);
					strcat(TIMENAME, n);
					luusdcard();
					HAL_Delay(2000);
				}
				else if (MFRC522_Compare(CardID, MyID2) == MI_OK)
				{
				get_time();
				sprintf(TG,".%02d.%02d.%02d-T%d-%02d-%02d-%02d ",sTime.Hours,sTime.Minutes,sTime.Seconds,thu,sDate.Date,sDate.Month,sDate.Year);
					LCD1602_setCursor(2,1);
					LCD1602_print("found-Viet      ");	
					strcpy(TIMENAME, name23);	// NOI CHUOI TEN VA THOI GIAN
					strcat(TIMENAME, TG);
					send_uart1(TIMENAME);
					strcat(TIMENAME, n);
					luusdcard();
					HAL_Delay(2000);
				}
				else if (MFRC522_Compare(CardID, MyID3) == MI_OK)
				{
				get_time();
				sprintf(TG,".%02d.%02d.%02d-T%d-%02d-%02d-%02d ",sTime.Hours,sTime.Minutes,sTime.Seconds,thu,sDate.Date,sDate.Month,sDate.Year);
					LCD1602_setCursor(2,1);
					LCD1602_print("found-Hung      ");	
					strcpy(TIMENAME, name4);	// NOI CHUOI TEN VA THOI GIAN
					strcat(TIMENAME, TG);
					send_uart1(TIMENAME);
					strcat(TIMENAME, n);
					luusdcard();
					HAL_Delay(2000);
				}
				else
				{
					LCD1602_setCursor(2,1);
					LCD1602_print("not found       ");
					HAL_Delay(2000);
				}
			}
				 					 
		  HAL_Delay(2000);  
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
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
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 23;
  sTime.Minutes = 59;
  sTime.Seconds = 45;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

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
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 57600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 57600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 57600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, D4_Pin|D5_Pin|D6_Pin|D7_Pin
                          |GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, RS_Pin|E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PE7 PE9 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : D4_Pin D5_Pin D6_Pin D7_Pin
                           PD12 */
  GPIO_InitStruct.Pin = D4_Pin|D5_Pin|D6_Pin|D7_Pin
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD3 PD4 PD5 PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : RS_Pin E_Pin */
  GPIO_InitStruct.Pin = RS_Pin|E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
