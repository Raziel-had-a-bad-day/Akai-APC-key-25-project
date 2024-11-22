
void stop_start(void);




void play_muting(void){    // all muting stuff here , sometimes it loses data

	uint8_t square_buttons_play [33]= {32,24,16,8,33,25,17,9,34,26,18,10,35,27,19,11,36,28,20,12,37,29,21,13,38,30,22,14,39,31,23,15};  // follows play steps


	if (!pot_states[2])     pot_states[2]=64;  // check if empty
//	uint8_t pot_out=pot_states[2]>>1;
	if ( play_list_write)   play_list[(scene_buttons[0]*32)+seq_step_long]=pot_states[2]>>1;  // keep updating while shift on



	for (i=0;i<32;i++) {// scene memory fill from buttons each time a button is pressed

				uint8_t data_temp2=i+(scene_buttons[0]*32);    // button + scene_select writes all on selected
				uint8_t data_temp3=button_states[square_buttons_play[i]];      // change as its wrong

				if (play_screen==2)	{	if   (play_list[data_temp2 ])		    data_temp3=3;    // this runs first , always
				else  data_temp3=0; 	  }  //write to buttons only when pressed , this i ok

				if ((data_temp3==3)  && (play_screen==3)  && (!play_list[data_temp2]))  {play_list[data_temp2]=  63;  // write if empty

				}

				 button_states[square_buttons_play[i]]=data_temp3;   // getting random trigger


					}
	}
void patch_screen(void)		{     // shows last loaded patch and save patch as well , save=blink
	for (i=0;i<32;i++) {//blank
			button_states[i+7]=0;
		}
		//	button_states[square_buttons_list[load_patch]]=3;   // light currently loaded
			button_states[patch_save+8]=4;   // blink postion to be saved
			//all_update=1;
	}

void main_screen(void){   // shows trigger point for loops

	uint8_t temp_select = midi_channel_list[scene_buttons[0]]; // midicannel> button list
	uint8_t data_temp;

	memcpy(button_states+8,button_states_main+8,32);  // read back

	for (i = 0; i < 32; i++) {     // only on scene select
		data_temp = i + (scene_buttons[0] * 32);

		//button_states[i+8]=button_states_main[i+8]; // read back notes


		if ((!play_screen)&&(!loop_selector)) {
			{
				if (scene_memory[data_temp])
					button_states[i+8] = 5;
				else
					button_states[i+8] = 0;
			}  // should show even if 0 velocity

			if ((device) && (select) && (i == temp_select)) { // show current midichannel red if enabled
				button_states[square_buttons_list[temp_select]] = 3; // show current midichannel red if enabled

			}
		}

	}



}
void loop_screen(void){  // loop screen , runs always when enabled
	//button_states_loop[33];  // stored,loop screen buttons , on/off
	// loop_screen_note_on[33];    //calculated, keeps info on note on 2048 count
	//loop_screen_note_off[33];
	//uint8_t position_counter; // set by loop length
	uint8_t current_scene=scene_buttons[0]*32;
	uint8_t selected_scene=scene_buttons[0];
	//uint8_t loop_end=looper_list[(current_scene*4)+1];
	uint16_t  loop_speed=((looper_list[(selected_scene*4)+2])+1);   // 8-64

	uint16_t loop_length=looper_list[selected_scene*4]+1;
	uint8_t note_length=looper_list[(selected_scene*4)+3]; // only for off
	uint16_t data_temp2;
	uint8_t offset=looper_list[selected_scene*4];
	if(loop_selector==1) {memcpy(button_states_main+8,button_states+8,32);	}

	//loop_screen_last_note[selected_scene]=loop_length; // wiil do for now


	for (i=0;i<32;i++) {// loop notes fill but only current scene , needs to chanee as it keeps writing to wrong scene

		data_temp2=i+(current_scene);    // this all needs to be elsewhere

		if(loop_selector==1){

				if (button_states_loop[data_temp2]) button_states[i+8]=5; else button_states[i+8]=0; } // read back on first select

			if (!button_states[i+8])  {button_states_loop[data_temp2]=0; loop_screen_note_on[data_temp2]=0;} //write from buttons ,test only

			if (button_states_loop[data_temp2]) {

						 loop_screen_note_on[data_temp2]= ((((i*loop_speed))+1)+offset)&255;    //  creates a short playlist , multiplied by speed 1-8
						 loop_screen_note_off[data_temp2]= ((loop_screen_note_on[data_temp2]+note_length)+1)&255;  // note off send

						 loop_screen_last_note[selected_scene]=i;

					}
			//		position_counter++;

					}
					loop_selector=2;

					}



