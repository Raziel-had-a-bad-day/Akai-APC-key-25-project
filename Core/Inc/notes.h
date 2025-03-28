









void play_muting(void){    // all muting stuff here , sometimes it loses data , disable

/*	uint8_t square_buttons_play [33]= {32,24,16,8,33,25,17,9,34,26,18,10,35,27,19,11,36,28,20,12,37,29,21,13,38,30,22,14,39,31,23,15};  // follows play steps


	if (!pot_states[2])     pot_states[2]=64;  // check if empty
//	uint8_t pot_out=pot_states[2]>>1;
	//if ( play_list_write)   play_list[(scene_buttons[0]*32)+seq_step_long]=pot_states[2]>>1;  // keep updating while shift on



	for (i=0;i<32;i++) {// scene memory fill from buttons each time a button is pressed

				uint8_t data_temp2=i+(scene_buttons[0]*32);    // button + scene_select writes all on selected
				uint8_t data_temp3=button_states[square_buttons_play[i]];      // change as its wrong



				 button_states[square_buttons_play[i]]=data_temp3;   // getting random trigger


					}*/
	}
void patch_screen(void)		{     // shows last loaded patch and save patch as well , save=blink
	for (i=0;i<32;i++) {//blank
			button_states[i+7]=0;
		}
		//	button_states[square_buttons_list[load_patch]]=3;   // light currently loaded
			button_states[patch_save+8]=4;   // blink postion to be saved
			//all_update=1;
	}


void loop_screen(void){  // loop screen ,always on now , 16 notes and 16 patterns , full redraw  but notes only

	//uint16_t current_scene=scene_buttons[0]*256;  // pattern select
	uint8_t selected_scene=scene_buttons[0];  // this wiil change to pattern select 0-15
	//uint8_t current_scene_drums=((scene_buttons[0]&3)*4) + (pattern_select*16);  // pattern location

	uint16_t data_temp2;
	uint8_t accent_temp;
	//uint16_t fast_bit;
	uint8_t note_counter=0;
	uint8_t drums=0;
	//uint32_t drum_pattern;   // holds 4 bytes
	//pattern_select=0;
	uint16_t drum_byte_select;
	uint8_t drum_byte;

	drums=1;    //always


	if (drums){



		for (i=0;i<16;i++) {// drums fill

		drum_byte_select= (i>>2)+(selected_scene*4)+(pattern_select*drum_store);  // select byte position
		drum_byte=drum_store_one[drum_byte_select];  // get data



		if (drum_byte & (1<<((i&3)*2))) data_temp2=1; else data_temp2=0;     // note test ok
		if (drum_byte & (1<<((i&3)+1))) accent_temp=1; else accent_temp=0;  // accent test





			if (data_temp2)    // read back on first loop select   , change button state colour
				{if (accent_temp) button_states[square_buttons_list[i]]=3 ;  else button_states[square_buttons_list[i]]=5 ;}
				 else button_states[square_buttons_list[i]]=0;



				note_counter++;



					} // end of loop
	} // end of drums




					//loop_note_count[selected_scene]=note_counter;
					}



