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
#include "midi.h"
#include "notes.h"
#include "lcd.h"
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
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */





#include "flash.h"




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
static void MX_TIM10_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void note_handling(uint8_t incoming_data);
void note_replace(uint8_t note_replace);
//void USB_CDC_RxHandler(uint8_t*, uint32_t);
void patch_screen(void);
void buttons_store(void);
void midi_send(void);  // creates midi_cue data
void flash_write(void);
void flash_read(void);
void panic_delete(void);
//void stop_start(void);
void note_off(void);
void arrows(void);
void cdc_send(void);
void all_notes_off(void);
void play_muting(void);
void main_screen(void);
void lcd_start(void);
void lcd_print(uint8_t  pos , char print);  // position 0-39 , character
void lcd_menu_vars(void);
void nrpn_send(void);
void loop_screen(void);// loop screen change
void note_buttons(void); // all note functions from buttons

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
  MX_TIM10_Init();
  MX_USB_DEVICE_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  // USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

  HAL_SPI_Init(&hspi1); // write to register hspi2
  HAL_DMA_Init(&hdma_spi1_rx);
  	HAL_DMA_Init(&hdma_spi1_tx);
  //	HAL_I2C_Init(&hi2c1);


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
	other_buttons=1; // nothing

	lcd_start();
	// panic_delete();                 WATCH FOR WEIRD APC SENDS , IE UP ARROW PLUS BOTTOM ROW 3 SENDS CONSTANT CONTROLLER 50 INFO  ?
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



	  if (seq_enable) {
		  uint16_t step_temp=0;
		  uint8_t seq_step_mod=seq_step_list[scene_buttons[0]]&31;
		  uint8_t step_end;
		//  uint8_t step_length;
		  uint8_t selected_scene=scene_buttons[0];
		//  uint8_t step_start;
		//  uint8_t play_total; // different for each note
		  uint8_t buttons_list[20]={0,64,65,66,67,68,69,70,71,81,82,83,84,85,86,91,93,98};  //extra buttons  slow

		  if (seq_pos_mem!=seq_pos){     // runs  8 times /step  , control sequencer counting

			  if(!pause) {

				  for (i=0;i<8;i++){				// controls steps needs better res

					  seq_step_fine[i] = (seq_step_fine[i]+1)&2047;   // long and slow , only 1/8 a note so 256 notes in total normal speed

					  step_temp=(( seq_step_fine[i]>>4)&31);  // reads fine position , one note length is 64 count
					//  if(seq_step_list[i]!=step_temp) seq_step_enable[i]=1; // flips on change of step

					//  if ((step_temp==step_end) && (step_temp))  { seq_step_fine[i] =looper_list[(i*4)]<<6; seq_step_reset[i]=(seq_step_reset[i]+1)&31; } // jump to start of loop, too early


					//  if ( (seq_step_list[i]&31) ==(step_temp))   seq_step_list[i]=step_temp+32;  else     seq_step_list[i] =step_temp;   //  copy back unless repeating then enable bit 6 , to stop retriggering notes
					//  if ( (seq_step_list[i]&31) ==(step_temp))   seq_step_list[i]=step_temp+32;  else     seq_step_list[i] =step_temp;
					  seq_step_list[i] =step_temp;
				  }}

			//  if (!pause)		 {midi_send();note_off();}   // midi data calculate
			  if (!pause)		 {midi_send();}   // midi data calculate

			  cdc_send(); // all midi compiled for send

			 // send_buffer_sent=1;
			  if (send_buffer_sent==1)                  {    // moving light send and button change , quick

				    // track only this
//			  				  send_buffer[0] =144;
//			  			  send_buffer[1] = square_buttons_list[seq_step_mem];   // for displaying 0-32 , reset previous light then new on , should be based on memory
//			  			  send_buffer[2]=button_states[send_buffer[1]]; // last state
//			  			  send_buffer[3] =144;
//			  			  send_buffer[4] = square_buttons_list[((seq_step_mod ) & 31)] ;  // set new light on , mostly for green moving button
//
//			  			  if (record) send_buffer[5] = 4; else send_buffer[5] = 1; //colour
			  			seq_step_mem=seq_step_mod;
			  			  send_buffer_sent=2;
			  					}

			  			 //// extra buttons

			  				counter_a=0;
			  							  for (i=1;i<19;i++) {   // test for extra button changes and set counter_a, quick
			  								  if (other_buttons_hold[i]!= button_states[buttons_list[i]]) {counter_a=i;
			  								  other_buttons_hold[i]= button_states[buttons_list[i]];}
			  							  	  	  }



			  			  if (counter_a) {


			  			  send_buffer[6] = 144;
			  			  send_buffer[7] = buttons_list[counter_a]&127;
			  			  send_buffer[8] = button_states[buttons_list[counter_a]]&127;  // not all will light up :/

			  			  } else
			  				  send_buffer[6] = 0;




			  			 if (serial_len)   HAL_UART_Transmit(&huart1,serial_out,serial_len,100); // uart send disable if no info
			  if (cdc_len) {CDC_Transmit_FS(cdc_send_cue, cdc_len);cdc_len=0;  } // USB send





			  if(pause  || down_arrow) {  //play notes  during pause or down arrow from keyboard , need to be elsewhere

						  if (keyboard[0])  {
							  last_key=keyboard[0];   //store key

							  if (midi_channel_list[selected_scene]==9)
							  {  midi_extra_cue[0]=153;         // use drumlist for now

							  midi_extra_cue[1]=(drum_list[selected_scene]);midi_extra_cue[2]=127; midi_extra_cue[28]=3;

							  {	nrpn_cue[selected_scene*3]=selected_scene+1; nrpn_cue[(selected_scene*3)+1]=((keyboard[0])+scene_transpose[selected_scene])&127 ;  } // pitch data
							  keyboard[0]=0;}
							  else  {midi_extra_cue[0]=144+midi_channel_list[selected_scene];  midi_extra_cue[1]=((keyboard[0])+scene_transpose[selected_scene])&127 ;
							  midi_extra_cue[2]=127; midi_extra_cue[28]=3;keyboard[0]=0;}

						  }

						  else   { if     (   midi_extra_cue[2])       {midi_extra_cue[2]=0;midi_extra_cue[28]=3; midi_cue_noteoff[50]=0;}   else midi_extra_cue[28]=0;}
						  	  keyboard[0]=0;
					  }


			 seq_pos_mem=seq_pos;
		 } // end of fast step 8/step



		  if ((s_temp) != (seq_pos>>3)) {			// normal sending , very slow atm



		//  if((button_send_trigger>>3)!=trigger_mem){      //
			  if ((!seq_pos)&& (!pause)) seq_step_long=(seq_step_long+1)&31;    // this needs to be main clock
			 if (play_list_write) play_list[(scene_buttons[0]*32)+seq_step_long]=pot_states[2]>>1;    // keep writing if enabled

			  //  if((bar_looping) && (scene_buttons[0]==(bar_looping-1)) )
			  //	  {

			  //  if (loop_selector<2)
			  //  seq_step_mod=((seq_step&7)+(looper_list[scene_buttons[0]<<2]))&31;

			  uint8_t selected_scene=scene_buttons[0];
			  seq_step_mod=seq_step_list[selected_scene]&31;
			  seq_current_step=seq_step_mod;
			  loop_current_length=looper_list[(selected_scene*4)+1];
			  loop_current_offset=looper_list[(selected_scene*4)];
			  loop_current_speed=looper_list[(selected_scene*4)+2];

			  //else seq_step_mod=((seq_step&7)+((loop_selector-2)*8))&31;
			  //	  }


			  // looper visual only when in scene though

			  //step_start=looper_list[(selected_scene*4)+1];
			  step_end=((looper_list[(selected_scene*4)+1]+looper_list[(selected_scene*4)])&31);

				play_position=seq_step_long;
			  if ((seq_step_long&31)==0)   play_position=(play_position+1)&31;  // normal screen leave alone ,too fast
				if ((seq_step_long&3)==0) {play_position=(play_position>>2)<<2;} // reset

			  if (seq_step_mem!=seq_step_mod)		{send_buffer_sent=1;}


			  if(write_velocity && button_states[seq_step_mod+8] )  scene_velocity[seq_step_mod+(scene_buttons[0]*32)]= write_velocity;    // Writes velocity while enabled


////


			  if ((send) && shift) {  all_notes_off(); flash_read();  button_states[70]=0; send=0; }   // reload

 			//	 if (send_buffer[9]) printf(" send_buffer[9]=%d\n ",send_buffer[9]);
 /*
 				printf("step_long= %d ", (seq_step_long+(selected_scene*32)));printf("button=%d ", button_states[square_buttons_list[seq_step_long]]);printf(" %d ", serial_out[3]);
 				printf(" %d ", serial_out[4]);printf("pitch=%d ", scene_pitch[(selected_scene*32)+(seq_step_list[selected_scene]&31)]);
 				printf(" transps=%d ",scene_transpose[selected_scene]);
 				printf("    vel=%d ", scene_velocity[(selected_scene*32)+(seq_step_list[selected_scene]&31)]);printf("  txt=%*s",3,lcd_char );printf("   %d\n ",lcd_pos);
 				printf("   %d\n ",lcd_pos);
 */

			  for (i=0;i<16;i++){	printf(" %d",loop_screen_note_on[(selected_scene*32)+i] );

			  }

			  printf(" loop=%d ",loop_selector);  printf(" step=%d ",seq_step_list[0]);printf("   %d\n ",seq_step_fine[0]&255);


 				// print section

 				lcd_menu_vars();

 				//		printf(" %d ", midi_cue[3]);
 				//		printf(" %d\n ", midi_cue[6]);

 				  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,led_blink);
 				  led_blink=!led_blink;

 				  flash_write(); // works only if button pressed
 				  note_off_enable=1;
 				//  arrows();
 				  if (!pause) seq_step = seq_pos>> 3; else seq_step=seq_step;

// 				  if(pause) {// keyboard midi send during pause
// 					  { if (keyboard[0]&&(scene_buttons[0]>3))  {;midi_cue[0]=143+scene_buttons[0];
// 					  midi_cue[1]=((keyboard[0])+scene_transpose[scene_buttons[0]])&127 ;midi_cue[2]=127;}
// 					  midi_cue[50]=3;}
// 					// midi_cue[50]=0;
//
//
// 				  }


 				  s_temp = seq_pos>>3;
 				//  trigger_mem=button_send_trigger>>3;
 			}   // blink steps , end of send trigger

 		}  // end of seq_enable



