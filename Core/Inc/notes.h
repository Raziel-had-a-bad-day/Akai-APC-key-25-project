

void buttons_store(void){

	uint8_t vel=pot_states[0]>>3;  // testing
	uint8_t scene_select=0;
	uint8_t button_pressed=255; // sends out changed button ,255 is none
	uint8_t incoming_message[3];
	memcpy(incoming_message,cdc_buffer, 3); // works off only receiving buffer
	uint8_t button_selection=button_states[incoming_message[1]];
	uint8_t buffer_clear = 0;
	uint8_t incoming_data1 = incoming_message[1]&127;
	uint8_t status=incoming_message[0];

	if (status == 128) // note on
		{note_off_flag[0]=0;




		if ((incoming_data1 >7)&(incoming_data1 <40)){

				last_button= square_buttons_list[incoming_data1-8]+(scene_buttons[0]*32)-8;  // stare last note pressed

					scene_velocity[last_button]=  (((pot_states[1]>>5)<<5)+31)&112;  // writes on note off ,needs -8 after square buttons 16-112 values, 3bit
					scene_pitch[last_button]=  pot_states[0]>>2; // 0-32 pitch
					if (keyboard[0]) scene_pitch[last_button]= keyboard[0];  // overwrite if keyboard held


					{if ((button_states[last_incoming ]==3)& (last_incoming!=incoming_data1)) button_states[last_incoming ] = 5;	button_pressed=last_incoming;}
							 last_incoming=incoming_data1; // switch previous key from red to yellow

		}  // pitch for 4-8 scenes

		}  // skip for now

	if ((incoming_data1 <8)&& (status==144)) {   // scene buttons
									scene_select=incoming_data1 +1;}  //enable scene_select section

	if (status == 145)  { keyboard[0]=(incoming_data1 -48);send_buffer[6]=147;send_buffer[7]=incoming_data1;send_buffer[8]=127;}  // store last key pressed mainly , 48-72 default setting(24)
	if ((status==129))  { keyboard[0]=0;send_buffer[6]=147;send_buffer[7]=incoming_data1;send_buffer[8]=0;}  // store last key pressed mainly

	if (status == 144){

		{note_off_flag[0]=1;note_off_flag[1]=incoming_data1 ;

		 if (incoming_data1>7){
		//{if (button_selection)button_states[incoming_data1 ] = 0; else button_states[incoming_data1 ] = 5;}
		 switch(button_selection){    // blank , r
		 case 0 :button_states[incoming_data1 ] = 5;break;
		 case 5 :button_states[incoming_data1 ] = 0;  ;break;
		 //case 5 :button_states[incoming_data1 ] = 3;break;


		 }

		 }

		}

		if (button_states[65]) scene_mute=1; else scene_mute=0;  //enable muting on scene select
		if (button_states[66]) {scene_solo=1;button_states[65]=0;} else scene_solo=0;  //enable muting on scene select
		button_pressed=incoming_data1; // important  , only after note on


	} // change button state

	if (status == 176) {
		pot_states[incoming_data1  - 48] = incoming_message[2]; // store pot
		//	if ((note_off_flag[0])&& (note_off_flag[1]<32))  scene_velocity[square_buttons_list[note_off_flag[1]]+(scene_buttons[0]*32)]=  pot_states[1];    // set velocity for now for held button , only for notes
		if (incoming_data1==50)  scene_transpose[scene_buttons[0]]=pot_states[2]>>1; // 0-64 transpose from base
		if (incoming_data1==48)	scene_pitch[last_button]=  pot_states[0]>>2; // 0-32 pitch   update last pressed button
		if (incoming_data1==51) pot_states[3]=pot_states[3]/3;
		if ((incoming_data1==49) && (button_states[68]))   scene_velocity[seq_pos+(scene_buttons[0]*32)]=  (((pot_states[1]>>5)<<5)+31)&112;  // update velocity in realtime if volume button pressed

	}

	if (scene_select)  { // change scene select lite , one at a time though , fully update so need for extra sends



		if(scene_mute) { // muted

		 if ((button_states[scene_select-1])!=3)  {button_states[scene_select-1]=3;}  else {button_states[scene_select-1]=1;}  // simple on off red buttons , no yellow

		}

		else {    // mormal
			if ((scene_select-1)==scene_buttons[0])  {  if ((button_states[scene_buttons[0]])==5)  {button_states[scene_select-1]=4;}  else {button_states[scene_select-1]=5;} }
			if ((scene_select-1)!=scene_buttons[0]){ button_states[scene_buttons[0]]=1; // leave green unless muting
					{if (((button_states[scene_select-1])==0) |((button_states[scene_select-1])==1))   button_states[scene_select-1]=5;  else button_states[scene_select-1]=4; }

					}

			scene_buttons[0]=scene_select-1;
			all_update=1;
			for (i=0;i<32;i++){

					{if (scene_memory[i+((scene_select-1)*32)]>>5) button_states[square_buttons_list[i]]=5; else button_states[square_buttons_list[i]]=0;}


		}

		  // ignore is muting

		  // disable switch scene if muting enabled



			 //  buttons from scene memory , ok , data seems to get lost here

		}

	
		scene_select=0;

	}

	buffer_clear = 1;

	if (buffer_clear)
		memcpy(cdc_buffer, clear, 3);

	if (button_pressed!=255)  {send_buffer[6]=144; send_buffer[7]=button_pressed;send_buffer[8]=button_states[button_pressed];button_pressed=255;}   // send after one press , maybe retriger for more



}

	void midi_send(void){  // only for midi music no info return

		uint8_t cue_counter=0;
		uint8_t velocity=0;
		uint8_t seq_step_mod=seq_step;

		for (i=0;i<4;i++){    // drums

			if (scene_memory[seq_step+(i*32)]>>5){
				midi_cue[cue_counter*3]=146;  // channel 3
				midi_cue[(cue_counter*3)+1]=drum_list[i];  // or pitch info
				velocity=(scene_velocity[seq_step+(i*32)])&127;   // use only 3 bit msb
				if (!velocity) velocity=64; //missing velocity info still
				if (scene_mute & ((button_states[i])==3)) velocity=0;
				if ((scene_solo) & (scene_buttons[0]!=i)) velocity=0;   // mute everything but solo
				midi_cue[(cue_counter*3)+2]=velocity;
				cue_counter++;
			}
		}
		for (i=4;i<8;i++){   // notes

		//	if((bar_looping) && (i==(bar_looping-1)))  seq_step_mod=(seq_step&7)+(bar_count<<3); else seq_step_mod=seq_step;
			if((bar_looping) && (i==(bar_looping-1)))  seq_step_mod=((seq_step&7)+(pot_states[3]>>2))&31; else seq_step_mod=seq_step;
					if (scene_memory[seq_step_mod+(i*32)]>>5){


						midi_cue[cue_counter*3]=147+(i-4);  // channel 4
				midi_cue[(cue_counter*3)+1]=((scene_pitch[seq_step_mod+(i*32)])+scene_transpose[i])& 127;;  //  pitch info
				velocity=(scene_velocity[seq_step_mod+(i*32)])&127;   // use only 3 bit msb
				if (!velocity) velocity=127; //missing velocity info still
				if ((scene_mute) & ((button_states[i])==3)) velocity=0;
				if ((scene_solo) & (scene_buttons[0]!=i)) velocity=0;   // mute everything but solo

				midi_cue[(cue_counter*3)+2]=velocity;
				cue_counter++;
			}




			midi_cue[25]=cue_counter*3;


		}


	}


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



			memcpy(all_settings,scene_transpose,9); // copy settings
			memcpy(all_settings+9,pot_states,8);
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


	test_data2[0]=0x03; //read ok

	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);  // when readin low till the end
	HAL_SPI_TransmitReceive (&hspi1,test_data2, test_data3,  256, 100); // request data , works
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
	HAL_Delay(100);

	memcpy(scene_memory,test_data3+4,256);

	test_data2[2]=1;
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);  // when readin low till the end
	HAL_SPI_TransmitReceive (&hspi1,test_data2, test_data3,  104, 100); // request data , works
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end
	HAL_Delay(100);

	memcpy(all_settings,test_data3+4,100); // copy back
	memcpy(scene_transpose,all_settings,9);
	memcpy(pot_states,all_settings+9,8);


	for (i=0;i<32;i++)


	{if (scene_memory[i]>>5) button_states[square_buttons_list[i]]=5; // needs this to run first so first page loads

	}
	for (i=0;i<255;i++){
		scene_velocity [i]=(scene_memory[i]>>1)&112;    //needs to update velocities or lost , shifted
		scene_pitch [i]=(scene_memory[i])&31;   // pitch

	}

	//memcpy(scene_velocity,scene_memory,256);


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



