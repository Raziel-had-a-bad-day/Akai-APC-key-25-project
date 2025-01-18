
void settings_storage(void){   // runs to store setting and backh

			uint8_t *settings[9]={ 	scene_transpose,pot_states,pot_tracking,mute_list,pattern_offset_list,midi_channel_list,looper_list,loop_length_set,alt_pots};
			uint8_t settings_multi[9]={1,1,4,1,1,1,4,1,1};   // sets length,  sound_set*x
			uint8_t settings_temp[64];
			uint8_t settings_total=0;  //adds up position
			uint8_t length=0;

			for (i=0;i<9;i++){

				length=(settings_multi[i]*sound_set);
				if(settings_write_flag) {		memcpy(settings_temp,settings[i],length);	// copy to temp
				memcpy(all_settings+settings_total,settings_temp,length);}
				else {

					memcpy(settings_temp,all_settings+settings_total,length);  // copy value
					memcpy(settings[i],settings_temp,length);                }  //

		settings_total=settings_total+length;

			}

			for (i=0;i<250;i++){if (all_settings[i]>127) all_settings[i]=0;}  // just in case
			settings_write_flag=0;

}

void flash_write(void){					// too much crap needs to simplify , easy mistakes
	  if ((send) & (write_once==0)&&(!shift)          ){

		// flash_sector_erase(10 );
		//  uint8_t spi_test3[5]={0,10,0,0};
		 		  uint8_t test_data3[270]={0,10,0,0};
		 	uint8_t patch_mem=(patch_save&15)<<4;    // 16*16 (4kbyte)   start location



		 		  test_data3[2]=patch_mem;

		 		  memcpy  (test_data3+4 ,drum_store_one+256,  256);   // maybe drum data


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


		  	  all_settings[250]=tempo;

		  	  settings_write_flag=1;
			settings_storage();

			memcpy  (test_data3+4 ,all_settings,  256); // copy


		  test_data3[0]=0x06;
		  test_data3[2]=patch_mem+1; //page 4
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

				  memcpy  (test_data3+4 ,drum_store_one+512,  256);    // key notes
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

								  memcpy  (test_data3+4 ,drum_store_one+768,  256);    // key notes
						  test_data3[0]=0x06;
						  test_data3[2]=patch_mem+4; //page6
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
	  	  loop_screen();
}

void flash_read(void){     // 1kbyte for now
	HAL_Delay(100);

	uint8_t test_data2[1028]={0,10,0,0};
	uint8_t test_data3[2048]={0,10,0,0};
	uint8_t patch_mem=(patch_save&15)<<4;    // 16*16 (4kbyte)   start location

	test_data2[0]=0x03; //read ok , get notes
	test_data2[2]=patch_mem;
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);  // when readin low till the end
	HAL_SPI_TransmitReceive (&hspi1,test_data2, test_data3,  1024, 100); // request data , works
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
	HAL_Delay(100);

	memcpy(drum_store_one+256,test_data3+4,256);    // buttons data

	memcpy(all_settings,test_data3+260,256); // copy back settings block

	settings_storage();

	memcpy(drum_store_one,test_data3+516,256);   // next block (+4)
	memcpy(drum_store_one+512,test_data3+772,512); // need more space

	tempo=all_settings[250];
	if (tempo==255) tempo=120;
	if (!tempo) tempo=120;

	for (i=0;i<40;i++){
		//pot_tracking[i>>1] =scene_transpose[3+(i>>3)];
		button_states[i]=3;
		//if (scene_memory[i]) button_states[i+8]=5; // needs this to run first so first page loads

	//if ((i<8) && (mute_list[i])) button_states[i]=3;  // muting
		}


	uint8_t d;



	//uint8_t data_temp2=0;
	//uint16_t loop_pos=0;
	for (n=0;n<8;n++){			//initial loop fill
		//uint16_t  loop_speed=((looper_list[(n*4)+2])+1);
		//uint8_t note_length=looper_list[(n*4)+3];
		loop_screen_last_note[n]=16;
		scene_buttons[0]=n;

		for (d=0;d<16;d++) {// loop notes fill

				//	data_temp2=i+(n*32);    // button + scene_select writes all on selected
					//loop_pos=(i*8);
					pattern_select=d;
					//loop_selector=1;
					//loop_screen();
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

	pattern_select=0;

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

