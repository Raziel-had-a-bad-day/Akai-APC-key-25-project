

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
	uint8_t seq_step_pointer= seq_step+(scene_buttons[0]*32); // scene memory address point
	if (status == 128) // note off
		{note_off_flag[0]=0;
		if (incoming_data1==98) shift=0;



			}

		  // skip for now

	if ((incoming_data1 <8)&& (status==144)) {   // scene buttons
									scene_select=incoming_data1 +1;}  //enable scene_select section

	if (status == 145)  {if((incoming_data1>47)& (incoming_data1<73)) keyboard[0]=(incoming_data1 -47);}  // store last key pressed mainly , 48-72 default setting(24)
	if ((status==129))  { keyboard[0]=0;}  // store last key pressed mainly

	if (status == 144){

		note_off_flag[0]=1;note_off_flag[1]=incoming_data1 ;
		if (incoming_data1==98) shift=1;


		if (incoming_data1==64){			// looping select , pot base or bars 1-4 from start
			switch(loop_selector){
			case 0 :loop_selector++;break;
			case 5 : loop_selector=0;break;


			default: loop_selector++;incoming_data1=255;break;

			}

		}


		 if ((incoming_data1>7)&(incoming_data1 <99)){				// button lights

		 switch(button_selection){    // blank , r
		 case 0 :button_states[incoming_data1 ] = 5;break;
		 case 5 :button_states[incoming_data1 ] = 0;  ;break;}
		 //case 5 :button_states[incoming_data1 ] = 3;break; }
		 }


		if (button_states[65]) scene_mute=1; else scene_mute=0;  //enable muting on scene select
		if (button_states[66]) {scene_solo=1;button_states[65]=0;} else scene_solo=0;  //enable muting on scene select
		button_pressed=incoming_data1; // important  , only after note on


		if ((incoming_data1 >7)&(incoming_data1 <40)&& (button_states[incoming_data1])){     // midi stuff   , only if enabled though

						last_button= square_buttons_list[incoming_data1-8]+(scene_buttons[0]*32)-8;   // memory location 0-256

						scene_velocity[last_button]=  (((pot_states[1]>>5)<<5)+31)&112;


						if (keyboard[0]) {scene_pitch[last_button]= keyboard[0];




						pot_tracking[((last_button&31)>>3)+((scene_buttons[0]-4)*4)]=pot_states[2]>>1; }  // overwrite if keyboard held





	}

	} // change button state

	if (status == 176) {
		pot_states[incoming_data1  - 48] = incoming_message[2]; // store pot
		//	if ((note_off_flag[0])&& (note_off_flag[1]<32))  scene_velocity[square_buttons_list[note_off_flag[1]]+(scene_buttons[0]*32)]=  pot_states[1];    // set velocity for now for held button , only for notes
		if ((incoming_data1==50) && (!keyboard[0]))

		{

			scene_transpose[scene_buttons[0]]=pot_states[2]>>1; // 0-64 transpose from base , only with shift off
		 pot_tracking[(seq_step>>3)+((scene_buttons[0]-4)*4)]=pot_states[2]>>1;  // keep writing per bar
		}

		if ((incoming_data1==49) && (button_states[68]))   scene_velocity[seq_step_pointer]=  (((pot_states[1]>>5)<<5)+31)&112;  // update velocity in realtime if volume button pressed

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
