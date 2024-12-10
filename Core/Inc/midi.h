




void midi_send(void){  // produces midi data from notes etc ,only for midi music no info return ,  runs 8x per step

		uint8_t cue_counter=0;
		uint16_t velocity=0;
		uint16_t seq_step_mod=seq_step_list[0];
		uint8_t scene=scene_buttons[0];

		 uint8_t retrigger=0; // enable if seq_step hasn't_ moved
		 uint8_t play_list_mute=1;
		 uint8_t note_enable=0;
		 uint8_t current_note;
		 uint8_t loop_note;
		 uint8_t i_s;
		 uint16_t data_temp2;
		 uint16_t current_pos=seq_pos&255;   // +8 to 2048 , gotta reset not much point otherwise
		 uint16_t offset=0;   // time offset
		 uint8_t offset_pitch=0;
		 uint8_t offset_vel=0; // velocity offset compared to notes
		 uint8_t offset_lfo=0;


		for (i=0;i<8;i++){    //  this really needs to go away


			offset_lfo= loop_lfo_out[i+20]; // 0-4
			offset=(looper_list[i*4]+current_pos+offset_lfo)&255;   // seq_pos+offset 0-255

			offset_pitch=offset>>3;
			offset_vel=(looper_list[(i*4)+1]+offset_pitch)&31;   // vel offset 0-31


			seq_step_mod=offset_pitch;   // 0-31 on loop screen

			i_s=i*32;
				cue_counter=i*3;
			data_temp2=(seq_step_mod)+(i_s);   //0-255

			if ((loop_screen_note_on[offset] & (1<<i))  )     note_enable=1;  // turn on note from loop list





		//	if (i!=0) note_enable=0;
			   // stays for now ,used only for setting keychange
		//	if (seq_step_list[i]>>5) retrigger=1; else retrigger=0;    // ESSENTIAL  stop retrigger if same position

		//	if (play_list[data_temp2 ])play_list_mute=1; else play_list_mute=0;     // disable play muting if looping
		//	scene_transpose[i]= play_list[data_temp2 ]; // replaces transpose



						if ( (!mute_list[i]) &&  play_list_mute  	&& (!retrigger) &&
								(note_enable)    // important , note on trigger now


						)     {    //  NOTE ON   ,disable if retrigger, plays from scene memory but only for note one

							midi_cue[cue_counter]=midi_channel_list[i]+144;  // get midi channel

							midi_cue[(cue_counter)+1]=((button_states_loop[data_temp2]))& 127;;  //  pitch info +transpose but only from play_list

							//		if ((nrpn_cue[(cue_counter+1)])!=(midi_cue[(cue_counter)+1]))				{	nrpn_cue[cue_counter]=i+1; nrpn_cue[(cue_counter+1)]=midi_cue[(cue_counter)+1];  }// change nrpn value only if needed
						//	else 	nrpn_cue[cue_counter]=0;

							if (midi_channel_list[i]==9)midi_cue[(cue_counter)+1]=drum_list[i];  // use drum list if set to  channel 10

							if(mute_list[i]) midi_cue[cue_counter]=0; //send nothing // IMPORTANT OR WILL SEND GARBAGE //

							velocity=(scene_velocity[offset_vel+i_s])&127;   // use only 3 bit msb

							velocity= (velocity*scene_volume[i])>>7;   // might change this to accent control


							if ((scene_solo) && (scene!=i)) velocity=0;   // mute everything but solo
							midi_cue[(cue_counter)+2]=velocity&127;
							//cue_counter++;
						note_enable=0;
						} else {midi_cue[cue_counter]=0;nrpn_cue[cue_counter]=0;}      // end of note on


						seq_step_enable[i]=0;note_enable=0;
		}   // end of loop
		midi_cue[50]=cue_counter;


		}






	void note_off(void) {      // way off here



		uint8_t counterb=0;
		//uint8_t send_temp3[180];


			//memcpy(send_temp3,midi_cue_noteoff,len1);
			for (i=0;i<8;i++){

				counterb=i*3;

				if (midi_cue[counterb]   || (seq_pos==254 ))   {    // only if new message otherwise leave alone , not enough

					midi_cue_noteoff[counterb] = noteoff_list[counterb];  // copy old  channel to send out
					noteoff_list[counterb]= midi_cue[counterb];  // replace old with new for later


					// if(! (noteoff_list[counterb+2]))   midi_cue_noteoff[(counterb)+1] =noteoff_list[counterb+1];   // if vel is 0 then keep old note
					midi_cue_noteoff[(counterb)+1] =noteoff_list[counterb+1];
					noteoff_list[(counterb)+1]= midi_cue[counterb+1];

					midi_cue_noteoff[(counterb)+2] =0;
					noteoff_list[(counterb)+2]= 0;

			} else    midi_cue_noteoff[counterb] =0;



			}
			//if ((seq_step==31 ) ) memcpy(midi_cue_noteoff,noteoff_list,24);   // flush everything at end ?  may be a little much






		}


