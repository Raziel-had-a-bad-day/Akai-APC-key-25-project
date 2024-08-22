void flash_write(void){					// too much crap needs to simplify , easy mistakes
	  if ((button_states[70]) & (write_once==0)){




		// flash_sector_erase(10 );
		  uint8_t spi_test3[5]={0,10,0,0};
		 		  uint8_t test_data3[270]={0,10,0,0};
		 		//  uint8_t temp_data;




		 		  /*
			for (i=0;i<256;i++){
				temp_data=	(scene_velocity [i]<<1)&224;    //merge into scene memory
				test_data3[i+4]=	(scene_pitch [i]&31)+temp_data;   // pitch

			}
*/
			memcpy  (test_data3+4 ,scene_memory,  256);


		  spi_test3[0]=0x06; //enable write each time
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
		  HAL_SPI_Transmit(&hspi1, spi_test3, 1, 1000);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
		  HAL_Delay(5);

		  //----formAT SECTION

		  spi_test3[0]=0x20; //sector erase 4k (block is 0x52)

		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);         // enable for sector erase   , stays empty when enabled
		  HAL_SPI_Transmit(&hspi1, spi_test3, 4, 1000);   //erase sector ,works       4kbytes   (block erase=32kbytes)
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
		  HAL_Delay(150);  // S
		  // test write

		  spi_test3[0]=0x04; //disable write

		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // low
		  HAL_SPI_Transmit(&hspi1, spi_test3, 1, 100);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // high end
		  HAL_Delay(20);


		  //sector erase works

		//  memcpy  (test_data3+4 ,scene_memory,  256);

		  test_data3[0]=0x06;
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
		  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
		  HAL_Delay(20);


		  test_data3[0]=0x02; //write ,page program
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);   // low
		  HAL_SPI_Transmit(&hspi1, test_data3 ,260, 1000);  //address,then data
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
		  HAL_Delay(200);

		  test_data3[0]=0x04; //disable write
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // low
		  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // high end
		  HAL_Delay(20);


		  	  all_settings[90]=tempo;
			memcpy(all_settings,scene_transpose,9); // copy settings
			memcpy(all_settings+9,pot_states,8);
			memcpy(all_settings+17,pot_tracking,32);
			memcpy(all_settings+49,mute_list,8);
			memcpy(all_settings+57,scene_volume,8);
			memcpy(all_settings+65,midi_channel_list,8);

			memcpy  (test_data3+4 ,all_settings,  100); // copy


		  test_data3[0]=0x06;
		  test_data3[2]=0x01; //page 4
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
		  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
		  HAL_Delay(20);


		  test_data3[0]=0x02; //write ,page program
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);   // low
		  HAL_SPI_Transmit(&hspi1, test_data3 ,104, 1000);  //address,then data
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
		  HAL_Delay(200);

		  test_data3[0]=0x04; //disable write
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // low
		  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // high end
		  HAL_Delay(20);



		  memcpy  (test_data3+4 ,play_list,  256);
		  test_data3[0]=0x06;
		  test_data3[2]=0x02; //page5
				  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
				  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
				  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
				  HAL_Delay(20);


				  test_data3[0]=0x02; //write ,page program
				  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);   // low
				  HAL_SPI_Transmit(&hspi1, test_data3 ,260, 1000);  //address,then data
				  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
				  HAL_Delay(200);

				  test_data3[0]=0x04; //disable write
				  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // low
				  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
				  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // high end
				  HAL_Delay(20);


		  HAL_Delay(200);
		//  write_once=1;

		  button_states[70]=0;

		  other_buttons=1;
	  }





}

void flash_read(void){
	HAL_Delay(100);

	uint8_t test_data2[260]={0,10,0,0};
	uint8_t test_data3[260]={0,10,0,0};


	test_data2[0]=0x03; //read ok , get notes

	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);  // when readin low till the end
	HAL_SPI_TransmitReceive (&hspi1,test_data2, test_data3,  256, 100); // request data , works
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
	HAL_Delay(100);

	memcpy(scene_memory,test_data3+4,256);

	test_data2[2]=1;  // get settings
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);  // when readin low till the end
	HAL_SPI_TransmitReceive (&hspi1,test_data2, test_data3,  104, 100); // request data , works
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
	HAL_Delay(100);



	memcpy(all_settings,test_data3+4,100); // copy back
	memcpy(scene_transpose,all_settings,9);
	memcpy(pot_states,all_settings+9,8);
	memcpy(pot_tracking,all_settings+17,32);
	memcpy(mute_list,all_settings+49,8);
	memcpy(scene_volume,all_settings+57,8);
	memcpy(midi_channel_list,all_settings+65,8);

	test_data2[2]=2;   // get play list
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);  // when readin low till the end
	HAL_SPI_TransmitReceive (&hspi1,test_data2, test_data3,  256, 100); // request data , works
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
	HAL_Delay(100);

	memcpy(play_list,test_data3+4,256);


	tempo=all_settings[90];

	for (i=0;i<32;i++)
		//pot_tracking[i>>1] =scene_transpose[3+(i>>3)];

	{if (scene_memory[i]) button_states[square_buttons_list[i]]=5; // needs this to run first so first page loads

	}
	if ((i<8) && (mute_list[i])) button_states[i]=3;  // muting


	for (i=0;i<255;i++){
		scene_velocity [i]=(scene_memory[i]>>1)&112;    //needs to update velocities or lost , shifted
		scene_pitch [i]=(scene_memory[i])&31;   // pitch

	}
	if (tempo==255)  tempo= 120;  //sets it for tempo 120 in case missing

	float tempo_hold=1;  // calculate tempo look up
	uint16_t counter;
	 float period= cpu_clock/prescaler; //  24584 hz 1/120   /128
	 float d;

		  	for (d=1;d<250;d++) {     // calculate ARR vales for TIM10
		  		tempo_hold=0.533333*d; // hz
		  		tempo_hold=period/tempo_hold; //
		  		counter=d;
		  	bpm_table[counter]=tempo_hold;
		  	}
		  	timer_value=bpm_table[tempo];  // starting bpm
		  //	timer_value=511;



}
void panic_delete(void){

	HAL_Delay(100);


	uint8_t temp_hold=0;

	for (n=0;n<32;n++){ // delete all if most notes are on
	{if (scene_memory[n+32]) temp_hold++;}

	}

	if (temp_hold > 29) {for (n=0;n<255;n++){scene_memory[n]=0;}}

}



// note bank search and replace