void note_buttons(void){  // always running

	uint8_t incoming_message[3];
	memcpy(incoming_message,cdc_buffer+cdc_start, 3); // works off only receiving buffer
	uint16_t clear[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint8_t incoming_data1 = incoming_message[1]&127;



	last_button = square_buttons_list[incoming_data1];   // memory location  of note pressed 0-256(0-3 (16notes)   + sounds*4+(pattern_select*16)

	uint8_t last_press = square_buttons_list[incoming_data1];  //0-32

	uint16_t drum_byte_select= (last_press>>2)+((scene_buttons[0])*4)+(pattern_select*drum_store);  // only for drums
	uint8_t drum_byte=drum_store_one[drum_byte_select];  // get data


			for (i = 0; i < 40; i++){   // test for bad data
				if (button_states[i]>10) button_states[i]=0;

			}



	if ((!pan) && (last_press<16) )      // loop screen , normal , only for drum satm

			{ //  , only if enabled though ,

	//	if (pause) pitch=keyboard[1]+32;  // during pause pitch comes from keyboard
		//pitch=keyboard[1]+32;  // 32+25
				 // if button lit but not in play screen

				switch(button_states[incoming_data1]){    // change state
							case 0 : drum_byte=drum_byte &~ (11<<((last_press&3)*2));   break;   // clear note and accent ,works
						//	case 3 :drum_byte=drum_byte | (1<<((((last_press-(drum_byte_select*4))*2))+1));  break;		// add accent
							case 5 :drum_byte=drum_byte + (1<<((last_press&3)*2)); break;		// note ok


				}


				drum_store_one[drum_byte_select]=drum_byte; //write back info

			}

				if(last_press>15){    // pattern selection
				//uint8_t pattern=pattern_select;
				uint8_t new_pattern=(last_press-16);
				//for (i =8 ; i < 40; i++) {button_states[i]=0;}  // clear all ,  this is ok
				memcpy(button_states+8,clear,32);



					//if (new_pattern==pattern_select)  { button_states[square_buttons_list[new_pattern+16]]=3; pattern_rewind=last_pattern_select+1;}  // enables return
					//else button_states[square_buttons_list[new_pattern+16]]=6;
					if (shift) { button_states[square_buttons_list[new_pattern+16]]=4; pattern_rewind=pattern_select+1; shift=0;}
					else button_states[square_buttons_list[new_pattern+16]]=6;



					new_pattern_select=new_pattern;  //add only to new pattern select


				}


		}

void buttons_store(void){    // incoming data from controller

	//uint8_t vel=pot_states[0]>>3;  // testing
	uint8_t alt_list;
	uint8_t scene_select=0;

	uint8_t incoming_message[3];



	memcpy(incoming_message,cdc_buffer+cdc_start, 3); // works off only receiving buffer
	//uint8_t button_selection=button_states[incoming_message[1]];
	uint8_t buffer_clear = 0;
	uint8_t incoming_data1 = incoming_message[1]&127;
	uint8_t status=incoming_message[0];
	//uint8_t current_scene=((scene_buttons[0])*32);   // current scene in pitch/volume/scene memory list
	uint16_t clear[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint8_t current_scene=scene_buttons[0];
	if (status == 128) // note off
		{  note_off_flag[0]=0;


		if (incoming_data1==98)    // shift related functions
			{shift=0;
			play_list_write=0;
			write_velocity=0;}
			}   // note off buttons

		  // skip for now

	if ((incoming_data1 <8)&& (status==144)) {   // scene buttons
									scene_select=incoming_data1 +1+second_scene;}  //enable scene_select section

	if (status == 145)  {if((incoming_data1>47)& (incoming_data1<73)) keyboard[0]=(incoming_data1 -47);}  // store last key pressed mainly , 48-72 default setting(24)  0-24 13 in th emiddle
	if (status == 129)  {if((incoming_data1>47)& (incoming_data1<73)) keyboard[0]=(incoming_data1 -47)+128;}  // note off keyboard




	if (status == 144){  // Note on  ,buttons

		note_off_flag[0]=1;note_off_flag[1]=incoming_data1 ;
		if (incoming_data1==98) shift=1;

		if ((incoming_data1>40)&&(incoming_data1 <99)  ){				// button lights, extras
			alt_list=	 button_states[incoming_data1 ];
			switch(alt_list){    // change state
			case 0 :alt_list = 1;break;   // normal lights , yellow
			case 5 :alt_list= 0;break;		// green
			case 1 :alt_list = 0;  ;break;
			default:alt_list = 0; ;break; }


			button_states[incoming_data1 ]=alt_list;
		}
		if ((incoming_data1>23)&&(incoming_data1 <40)  ){				// button lights, notes
			alt_list=button_states[incoming_data1];

			switch(alt_list){    // change state
			case 0 :alt_list = 5;break;   // normal lights , yellow
			default:alt_list= 0;break;

			//case 5 :alt_list= 0;break;		// green
			//case 3 :alt_list = 0;  ;break;
			//case 5 :button_states[incoming_data1 ] = 3;break; }
			}
			button_states[incoming_data1]=alt_list;

		}
		if ((incoming_data1>7)&&(incoming_data1 <24) ){				// button lights, pattern select

			}



		 if ((!button_states[64]) && (scene_buttons[0]>7)) {scene_buttons[0]=scene_buttons[0]-8;second_scene=0;loop_screen();}
		 if (button_states[64] && (scene_buttons[0]<8)) 	{scene_buttons[0]=scene_buttons[0]+8;second_scene=8;loop_screen();}


		 if (button_states[83]) {scene_solo=1;} else scene_solo=0;  //enable muting on scene select
		if (button_states[67])  {right_arrow=1;     }  //shift notes right
		if (button_states[86])  {select=1;} else select=0;  // select enable

		if (button_states[65]) {down_arrow=1;  	   }		else down_arrow=0;
		if (button_states[85]) { scene_mute=1;} else scene_mute=0;
		if (button_states[93]) { record=1;} else {record=0;} // select enable

		if (button_states[68])  volume=1; else volume=0;
		if (button_states[69])  { pan=1; patch_screen()  ;   }   else pan=0;
		if (button_states[70])  send=1; else send=0;
		if (button_states[71])  {device=1;  }else {device=0; }
		if (button_states[81])  {button_states[91]=5;memcpy(loop_note_list,clear,16); pause=1; seq_step=0;seq_step_long=0;play_position=0;button_states[81]=0; }     // stop all clips, pause and reset to start

		if ((button_states[82])  )    {clip_stop=1;   } else clip_stop=0;
	//	if ((!button_states[81]) && (stop_toggle==2)) {stop_toggle=4; stop_start();}

			//button_pressed=incoming_data1; // important  , only after note on
		//	if (button_pressed!=255)  {send_buffer[9]=144; send_buffer[10]=button_pressed;send_buffer[11]=button_states[button_pressed];button_pressed=255;}   // send after one press , maybe retriger for more
			if ((incoming_data1 > 7) && (incoming_data1 < 40)) note_buttons();
		} // end of note on


		    // not very useful
	if ((status == 176) && (clip_stop)){


	alt_pots[incoming_data1  - 48] =(incoming_message[2]&127);

		status=0; // clear
	}


	if (status == 176) {//  controller data , store pot



		pot_states[incoming_data1  - 48] = incoming_message[2]&127; // store pot all  // not always ok






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

		if ((incoming_data1==48) &&(!select) && (current_scene>3))  //  cc function
			{

			//	midi_cc[scene_buttons[0]+4]=incoming_message[2];   // use filter on es1  or else
		//	if ( (es_filter[i&3])!=(es_filter[(i&3)+4]))

			{midi_cc_cue[0] =midi_channel_list[current_scene]+176; midi_cc_cue[1] =incoming_message[2]; }

			}


		if ((incoming_data1==49) && shift && (!keyboard[0]) )  write_velocity=(((pot_states[1]&63))+63); // scene_velocity[seq_step_mod+current_scene]=  (((pot_states[1]>>5)<<5)+31)&112;   // update velocity live while pressing shift




		if ((shift) && (device)){   // pots alt functions set , tempo , midi

			if (incoming_data1==54)   {midi_channel_list[current_scene]=(incoming_message[2]>>3);current_midi=midi_channel_list[current_scene];}   // set midi

			if (incoming_data1==55) {timer_value=bpm_table[incoming_message[2]+64]; tempo=incoming_message[2]+64;} //tempo



		}



/*
		if ((incoming_data1<56)&&(incoming_data1>51)&&(!shift)&& (!loop_selector))  {    // pots 4-8


		if ((incoming_message[2] <64))      // crossfade mode
		{	scene_volume[(incoming_data1-52)*2]= 127;      // 0,2,4,6,
		scene_volume[((incoming_data1-52)*2)+1]= 127-((63-incoming_message[2])<<1);} //1,3,5,7
		else
		{	scene_volume[(incoming_data1-52)*2]=127-(( incoming_message[2]-64)<<1);      // 0,2,4,6,
				scene_volume[((incoming_data1-52)*2)+1]=127;} //1,3,5,7

		} //end of pots 4-8
*/
		if ((incoming_data1<56)&&(incoming_data1>51)&& (!device))  {    // pots 4-8  , with device button off

		if (incoming_data1==52)	{if(shift)    loop_length_set[current_scene]=pot_states[4]>>3;   // works ok  sets loop length 0-16
		else looper_list[(current_scene*4) ]=(pot_states[4]>>2)&31;  // 1-32 start loop  fine offset 8/note  ,
		}
		if (incoming_data1==53){ looper_list[(current_scene*4)+1]=(pot_states[5]>>3)&15;pattern_offset_list[pattern_select]= looper_list[(12*4)+1];} // position offset but only from keys first entry  ,stored per pattern not per note ?
		if (incoming_data1==54) looper_list[(current_scene*4)+2]=(pot_states[6]>>4)&7; //  lfo gain
		if ((incoming_data1==55)&&(!shift)) note_accent[current_scene]=pot_states[7];  // accent also used for tempo with shift


		}

		//	if ((note_off_flag[0])&& (note_off_flag[1]<32))  scene_velocity[square_buttons_list[note_off_flag[1]]+(scene_buttons[0]*32)]=  pot_states[1];    // set velocity for now for held button , only for notes


			if ((incoming_data1==48) && (shift)){     // change program pot 1
				if (scene_buttons[0]<12) program_change[0]=pot_states[0]>>3;   else program_change[1]=pot_states[0]>>3;



			}

		if ((incoming_data1==50) && (!keyboard[0]))  // if held down

		{

			scene_transpose[current_scene]=pot_states[2]>>1; // 0-64 transpose from base , only with shift off , should trigger a note or loses track

			if (shift)  play_list_write=1; // enter to play list when enabled




		}

		//if ((incoming_data1==49) && (button_states[68]))   scene_velocity[seq_step_pointer]=  (((pot_states[1]>>5)<<5)+31)&112;  // update velocity in realtime if volume button pressed

	} // end of CC (176, pots ) message



	if (scene_select)  { // change scene select lite , one at a time though , fully update so need for extra sends
		scene_select=scene_select-1;
		uint8_t clear_green[8]= {1,1,1,1,1,1,1,1};

		memcpy(button_states,clear_green,8) ;  // turn green
		button_states[scene_select&7]=5;



			scene_buttons[0]=scene_select;

			 current_midi=midi_channel_list[scene_buttons[0]];




			 //loop_selector=1; // redraw
	
		scene_select=0;

	}// end of scene select
	buffer_clear = 1;
	loop_screen();  // always run
	if (buffer_clear)
		memcpy(cdc_buffer+cdc_start, clear, 3);


	} // end of button store



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


void pattern_settings(void){     // pattern change function
	if (s_temp==0){
		//uint16_t clear[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


		if ((pattern_select!=new_pattern_select)){    // switch pattern at the endo of the loop to new

			button_states[square_buttons_list[pattern_select+16]]=0;
		pattern_select=new_pattern_select; // switch pattern to new
		if ((pattern_rewind)) button_states[square_buttons_list[new_pattern_select+16]]=3; else button_states[square_buttons_list[new_pattern_select+16]]=5;


		loop_screen();


		}
		if (pattern_rewind) {new_pattern_select=pattern_rewind-1;

		button_states[square_buttons_list[pattern_select+16]]=3;pattern_rewind=0;}   // fake press


		//memcpy(loop_note_list,clear,16);   // reset on pattern select , running always atm


	} //end of s_temp


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