//	  if((all_update>2) & (all_update<8)){   // update selected line  square scene lights 3-7
//
//
//
//		  uint8_t select_line=(((all_update-3))*8)&31;
//
//		  select_line= square_buttons_list [select_line];
//
//		  temp=select_line;
//
//
//		  for (i=0;i<8;i++)  {
//			  send_all[i*3]=144;
//			  send_all[(i*3)+1]=i+select_line;    // button
//			  send_all[(i*3)+2]=button_states[i+select_line];   // button value
//		  }
//
//		  all_update=10;}
//
//


/*


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
*/
		if((device) && (select) )   {														///   Midi channel  , maybe elsewhere

				//	button_pressed=square_buttons_list[pot_states[7]>>3]; // last data
			//		button_states[square_buttons_list[midi_channel_list[20]]]=0;
			//		button_states[square_buttons_list[midi_channel_list[scene_buttons[0]]]]=3;  //
				//	midi_channel_list[scene_buttons[0]]=pot_states[7]>>3;


			//		button_states[square_buttons_list[pot_states[7]>>3]]=3;
				//all_update=3+(pot_states[7]>>6);  // 3+0-4
				}



 	  if (cdc_buffer[0]) {      //  when cdc buffer incoming
 		  seq_enable=1; // wont start till midi in


 		  for (i=0;i<32;i++) {// scene memory fill from buttons each time a button is pressed
 				uint8_t data_temp=i+(scene_buttons[0]*32);


 				if ((!play_screen) &&(!loop_selector)) {main_screen();

 			if (button_states [i+8] )  scene_memory[data_temp]	= ((scene_velocity [data_temp])&112)+scene_pitch[data_temp]; } // only for current

 			//	if (!play_screen &&(loop_selector==2)){
 				 		//	if (button_states [i+8] )  button_states[i+8]	= button_states_loop [data_temp]; }



 		  }
 			//	}  // play list  muting


 	  if(play_screen) {play_screen=3;   play_muting();} // only reads, on receive
 		  buttons_store();   // only runs after receive


 		  if (button_states[91] ) {pause=1;all_notes_off(); } else pause=0;


 	  }  // end of cdc message


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  } // while loop
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  huart1.Init.BaudRate = 31250;
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
  GPIO_InitStruct.Pull = GPIO_NOPULL;
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

			//button_send_trigger=(button_send_trigger+1)&255; // sets interval for buttons
			 seq_pos++;
			if (seq_pos>=256) seq_pos=0;
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

void stop_start(void)             {
  //  if (TIM10==htim ->Instance)
    if  (stop_toggle ==1) {HAL_TIM_Base_Stop_IT(&htim10);stop_toggle =2;}
    if  (stop_toggle ==4) {HAL_TIM_Base_Start_IT(&htim10);stop_toggle=0;}

}
void all_notes_off(void){
	uint8_t data[48];
	for (i=0;i<16;i++){
		data[i*3]=176+i;
		data[(i*3)+1]=123;
		data[(i*3)+2]=0;
	}
	 HAL_UART_Transmit(&huart1,data,48,100); //   send all notes off on serial
	 HAL_Delay(100);
	 //CDC_Transmit_FS(data+3, 45);

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