void note_handling   (uint8_t incoming_data) {     // select last data byte to enter maybe store it elsewhere  serial1_hold data  , works ok


// this for note generation , not so important for sequencer however
uint8_t data_byte=0;


uint8_t select_note=0;


//uint8_t incoming_data=0;



		// select which statement status
		if (incoming_data==248) return;



		if (!(incoming_data>>7))  data_byte=1;
		else {

		//cleared after select note

		switch (incoming_data>>4){

		case 9: note_temp[0]=1;data_byte=0;velocity=0; break;   // selects note on or off for now
		case 8: note_temp[0]=2;data_byte=0;velocity=0;break;	//others maybe later


		default: return; break;
		}}




		if (data_byte && (velocity))  {note_temp[2]=incoming_data;velocity=0; data_byte=0;select_note=1;} //write velocity ,ready to write  to final note
		if (data_byte && (!velocity))  {note_temp[1]=incoming_data; velocity=1;data_byte=0;}  //write key hit

		//Mote ON  , select any available or force the first one
	//	printf( " -%d ", note_temp[0] );printf( " %d ", note_temp[1] );printf( " %d- ", note_temp[2] );

		if (select_note && (note_temp[0]==1) ){   //1
			if (!note_bank[0])  {note_bank[0]=1;note_bank[1]=note_temp[1];note_bank[2]=note_temp[2];note_temp[0]=0;note_temp[1]=0;note_temp[2]=0;select_note=0; } }

		if (select_note && (note_temp[0]==1) ){   //2
					if (!note_bank[3])  {note_bank[3]=1;note_bank[4]=note_temp[1];note_bank[5]=note_temp[2];note_temp[0]=0;note_temp[1]=0;note_temp[2]=0;select_note=0;  } }


		if (select_note && (note_temp[0]==1)){    //3
					if (!note_bank[6])  {note_bank[6]=1;note_bank[7]=note_temp[1];note_bank[8]=note_temp[2];note_temp[0]=0;note_temp[1]=0;note_temp[2]=0; select_note=0; } }

		if (select_note && (note_temp[0]==1)){   // start note replace on 1
										note_bank[0]=0;note_bank[2]=64;note_replace_enable=1;select_note=0; }  // note replaces sends a stop note then restarts a new note

		//Note Off

		if (select_note && (note_temp[0]==2) ){
					if (note_temp[1]==note_bank[1])  {note_bank[0]=0;note_bank[1]=note_temp[1];note_bank[2]=note_temp[2];note_temp[0]=0;note_temp[1]=0;note_temp[2]=0;select_note=0; } }



		if (select_note && (note_temp[0]==2)){
							if ((note_temp[1]==note_bank[4]))  {note_bank[3]=0;note_bank[4]=note_temp[1];note_bank[5]=note_temp[2];note_temp[0]=0;note_temp[1]=0;note_temp[2]=0;select_note=0;  } }


				if (select_note && (note_temp[0]==2)){
							if ((note_temp[1]==note_bank[7]))  {note_bank[6]=0;note_bank[7]=note_temp[1];note_bank[8]=note_temp[2];note_temp[0]=0;note_temp[1]=0;note_temp[2]=0; select_note=0; } }

				if (select_note && (note_temp[0]==2)){
																{note_bank[0]=0;note_bank[3]=0;note_bank[6]=0;note_bank[1]=note_temp[1];note_bank[2]=note_temp[2];note_temp[0]=0;note_temp[1]=0;note_temp[2]=0; select_note=0; } }  // kill all notes just in case


				incoming_data=0;
		//NOTE ON  1




			}

void note_replace(uint8_t note_replace) {    // replace first note

	if (note_replace)

	{
		note_bank[0] = 1;
		note_bank[1] = note_temp[1];
		note_bank[2] = note_temp[2];
		note_temp[0] = 0;
		note_temp[1] = 0;
		note_temp[2] = 0;
		note_replace_enable=0;
	}

}



/*
void flash_command(uint8_t command,uint32_t address,  uint8_t* pointer,uint16_t len ,uint16_t delay){  // use to operate sd



	test_data3[0]=0x04; //disable write
			  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // low
			  HAL_SPI_Transmit(&hspi1, test_data3, len, 100);
			  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // high end
			  HAL_Delay(delay);




}
*/