void note_buttons(void){
	//uint8_t scene_select=0;
	//uint8_t button_pressed=255; // sends out changed button ,255 is none
	uint8_t incoming_message[3];
	memcpy(incoming_message,cdc_buffer, 3); // works off only receiving buffer
	//uint8_t button_selection=button_states[incoming_message[1]];
	//uint8_t buffer_clear = 0;
	uint8_t incoming_data1 = incoming_message[1]&127;
	//uint8_t status=incoming_message[0];
	uint8_t current_scene=((scene_buttons[0])*32);
	last_button = square_buttons_list[incoming_data1]+ (current_scene);   // memory location  of note pressed 0-256
			uint8_t last_press = square_buttons_list[incoming_data1];  //0-32
			for (i = 0; i < 40; i++){   // test for bad data
				if (button_states[i]>10) button_states[i]=0;

			}

	if ((!clip_stop) && (!pan) && (!loop_selector)) {     // main screen , normal

		{ //  , only if enabled though ,


			//if (shift) {seq_step_long=last_button&31; play_position=seq_step_long; }

			if (button_states[last_press+8]) { // if button lit but not in play screen
				scene_memory[last_button] = 127; // just to turn on , gets replaced



					if (pause) {
						scene_pitch[last_button] = last_key;
						last_key = 0;}
					else
						scene_pitch[last_button] = scene_transpose[scene_buttons[0]]>>1; // add pitch only with transpose

					scene_velocity[last_button] =(pot_states[1]>>6)+64;
			} else
			{scene_memory[last_button] = 0;scene_velocity[last_button]=0;}

		} // incoming buttons 0 -32
	}    // play screen off and patch screen off

	if ((!clip_stop) && (!pan) && (loop_selector))      // main screen , normal

			{ //  , only if enabled though ,



				if (button_states[last_press+8])  // if button lit but not in play screen
					button_states_loop[last_button]=pot_states[0];   // enter pitch

		}    // play screen off and patch screen off


	if (clip_stop && (!pan)) {     // play mute  screen

		if ((incoming_data1 > 7) && (incoming_data1 < 40)) { //  , only if enabled though ,
			last_button = square_buttons_list[incoming_data1 - 8]
											  + (current_scene) - 8;   // memory location 0-256
		}

	}

		}

