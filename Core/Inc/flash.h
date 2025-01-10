


void flash_write(void){					// too much crap needs to simplify , easy mistakes
	  if ((send) & (write_once==0)&&(!shift)          ){

		// flash_sector_erase(10 );
		//  uint8_t spi_test3[5]={0,10,0,0};
		 		  uint8_t test_data3[270]={0,10,0,0};
		 	uint8_t patch_mem=(patch_save&15)<<4;    // 16*16 (4kbyte)   start location


		 		  //  uint8_t temp_data;
/*
		 		  for(i=0;i<256;i++){
		 			  test_data3[i+4]=255;

		 		  }
*/


			/*for (i=0;i<256;i++){
				if (scene_velocity[i])  scene_memory[i]=	((scene_velocity [i]&112)<<1)+	(scene_pitch [i]&31) ;   //merge into scene memory


			}*/

		 		  test_data3[2]=patch_mem;

		 		  memcpy  (test_data3+4 ,scene_memory,  256);   // maybe drum data


		 		 test_data3[0]=0x06; //enable write each time
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
		  HAL_SPI_Transmit(&hspi1, test_data3, 1, 1000);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
		  HAL_Delay(5);

		  //----formAT SECTION


		  test_data3[0]=0x20; //sector erase 4k (block is 0x52) , this is need , setting FF doesn't seem to work for page erase


		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);         // enable for sector erase   , stays empty when enabled
		  HAL_SPI_Transmit(&hspi1,  test_data3, 4, 1000);   //erase sector ,works       4kbytes   (block erase=32kbytes)
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
		  HAL_Delay(150);  // S
		  // test write


		  test_data3[0]=0x04; //disable write

			  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // low
			  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
			  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // high end
			  HAL_Delay(20);


		  //sector erase works

		//  memcpy  (test_data3+4 ,scene_memory,  256);

		  test_data3[0]=0x06;
		  test_data3[2]=patch_mem;
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
		  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
		  HAL_Delay(20);


		  test_data3[0]=0x02; //write ,page program
		  test_data3[2]=patch_mem;
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);   // low
		  HAL_SPI_Transmit(&hspi1, test_data3 ,260, 1000);  //address,then data
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
		  HAL_Delay(200);

		  test_data3[0]=0x04; //disable write
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // low
		  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // high end
		  HAL_Delay(20);


		  	  all_settings[150]=tempo;
			memcpy(all_settings,scene_transpose,9); // copy settings
			memcpy(all_settings+9,pot_states,8);
			memcpy(all_settings+17,pot_tracking,32);
			memcpy(all_settings+49,mute_list,8);
			memcpy(all_settings+57,scene_volume,8);
			memcpy(all_settings+65,midi_channel_list,8);
			memcpy(all_settings+73,looper_list,32);
			memcpy(all_settings+105,loop_length_set,8); // first 256bytes


			memcpy  (test_data3+4 ,all_settings,  200); // copy


		  test_data3[0]=0x06;
		  test_data3[2]=patch_mem+1; //page 4
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);
		  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
		  HAL_Delay(20);


		  test_data3[0]=0x02; //write ,page program
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);   // low
		  HAL_SPI_Transmit(&hspi1, test_data3 ,204, 1000);  //address,then data
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
		  HAL_Delay(200);

		  test_data3[0]=0x04; //disable write
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // low
		  HAL_SPI_Transmit(&hspi1, test_data3, 1, 100);
		  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // high end
		  HAL_Delay(20);



		  memcpy  (test_data3+4 ,drum_store_one,  256);  // drums
		  test_data3[0]=0x06;
		  test_data3[2]=patch_mem+2; //page5
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

				  memcpy  (test_data3+4 ,button_states_loop,  256);    // key notes
						  test_data3[0]=0x06;
						  test_data3[2]=patch_mem+3; //page6
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
		  HAL_Delay(500);



		  //  write_once=1;
		  send=0;
		  button_states[70]=0;
		  button_states[69]=0;
		  pan=0;
		 scene_buttons[0]=0;
		 all_update=1;
		  other_buttons=1;
		  button_states[square_buttons_list[patch_save]]=0;
		 first_message=2;
	  }

}

