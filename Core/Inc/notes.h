

void buttons_store(void){

	//uint8_t vel=pot_states[0]>>3;  // testing
	uint8_t scene_select=0;
	uint8_t button_pressed=255; // sends out changed button ,255 is none
	uint8_t incoming_message[3];
	memcpy(incoming_message,cdc_buffer, 3); // works off only receiving buffer
	uint8_t button_selection=button_states[incoming_message[1]];
	uint8_t buffer_clear = 0;
	uint8_t incoming_data1 = incoming_message[1]&127;
	uint8_t status=incoming_message[0];
	uint8_t current_scene=((scene_buttons[0])*32);


	//uint8_t seq_step_pointer= seq_step+(scene_buttons[0]*32); // scene memory address point
	if (status == 128) // note off
		{note_off_flag[0]=0;
		if (incoming_data1==98) shift=0;



			}

		  // skip for now

	if ((incoming_data1 <8)&& (status==144)) {   // scene buttons
									scene_select=incoming_data1 +1;}  //enable scene_select section

	if (status == 145)  {if((incoming_data1>47)& (incoming_data1<73)) keyboard[0]=(incoming_data1 -47);}  // store last key pressed mainly , 48-72 default setting(24)





	if ((status==129))  { keyboard[0]=0;}  // store last key pressed mainly

	if (status == 144){  // Note on

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
		 case 3 :button_states[incoming_data1 ] = 0;break;
		 case 5 :button_states[incoming_data1 ] = 0;  ;break;}
		 //case 5 :button_states[incoming_data1 ] = 3;break; }
		 }



		if (button_states[83]) {scene_solo=1;} else scene_solo=0;  //enable muting on scene select
		if (button_states[67])  {right_arrow=1;     }  //shift notes right
		if (button_states[86])  select=1; else select=0;  // select enable
		if (button_states[85]) { scene_mute=1;} else scene_mute=0;
		if (button_states[68])  volume=1; else volume=0;
		if (button_states[69])  pan=1; else pan=0;
		if (button_states[71])  device=1; else device=0;


		button_pressed=incoming_data1; // important  , only after note on




		if ((incoming_data1 >7)&&(incoming_data1 <40)&& (button_states[incoming_data1])){     // midi stuff   , only if enabled though

						last_button= square_buttons_list[incoming_data1-8]+(current_scene)-8;   // memory location 0-256


						if (select) {
							scene_velocity[last_button]=  (((pot_states[1]>>5)<<5)+31)&112;
							scene_pitch[last_button]= pot_states[0]>>2;


						}   // only use if select enabled , otherwise leave



						if (scene_velocity[last_button]==0) button_states[incoming_data1 ] = 3;  // change to red if 0 velocity


						if (keyboard[0]) scene_pitch[last_button]= keyboard[0]; // replace if pressed
		}



	} // change button state

	if (status == 176) {
		pot_states[incoming_data1  - 48] = incoming_message[2]&127; // store pot
		if ((incoming_data1==55) &&(shift)) {timer_value=bpm_table[incoming_message[2]+64]; tempo=incoming_message[2]+64;} //tempo


		if ((incoming_data1<56)&&(incoming_data1>51)&&(!shift))  {
		//	midi_cc=1;   // volume cc message enable   , for now disabled
		midi_cc_list[0]=176+scene_buttons[0];
		midi_cc_list[1]=7; //volume control
		midi_cc_list[2]=incoming_message[2]&127;
		midi_cc_list[12]=3;

		if((device) && (select) )   {

			button_pressed=square_buttons_list[pot_states[7]>>3]; // last data
			button_states[square_buttons_list[midi_channel_list[scene_buttons[0]]]]=0;  // turn off

			midi_channel_list[scene_buttons[0]]=pot_states[7]>>3;button_states[square_buttons_list[pot_states[7]>>3]]=3;
		all_update=3+(pot_states[7]>>6);  // 3+0-4


		}   // input midichannel

		if (scene_buttons[0]>3) scene_volume[incoming_data1-48]= pot_states [incoming_data1-48]; else scene_volume[incoming_data1-52]= pot_states [incoming_data1-48]; // velocity mod on midi out

		}



		//	if ((note_off_flag[0])&& (note_off_flag[1]<32))  scene_velocity[square_buttons_list[note_off_flag[1]]+(scene_buttons[0]*32)]=  pot_states[1];    // set velocity for now for held button , only for notes
		if ((incoming_data1==50) && (!keyboard[0]))

		{

			scene_transpose[scene_buttons[0]]=pot_states[2]>>1; // 0-64 transpose from base , only with shift off




		}

		//if ((incoming_data1==49) && (button_states[68]))   scene_velocity[seq_step_pointer]=  (((pot_states[1]>>5)<<5)+31)&112;  // update velocity in realtime if volume button pressed

	}


	if((keyboard[0]) && (shift))  {pot_tracking[(seq_step_list[scene_buttons[0]]>>3)+(current_scene>>3)]=(keyboard[0]);keyboard[0]=0; }  // use keyboard to enter transpose info , also mute

	if (scene_select)  { // change scene select lite , one at a time though , fully update so need for extra sends
		scene_select=scene_select-1;


			if (((scene_select)==scene_buttons[0])&& (!scene_mute))  {  if ((button_states[scene_buttons[0]])!=5)  {button_states[scene_select]=5;}
			 }

			if (((scene_select)==scene_buttons[0])&& (scene_mute))  {  if ((button_states[scene_select])==5)  {button_states[scene_select]=3;mute_list[scene_select]=1;}
						else {button_states[scene_select]=5;mute_list[scene_select]=0;} }




			if ((scene_select)!=scene_buttons[0]){
				if (!scene_mute)


					{
					if (mute_list[scene_buttons[0]]) button_states[scene_buttons[0]]=3;  else  button_states[scene_buttons[0]]=1;  // leave previous button on red if muted
					if ((button_states[scene_select])!=5) button_states[scene_select]=5; else button_states[scene_select]=1;

					}

			if (scene_mute ){

					if (button_states[scene_select]==3)  {button_states[scene_select]=1;mute_list[scene_select]=0;} else {button_states[scene_select]=3;mute_list[scene_select]=1;}
			}






			scene_buttons[0]=scene_select;
			all_update=1;
			uint8_t temp_select=midi_channel_list[scene_buttons[0]];// midicannel> button list


			for (i=0;i<32;i++){

					{if (scene_memory[i+((scene_select)*32)]>>5) button_states[square_buttons_list[i]]=5; else button_states[square_buttons_list[i]]=0;}

							if((device) && (select) &&  (i==temp_select))  {
								button_states[square_buttons_list[temp_select]]=3; // show current midichannel red if enabled

							}



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




void arrows(void){

	if (right_arrow)
	{
		uint8_t scene=scene_buttons[0];
		uint8_t temp_hold;
		uint8_t temp[33];
		uint8_t point=scene*32;
		temp_hold= scene_memory[(point)+31];
		memcpy(temp+1,scene_memory+point,31);
		temp[0]=temp_hold;
		memcpy(scene_memory+point,temp,32);

				temp_hold= scene_velocity[(point)+31];
				memcpy(temp+1,scene_velocity+point,31);
				temp[0]=temp_hold;
				memcpy(scene_velocity+point,temp,32);

				temp_hold= scene_pitch[(point)+31];
								memcpy(temp+1,scene_pitch+point,31);
								temp[0]=temp_hold;
								memcpy(scene_pitch+point,temp,32);

								temp_hold= button_states[39];
																memcpy(temp+1,button_states+8,31);
																temp[0]=temp_hold;
																memcpy(button_states+8,temp,32);


			button_states[67]=0;
		right_arrow=0;
		all_update=1;







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
