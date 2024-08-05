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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"string.h"
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include "variables.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
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
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
#include "notes.h"





int _write(int le, char *ptr, int len)
{
int DataIdx;
for(DataIdx = 0; DataIdx < len; DataIdx++)
{
ITM_SendChar(*ptr++);
}
return len;
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM10_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void note_handling(uint8_t incoming_data);
void note_replace(uint8_t note_replace);
//void USB_CDC_RxHandler(uint8_t*, uint32_t);
void buttons_store(void);
void midi_send(void);  // creates midi_cue data
void flash_write(void);
void flash_read(void);
void panic_delete(void);



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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM10_Init();
  MX_USB_DEVICE_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

  HAL_SPI_Init(&hspi1); // write to register hspi2
  HAL_DMA_Init(&hdma_spi1_rx);
  	HAL_DMA_Init(&hdma_spi1_tx);


    HAL_TIM_Base_Start_IT(&htim10);
   // HAL_UART_Receive_IT(&huart1, &serial1_temp, 1);		// midi irq
    HAL_UART_Receive_DMA(&huart1, serial1_hold2,1);    //
  //CDC_Transmit_FS("Hello\r\n",7);

	printf("hello");
	//memcpy(scene_memory,test_data,32);
	//button_states[70]=1;

//	flash_write();
	//  get flash data
	flash_read();
	// panic_delete();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


 			if (seq_enable) {

 		//	if ((s_temp / 6) != (seq_pos / 6)) {    //send every 6, not good


 				//if (((s_temp+6) <= seq_pos) | ((s_temp==186)&(seq_pos<=0))) {
 						if ((s_temp>>3) != (seq_pos>>3)) {

 							uint8_t seq_step_mod=seq_step;
 							if((bar_looping) && (scene_buttons[0]==(bar_looping-1)))  seq_step_mod=((seq_step&7)+(pot_states[3]>>4))&31;  // looper visual

 				send_buffer[1] = square_buttons_list[seq_step_mod];   // for displaying 0-32 , reset previous light
 			//	if (button_states[send_buffer[1] ]) send_buffer[2]=5; else send_buffer[2]=0; // reset to previous state or 0
 				send_buffer[2]=button_states[send_buffer[1]]; // tis has to be 0 or 5
 				send_buffer[4] = square_buttons_list[((seq_step_mod + 1) & 31)] ;  // set new light on
 				send_buffer[5] = 1;
 			//	send_buffer[10]=drum_list[scene_buttons[0]];  // pitch


 			//	if(button_states[send_buffer[1]])	send_buffer[11]=scene_velocity[seq_step+(32*scene_buttons[0])]; else send_buffer[11]=0;  // velocity from scene

 				if(all_update){   // update all square scene lights
 					for (i=0;i<40;i++)  { send_all[i*3]=144;
 					send_all[(i*3)+1]=i;

 					send_all[(i*3)+2]=button_states[i];
 					
 					
 					}

 					CDC_Transmit_FS(send_all, 120); // send apc data , bit heavy
 					all_update=0;}


 				if ((button_states[64])& (!bar_looping)) {bar_looping=scene_buttons[0]+1;bar_count=seq_pos>>3;}
 			 			if (!button_states[64]) bar_looping=0;


 			midi_send();

 				uint8_t len=midi_cue[25];
 				uint8_t send_temp[35];
 				memcpy(send_temp,midi_cue,len);
 				memcpy(send_temp+len,send_buffer,9);  // midi first
 				len=len+9;
 			//	if (send_buffer[8]) len=len+9; else len=len+6;
 				CDC_Transmit_FS(send_temp, len); //send all if possible , after each step





 				printf(" %d\n ", seq_pos);

 				//		printf(" %d ", midi_cue[3]);
 				//		printf(" %d\n ", midi_cue[6]);

 				  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,led_blink);
 				  led_blink=!led_blink;

 				  flash_write(); // works only if button pressed

 				//  panic_delete();   // this will freeze if getting bad data constantly
 				 // printf(" %d \n", sys_cnt[3]);



 				//	send_buffer[13]=0; send_buffer[14]=0; // these are cleared each time
 				seq_step = (seq_step + 1) & 31;
 				s_temp = seq_pos;

 			}   // blink steps

 		}


 	  if ((seq_pos == 64) &&(mtc_tick)) {    // only if mtc is incoming

 		  if (mtc_tick > 64) {
 				timer_value = timer_value - (mtc_tick - 64);
 				mtc_tick = seq_pos;
 			} // chase mtc ,reasonable
 			if (mtc_tick < 64) {
 				timer_value = timer_value + (64 - mtc_tick);
 				mtc_tick = seq_pos;
 			}
 			if ((timer_value > 700) | (timer_value < 300))
 				timer_value = 512; // fix in case

 		} else seq_enable=1;  // if no mtc then just run sequencer

     // if ((seq_enable)&& (button_states[90])) {seq_enable=0;button_states[90]=0;}
    //  if ((!seq_enable)&& (button_states[90])) {seq_enable=1;button_states[90]=0;}

 	  if (serial1_hold[5])	{//printf("%d",timer_value);printf(" %d ",seq_pos);printf(" %d \n",mtc_tick);


 		  if (serial1_hold[0]>247) realtime=serial1_hold[0];
 		  else {realtime=0;message_cue[message_counter]=serial1_hold[0];message_counter++; }
 		  if(message_counter>2) message_counter=0;   // set to typical midi for now


 		  if (realtime==248 ) {realtime=0;
 		  if(mtc_tick==191) mtc_tick=0; else mtc_tick++;realtime=0;}   // 8/4 or 32/16
 		//  if (realtime==248 ) {mtc_tick++;realtime=0;printf(" %d",seq_pos);printf(" %d \n",mtc_tick);} //always running

 		  if (realtime==250 )
 					  {seq_enable=1;realtime=0;seq_pos=0;seq_step=0;mtc_tick=0;} // reset to 0 on real time message :start
 		  if (realtime==252)
 							  {seq_enable=0;
 							  send_buffer[5]=0;  // clear last step
 							  realtime=0; } // reset to 0 on real time message :stop



 		//  note_handling(serial1_hold[0]);note_replace(note_replace_enable);    // notes generate after each !!!
 		 // 	note_handling(serial1_hold[1]);note_replace(note_replace_enable);
 		 // 	note_handling(serial1_hold[2]);note_replace(note_replace_enable);

 	  serial1_hold[5]=0;serial1_hold[0]=0;
 	  //HAL_UART_Receive_IT(&huart1,&serial1_temp,1); // finish 4 bytes and blink , reset irq



 	  }  // end of midi in

 	  if (cdc_buffer[0]) {      //  when cdc buffer incoming

 			for (i=0;i<32;i++) {// scene memory fill from buttons each time a button is pressed
 				uint8_t data_temp=i+(scene_buttons[0]*32);


 			{	if (button_states [square_buttons_list[i]])  scene_memory[data_temp]	= (scene_velocity [data_temp]<<1)+scene_pitch[data_temp]; else scene_memory[data_temp]	= 0;}

 	  } // this needs fix
 		//	printf(" %d ",button_states[34]);printf(" %d ",button_states[35]);printf(" %d ",button_states[37]);printf(" %d ",scene_memory[3]);
 			// needs velocity and pitch too here


 			buttons_store();   // only runs after receive



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
  RCC_OscInitStruct.PLL.PLLN = 192;
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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 3905;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 512;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

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
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CS1_Pin */
  GPIO_InitStruct.Pin = CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS1_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1) { 	// midi irq

//		if (serial1_hold2[0]==248)  {	memcpy(serial1_hold,serial1_hold2,4)  ;   // copy mem

	//	serial1_hold[5]=1;seq_pos++;   HAL_UART_Receive_DMA(&huart1, serial1_hold2,1); }


//		else

		memcpy(serial1_hold,serial1_hold2,1)  ;   // copy mem

		serial1_hold[5]=1;   HAL_UART_Receive_DMA(&huart1, serial1_hold2,1);
		//HAL_UART_Receive_IT(&huart1,&serial1_temp,1);}  //enable read  or reset irq

	}




	//memcpy((serial_hold+8),(serial_hold2+8),8);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    // unreliable
	{
		if(TIM10==htim->Instance){     // send spi to display , ok

			 if (seq_pos==191) seq_pos=0;  else seq_pos++;
			 TIM10->ARR=timer_value;
		}
	}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi)   // when finished sending
	{

	   if (SPI1==hspi->Instance) {				// keep it short if possible

	       HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  //  end
	       flash_flag=2;


	       //    if (sample_dma_counter==0)		{  memcpy(flash_read_block3,flash_read_block2+4,1024);  flash_flag=2;  }
	    //   if (sample_dma_counter==1) sample_dma_counter=3;
	 			   //			      if (sample_dma_counter==2) sample_dma_counter=4;
	  //     if (sample_dma_counter==3)   {memcpy(flash_read_block,flash_read_block2+4,1024);sampler_1k_load(sample_flash_address[0]);sample_dma_counter=2;return;}
	  //     if (sample_dma_counter==4)			{  memcpy(flash_read_block3,flash_read_block2+4,1024); sampler_1k_load(sample_flash_address[1]); sample_dma_counter=1; }
	//	if (sample_dma_counter)   {memcpy(flash_read_block,flash_read_block2+4,1024); sampler_1k_load(sample_flash_address[1]); sample_dma_counter=0;flash_flag=2; }

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