void buttons_store(void){    // incoming data from controller

	//uint8_t vel=pot_states[0]>>3;  // testing
	uint8_t alt_list;
	uint8_t scene_select=0;
	uint8_t button_pressed=255; // sends out changed button ,255 is none
	uint8_t incoming_message[3];
	memcpy(incoming_message,cdc_buffer, 3); // works off only receiving buffer
	//uint8_t button_selection=button_states[incoming_message[1]];
	uint8_t buffer_clear = 0;
	uint8_t incoming_data1 = incoming_message[1]&127;
	uint8_t status=incoming_message[0];
	//uint8_t current_scene=((scene_buttons[0])*32);   // current scene in pitch/volume/scene memory list
	uint16_t clear[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint8_t current_scene=scene_buttons[0];
	if (status == 128) // note off
		{note_off_flag[0]=0;


		if (incoming_data1==98)    // shift related functions
			{shift=0;
			play_list_write=0;
			write_velocity=0;}
			}

		  // skip for now

	if ((incoming_data1 <8)&& (status==144)) {   // scene buttons
									scene_select=incoming_data1 +1;}  //enable scene_select section

	if (status == 145)  {if((incoming_data1>47)& (incoming_data1<73)) keyboard[0]=(incoming_data1 -47);}  // store last key pressed mainly , 48-72 default setting(24)  0-24 13 in th emiddle



//	if ((status==129))  { keyboard[0]=0;}  // store last key pressed mainly

	if (status == 144){  // Note on

		note_off_flag[0]=1;note_off_flag[1]=incoming_data1 ;
		if (incoming_data1==98) shift=1;

		if ((incoming_data1>40)&&(incoming_data1 <99) && (!clip_stop) ){				// button lights extras
			alt_list=	 button_states[incoming_data1 ];
			switch(alt_list){    // change state
			case 0 :alt_list = 5;break;   // normal lights , yellow
			case 3 :alt_list= 0;break;		// green
			case 5 :alt_list = 0;  ;break;}
			//case 5 :button_states[incoming_data1 ] = 3;break; }


			button_states[incoming_data1 ]=alt_list;
		}
		if ((incoming_data1>7)&&(incoming_data1 <40) && (!clip_stop) ){				// button lights notes
			alt_list=button_states[(square_buttons_list[incoming_data1])+8];
			switch(alt_list){    // change state
			case 0 :alt_list = 5;break;   // normal lights , yellow
			case 3 :alt_list= 0;break;		// green
			case 5 :alt_list = 0;  ;break;}
			//case 5 :button_states[incoming_data1 ] = 3;break; }

			button_states[(square_buttons_list[incoming_data1])+8]=alt_list;

		}






	/*	 if ((incoming_data1>7)&&(incoming_data1 <99) && (clip_stop) ){				// button lights all for play screen

			 switch(button_selection){    // change state
			 case 0 :button_states[incoming_data1 ] = 3;break;  // play mode lights
			 case 3 :button_states[incoming_data1 ] = 0;break;
			 case 5 :button_states[incoming_data1 ] = 0;  ;break;}
			 //case 5 :button_states[incoming_data1 ] = 3;break; }
			 }*/




		 if (!button_states[64] && (loop_selector)) {loop_selector=0;main_screen();}
		 if (button_states[64] && (!loop_selector)) {loop_selector=1;loop_screen();}


		 if (button_states[83]) {scene_solo=1;} else scene_solo=0;  //enable muting on scene select
		if (button_states[67])  {right_arrow=1;     }  //shift notes right
		if (button_states[86])  {select=1;main_screen();} else select=0;  // select enable

		if (button_states[65]) {down_arrow=1;  	   }		else down_arrow=0;
		if (button_states[85]) { scene_mute=1;} else scene_mute=0;
		if (button_states[93]) { record=1;} else {record=0;} // select enable

		if (button_states[68])  volume=1; else volume=0;
		if (button_states[69])  { pan=1; patch_screen()  ;   }   else pan=0;
		if (button_states[70])  send=1; else send=0;
		if (button_states[71])  {device=1;main_screen();  }else {device=0; }
		if (button_states[81])  {button_states[91]=5;memcpy(seq_step_fine,clear,16); pause=1; seq_step=0;seq_step_long=0;play_position=0;button_states[81]=0; }     // stop all clips, pause and reset to start
	//	if ((button_states[82]) && (!clip_stop) && (!play_screen))  {clip_stop=1; play_screen=2;   play_muting();all_update=1; }  // play screen
		if ((!button_states[82])  && (clip_stop))    {clip_stop=0; play_screen=0;  main_screen(); }
	//	if ((!button_states[81]) && (stop_toggle==2)) {stop_toggle=4; stop_start();}

			button_pressed=incoming_data1; // important  , only after note on
		//	if (button_pressed!=255)  {send_buffer[9]=144; send_buffer[10]=button_pressed;send_buffer[11]=button_states[button_pressed];button_pressed=255;}   // send after one press , maybe retriger for more
			if ((incoming_data1 > 7) && (incoming_data1 < 40)) note_buttons();
		} // end of note on


		    // not very useful



	if (status == 176) {// store pot
		pot_states[incoming_data1  - 48] = incoming_message[2]&127; // store pot all
		if (pan) {
			patch_save=pot_states[7]>>4;   // set next memory to be save d or loaded
			patch_screen();
		}

//		if ((incoming_data1==48) &&(!select) && (scene_buttons[0]<4))  // filter
//		{
//
//			es_filter[scene_buttons[0]+4]=incoming_message[2];   // use filter on es1  or else
//	//	if ( (es_filter[i&3])!=(es_filter[(i&3)+4]))
//
//		{es_filter_cue[0] =scene_buttons[0]+1; es_filter_cue[1] =incoming_message[2]; }
//
//		}

		if ((incoming_data1==48) &&(!select) && (scene_buttons[0]>3))  //  cc function
			{

			//	midi_cc[scene_buttons[0]+4]=incoming_message[2];   // use filter on es1  or else
		//	if ( (es_filter[i&3])!=(es_filter[(i&3)+4]))

			{midi_cc_cue[0] =midi_channel_list[scene_buttons[0]]+176; midi_cc_cue[1] =incoming_message[2]; }

			}


		if ((incoming_data1==49) && shift && (!keyboard[0]) )  write_velocity=(((pot_states[1]&63))+63); // scene_velocity[seq_step_mod+current_scene]=  (((pot_states[1]>>5)<<5)+31)&112;   // update velocity live while pressing shift

		if ((incoming_data1==55) &&(shift)&& (!loop_selector)) {timer_value=bpm_table[incoming_message[2]+64]; tempo=incoming_message[2]+64;} //tempo


		if ((incoming_data1<56)&&(incoming_data1>51)&&(!shift)&& (!loop_selector))  {    // pots 4-8


		if((device) && (select) )   {														///   Midi channel  , maybe elsewhere

		//	button_pressed=square_buttons_list[pot_states[7]>>3]; // last data

		//	midi_channel_list[20]=midi_channel_list[scene_buttons[0]];
			button_states[midi_channel_list[scene_buttons[0]]+8]=0 ;  // turn off
			//midi_channel_list[20]=midi_channel_list[scene_buttons[0]];   // store last setting


		midi_channel_list[scene_buttons[0]]=pot_states[7]>>3;    // add pot value
			button_states[(pot_states[7]>>3)+8]=3;			// turn on light


		//	all_update=3+(pot_states[7]>>6);  // 3+0-4
		//all_update=2;

		  // input midichannel

		//if (scene_buttons[0]>3) scene_volume[incoming_data1-48]= pot_states [incoming_data1-48]; else scene_volume[incoming_data1-52]= pot_states [incoming_data1-48]; // velocity mod on midi out

		}  // end of midi selection
		if ((incoming_message[2] <64))      // crossfade mode
		{	scene_volume[(incoming_data1-52)*2]= 127;      // 0,2,4,6,
		scene_volume[((incoming_data1-52)*2)+1]= 127-((63-incoming_message[2])<<1);} //1,3,5,7
		else
		{	scene_volume[(incoming_data1-52)*2]=127-(( incoming_message[2]-64)<<1);      // 0,2,4,6,
				scene_volume[((incoming_data1-52)*2)+1]=127;} //1,3,5,7

		} //end of pots 4-8
		if ((incoming_data1<56)&&(incoming_data1>51)&&(!shift)&& (loop_selector))  {    // pots 4-8  with loop light on

		if (incoming_data1==52)	looper_list[(scene_buttons[0]*4) ]=(pot_states[4]>>4)&7;  // 1-32 start loop , not in real time just for storing
		if (incoming_data1==53) looper_list[(scene_buttons[0]*4)+1]=(pot_states[5]>>2)&31;   // loop length
		if (incoming_data1==54) looper_list[(scene_buttons[0]*4)+2]=(pot_states[6]>>4)&7; //  playback speed
		if (incoming_data1==55) looper_list[(scene_buttons[0]*4)+3]=(pot_states[7]>>1)&63; // note length


		}

		//	if ((note_off_flag[0])&& (note_off_flag[1]<32))  scene_velocity[square_buttons_list[note_off_flag[1]]+(scene_buttons[0]*32)]=  pot_states[1];    // set velocity for now for held button , only for notes
		if ((incoming_data1==50) && (!keyboard[0]))  // if held down

		{

			scene_transpose[scene_buttons[0]]=pot_states[2]>>1; // 0-64 transpose from base , only with shift off , should trigger a note or loses track

			if (shift)  play_list_write=1; // enter to play list when enabled
			button_pressed=last_button;  //retrigger



		}

		//if ((incoming_data1==49) && (button_states[68]))   scene_velocity[seq_step_pointer]=  (((pot_states[1]>>5)<<5)+31)&112;  // update velocity in realtime if volume button pressed

	} // end of CC (176, pots ) message


	//if((keyboard[0]) )  {pot_tracking[(seq_step_list[scene_buttons[0]]>>3)+(current_scene>>3)]=(keyboard[0]);keyboard[0]=0; }  // use keyboard and shift to enter transpose info ,replaced pot info
	if((keyboard[0]) && (!pause) && (!down_arrow))  {scene_transpose[scene_buttons[0]]=(keyboard[0]+19)&63 ;keyboard[0]=0; }  // use keyboard and shift to enter transpose info ,replaced pot info


	if (scene_select)  { // change scene select lite , one at a time though , fully update so need for extra sends
		scene_select=scene_select-1;


			if (((scene_select)==scene_buttons[0])&& (!scene_mute))  {  if ((button_states[scene_buttons[0]])!=5)  {button_states[scene_select]=5;}
			 }

			if (((scene_select)==scene_buttons[0])&& (scene_mute))  {  if ((button_states[scene_select])==5)  {button_states[scene_select]=3;mute_list[scene_select]=1;}
						else {button_states[scene_select]=5;mute_list[scene_select]=0;} }  // same scene selected




			if ((scene_select)!=scene_buttons[0]){ // different scene selected

					if(loop_selector) loop_selector=1;
				if (!scene_mute)


					{
					if (mute_list[scene_buttons[0]]) button_states[scene_buttons[0]]=3;  else  button_states[scene_buttons[0]]=1;  // leave previous button on red if muted
					if ((button_states[scene_select])!=5) button_states[scene_select]=5; else button_states[scene_select]=1;

					}

			if (scene_mute ){

					if (button_states[scene_select]==3)  {button_states[scene_select]=1;mute_list[scene_select]=0;} else {button_states[scene_select]=3;mute_list[scene_select]=1;}
			}


			scene_buttons[0]=scene_select;


			//all_update=1;


			 if(play_screen) {play_screen=2;   play_muting();} // needed ?

			// if((!play_screen)&&(!loop_selector)) main_screen();


			   // reset play screen
//  buttons from scene memory , ok , data seems to get lost here

			 current_midi=midi_channel_list[scene_buttons[0]];
			} //end of scene select

	
		scene_select=0;

	}

	buffer_clear = 1;
	if(loop_selector) loop_screen();
	if (buffer_clear)
		memcpy(cdc_buffer, clear, 3);


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




void arrows(void){   // disable

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
		//all_update=1;







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