void cdc_send(void){     // all midi runs often , need to separate

	//UART_HandleTypeDef huart1;
			uint8_t len;  // note on
			uint8_t send_temp[256];
			uint8_t len1;  // note off
			uint8_t cue_counter;
			uint8_t counterb;
			uint8_t note_midi[50];
			uint8_t nrpn_temp[100];
			uint8_t note_off_midi[50];
			uint8_t cc_temp[22];
			uint8_t seq_step_mod=seq_step_list[scene_buttons[0]]&31;
			uint8_t cue_counter2=0;
			uint8_t nrpn_chl=185;
			cc_temp[20]=0;
			//memcpy(cue_temp,midi_cue,25);



			cue_counter=0;
			for (i=0;i<8;i++){  // short , ready to send notes only
				counterb=i*3;


				if (midi_cc_cue[0]){       // send a single  cc  if true

					cc_temp[0]=midi_cc_cue[0]; // channel cc
					cc_temp[1]=74;  // cutoff
					cc_temp[2]=midi_cc_cue[1];
					cc_temp[20]=3;
					midi_cc_cue[0]=0;

				}

				if (midi_cue[counterb]){						  // note send if true

					note_midi[cue_counter]=midi_cue[counterb];
					note_midi[(cue_counter)+1]=midi_cue[counterb+1];
					note_midi[(cue_counter)+2]=midi_cue[counterb+2];
					midi_cue[counterb]=0;   // clear when used
					cue_counter=cue_counter+3;


				}}

			note_midi[50]=cue_counter;

			nrpn_temp[80]=0;   //disable nrpn for now

				//memcpy(cue_temp,midi_cue_noteoff,25);
				cue_counter=0;
				for (i=0;i<8;i++){  // short , ready to send notes off only
					counterb=i*3;

					if (midi_cue_noteoff[counterb]){
					note_off_midi[cue_counter]=midi_cue_noteoff[counterb];
					note_off_midi[(cue_counter)+1]=midi_cue_noteoff[counterb+1];
					note_off_midi[(cue_counter)+2]=midi_cue_noteoff[counterb+2];
					midi_cue_noteoff[counterb]=0;  // clear when used


					cue_counter=cue_counter+3;


					}}
				note_off_midi[50]=cue_counter;


				len1=note_off_midi[50];

					len=note_midi[50];
				//	if(pause) len=0;

			memcpy(send_temp,note_off_midi,len1); // adding  Note off



			memcpy(send_temp+len1,note_midi,len);     // adding Note on

			 if (pause)  len=len1; else len=len+len1;     // add note off and note on

			 if (pause==2)   len=0;   // disable all after last send



			 serial_len=len;



			 memcpy(serial_out, midi_extra_cue, midi_extra_cue[28]); // extra stuff sent , anything
			 // start of serial send
			 memcpy(serial_out + midi_extra_cue[28], send_temp, serial_len);
			 serial_len = serial_len + midi_extra_cue[28];
			 midi_extra_cue[28] = 0;
			 memcpy(serial_out + serial_len, nrpn_temp, nrpn_temp[80]); // temp only !  add nrpn
			 serial_len = serial_len + nrpn_temp[80];
			 nrpn_temp[80] = 0;
			 if (pause)
				 pause = 2;

					// empty
//					memcpy(serial_out+serial_len,cc_temp,cc_temp[20]);    // cc send
//
//
//
//					serial_len=serial_len+cc_temp[20];
					cc_temp[20]=0;
			// end of serial send

			//  only for midi controller

// NRPN


		//	nrpn_cue[50]=0;
//

	if (cdc_len == 0) {	// only process after cdc send and only cdc send stuff , only  controller now

		len=0;

			if (play_screen) seq_step_mod=((play_position&3)*8) + ((play_position>>2)&7);
		//	uint8_t button_exception1=square_buttons_list[((seq_step_mod ) & 31)]; // 0-40
			uint8_t button_exception1=((seq_step_mod ) & 31);
			uint8_t scene_select=scene_buttons[0];
			uint8_t button_colour=0;
			uint8_t loop_length=loop_screen_last_note[scene_select];
			uint8_t divider;


			  if (loop_selector)
			  {
				  switch(loop_length>>2){

				  case 0:  divider=7;break;
				  case 3:divider=15;break;
				  case 7:divider=31;break;
				  default:divider=31;break;
				  }

				  button_exception1=(seq_step_fine[scene_select]>>3)&divider;
				 }


			if (record) button_colour=4; else button_colour=1;

		all_update=1;

		if((all_update==1)){   // update all square scene lights on main screen

			  for (i=0;i<32;i++)  {

				  uint8_t display_button=button_states[i+8]; // just holds normal 0-32 number value
				  uint8_t alt_list=square_buttons_list[i];

			  if (((button_states_save[i]) != display_button) && (i!=button_exception1))  // send if changed
			  {
				  send_temp[len]=144;
				  send_temp[len+1]=alt_list;
				  send_temp[len+2]=display_button;  // only this part needs to change , all other normal
				 	len=len+3;
				  button_states_save[i] = display_button;
			  }

			  if((i==button_exception1) && (button_states_save[i]!=button_colour)) {   // green moving button , seems ok
			  send_temp[len]=144;
			  send_temp[len+1]=alt_list;
			  send_temp[len+2]=button_colour;
			  len=len+3;
			  button_states_save[i] =button_colour;
			  }

			  }

			  for (i=0;i<8;i++)  {  //selection

			  				  uint8_t display_button=button_states[i]; // just holds normal 0-32 number value


			  			  if ((button_states_save[i+32] != display_button))  // send if changed
			  			  {
			  				  send_temp[len]=144;
			  				  send_temp[len+1]=i;
			  				  send_temp[len+2]=display_button;  // only this part needs to change , all other normal
			  				 	len=len+3;
			  				  button_states_save[i+32] = display_button;
			  			  }
			  }





		if (send_buffer[6]) {
			memcpy(send_temp + len, send_buffer + 6, 3); // extra buttons, ok
			len = len + 3;
			send_buffer[6] = 0;

		}
		all_update = 0;
		}

/*

		if((all_update==1)&&(loop_selector)){   // update all square scene lights on loop screen

					  for (i=0;i<40;i++)  {


					  if (((button_states_save[i]) != button_states_loop[i]) && (i!=button_exception1))  // send if changed
					  {
						  send_temp[len]=144;
						  send_temp[len+1]=i;
						  send_temp[len+2]=button_states[i];
						 	len=len+3;
						  button_states_save[i] = button_states_loop[i];
					  }
					  if((i==button_exception1) && (button_states_save[i]!=button_colour)) {   // green moving button
					  send_temp[len]=144;
					  send_temp[len+1]=i;
					  send_temp[len+2]=button_colour;
					  len=len+3;
					  button_states_save[i] =button_colour;
					  }

					  }

				if (send_buffer[6]) {
					memcpy(send_temp + len, send_buffer + 6, 3); // extra buttons, ok
					len = len + 3;
					send_buffer[6] = 0;

				}
				all_update = 0;
				}
*/







	//	if (send_buffer_sent==2){
//			memcpy(send_temp + len, send_buffer, 6);  // green moving button ,ok
	//	len = len + 6;

		send_buffer_sent=1;


		memcpy(cdc_send_cue, send_temp, len);    // copy for cdc
		cdc_len = len;
	}




//send all if possible , after each step midi notes first  // might change
			//HAL_UART_Transmit(&huart1,serial_out,serial_len,100); // uart send
//			if (midi_cc) midi_cue[50]= midi_cue[50]- midi_cc_list[12];   // remove cc
//			midi_cc_list[12]=0;
//			midi_cc=0;   // disable cc



		}