void flash_read(void){     // 1kbyte for now
	HAL_Delay(100);

	uint8_t test_data2[1028]={0,10,0,0};
	uint8_t test_data3[1028]={0,10,0,0};
	uint8_t patch_mem=(patch_save&15)<<4;    // 16*16 (4kbyte)   start location

	test_data2[0]=0x03; //read ok , get notes
	test_data2[2]=patch_mem;
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);  // when readin low till the end
	HAL_SPI_TransmitReceive (&hspi1,test_data2, test_data3,  1024, 100); // request data , works
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
	HAL_Delay(100);

	memcpy(scene_memory,test_data3+4,256);    // buttons data

	memcpy(all_settings,test_data3+260,200); // copy back settings block
	memcpy(scene_transpose,all_settings,9);
	memcpy(pot_states,all_settings+9,8);
	memcpy(pot_tracking,all_settings+17,32);
	memcpy(mute_list,all_settings+49,8);
	memcpy(scene_volume,all_settings+57,8);
	//memcpy(midi_channel_list,all_settings+65,8);  // temp disable


	memcpy(looper_list,all_settings+73,32); // first 256bytes
	memcpy(loop_length_set,all_settings+105,8); // first 256bytes

	memcpy(drum_store_one,test_data3+516,256);   // next block (+4)
	memcpy(button_states_loop,test_data3+772,256); // need more space

	tempo=all_settings[150];
	if (tempo==255) tempo=120;
	if (!tempo) tempo=120;

	for (i=0;i<40;i++){
		//pot_tracking[i>>1] =scene_transpose[3+(i>>3)];
		button_states[i]=3;
		//if (scene_memory[i]) button_states[i+8]=5; // needs this to run first so first page loads

	//if ((i<8) && (mute_list[i])) button_states[i]=3;  // muting
		}
/*
	for (i=0;i<255;i++){
		scene_velocity [i]=(scene_memory[i]>>1)&112;    //needs to update velocities or lost , shifted
		scene_pitch [i]=(scene_memory[i])&31;   // pitch ?

	}*/

	uint8_t d;



	//uint8_t data_temp2=0;
	//uint16_t loop_pos=0;
	for (n=0;n<4;n++){			//initial loop fill
		//uint16_t  loop_speed=((looper_list[(n*4)+2])+1);
		//uint8_t note_length=looper_list[(n*4)+3];
		loop_screen_last_note[n]=16;
		scene_buttons[0]=n;

		for (d=0;d<16;d++) {// loop notes fill

				//	data_temp2=i+(n*32);    // button + scene_select writes all on selected
					//loop_pos=(i*8);
					pattern_select=d;
					//loop_selector=1;
					loop_screen();
					//if (!button_states_loop[data_temp2]) button_states_loop[data_temp2]=1;
					//if (button_states_loop[data_temp2]) {

						// loop_screen_note_on[data_temp2]= ((i*loop_speed)+1)&255;    //  creates a short playlist , multiplied by speed 1-8
						// loop_screen_note_on[loop_pos]=loop_screen_note_on[loop_pos] | (1<<n);  // turn on bit, basic for now
						// loop_screen_note_on[loop_pos]=loop_screen_note_on[loop_pos] | (1<<n);  // turn on bit, basic for now

						 // loop_screen_note_on[data_temp2]=loop_screen_note_on[data_temp2] & ~ (1<<selected_scene);} //write from buttons ,test only


					//	 loop_screen_note_off[(loop_pos+3)&255]= loop_screen_note_on[loop_pos];  // note off send, works

						 loop_screen_last_note[n]=d;

					}


					}



	float tempo_hold=1;  // calculate tempo look up

	 float period= cpu_clock/prescaler; //  24584 hz 1/120   /128



	 for (d=1;d<250;d++) {     // calculate ARR vales for TIM10

		  		if (d<40) tempo_hold=23437;  // 40bpm

		  		else tempo_hold=(period/(0.0166667*d*ppq_set));   // 500khz or 191 prescaler   , 32 ppq maybe do 96ppq  count



		  		bpm_table[d]=tempo_hold;
		  	}
		  	timer_value=bpm_table[tempo]-1;  // starting bpm -1
		  //	timer_value=511;

		  	TIM2->PSC=prescaler-1;
		  	TIM2->ARR=timer_value;

		  //	TIM2->CCR1=(timer_value/2) ;

		  	send=0;
				  button_states[70]=0;
				  button_states[69]=0;
				  pan=0;
				 scene_buttons[0]=0;
				 all_update=1;

				 button_states[patch_save+8]=0;
			//	 memcpy(button_states_main,button_states,40);
			//	 memcpy(button_states_save,button_states+8,32);
			//	 memcpy(button_states_save+32,button_states,8);
				 first_message=2;

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