void nrpn_send(void){			 // all nrpn data goes here ,sends as well on serial



	//UART_HandleTypeDef huart1;
			uint8_t len;  // note on
			uint8_t send_temp[256];
			uint8_t len1;  // note off
			uint8_t cue_counter;
			uint8_t counterb;
			uint8_t note_midi[50];
			uint8_t nrpn_temp[100];
			uint8_t note_off_midi[50];
			uint8_t cc_temp[22];

			uint8_t cue_counter2=0;
			uint8_t nrpn_chl=185;
			cc_temp[20]=0;
			//memcpy(cue_temp,midi_cue,25);



			cue_counter=0;
			for (i=0;i<8;i++){  // short , ready to send notes only
				counterb=i*3;


			//	if (nrpn_cue[counterb] || (es_filter_cue[0])){				// NRPN section and filter
					if (nrpn_cue[counterb] ){				// NRPN section   sent if true
						nrpn_temp[cue_counter2] = nrpn_chl; // CC99  , ch 10
						nrpn_temp[cue_counter2+1] =99;
						nrpn_temp[cue_counter2+2] =5;
						nrpn_temp[cue_counter2+3] = nrpn_chl;  //CC98
						nrpn_temp[cue_counter2+4] =98;

						{nrpn_temp[cue_counter2+5] =(nrpn_cue[counterb]-1)*8;   nrpn_temp[cue_counter2+8] =pitch_lut[nrpn_cue[counterb+1]&127]; } // data,   pitch translate

						nrpn_temp[cue_counter2+6] = nrpn_chl;
						nrpn_temp[cue_counter2+7] =6;

						nrpn_cue[counterb]=0;     // clear once used
						cue_counter2=cue_counter2+9;

				}

			}

			nrpn_temp[80]=cue_counter2;  // can be a trimmed a lot



}

		void loop_lfo(void) {  // 0-64-0

			uint16_t temp_hold=0;
			for (i=0;i<8;i++){



				if ((loop_lfo_out[i+10]==0) && (loop_lfo_out[i]==63))	loop_lfo_out[i+10]=2;
				if (loop_lfo_out[i+10]==0) loop_lfo_out[i]= (loop_lfo_out[i]+1)&255;
				if ((loop_lfo_out[i+10]==2) && (loop_lfo_out[i]==0))	loop_lfo_out[i+10]=0;
				if (loop_lfo_out[i+10]==2) loop_lfo_out[i]= (loop_lfo_out[i]-1)&255;

				temp_hold=((loop_lfo_out[i]*looper_list[(i*4)+2])>>7);

						loop_lfo_out[i+20]=temp_hold;




			}





		}

