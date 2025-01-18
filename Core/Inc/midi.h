




void midi_send(void){  // produces midi data from notes etc ,only for midi music no info return ,  runs 8x per step

		uint8_t cue_counter=0;
		uint16_t velocity=0;
		//uint16_t seq_step_mod=seq_step_list[0];
		uint8_t scene=scene_buttons[0]; // only for drums atm
		uint8_t random_list[16]={12,12,12,12,12,12,12,12,12,12,12,12,46,65,23,45}; // random notes for now
		memcpy (random_list,alt_pots,8);
		memcpy (random_list+8,alt_pots,8);



		uint8_t looper_list_temp[sound_set*4];
		uint8_t lfo_out[sound_set];
		uint16_t loop_note_temp[sound_set];
		memcpy (looper_list_temp,looper_list,sound_set*4); // need this
		memcpy(lfo_out,loop_lfo_out+32,sound_set);  // time adjust , maybe change to controller data
		memcpy(loop_note_temp,loop_note_list,sound_set*2); // 16 bit

		// uint8_t retrigger=0; // enable if seq_step hasn't_ moved
		// uint8_t play_list_mute=1;
		 uint8_t note_enable=0;
		// uint8_t current_note;
		// uint8_t note_length= pot_states[3]>>4;
		// uint8_t i_s;
		// uint16_t data_temp2;
		 //uint16_t data_temp3;
		 uint16_t current_pos=seq_pos&127;   // +8 to 2048 , gotta reset not much point otherwise
		 uint16_t offset=0;   // time offset
		 uint8_t offset_pitch=0;
		 uint8_t offset_vel=0; // velocity offset compared to notes
		 uint8_t offset_lfo=0;
		 uint16_t offset2=0;
		 uint16_t pattern=pattern_select;




		for (i=0;i<sound_set;i++){    //  this really needs to go away


			//offset_lfo= lfo_out[i]; // 0-4
			if (retrigger_countdown[i]) retrigger_countdown[i]=retrigger_countdown[i]-1;// space out notes, always run, blocks playback for a minimum amount of time
			//offset2=((32-looper_list_temp[i*4])+offset_lfo)&31;  // base offset ,fine 1/8
			offset2=((looper_list_temp[i*4])+offset_lfo)&31;  // base offset ,fine 1/8

			offset=(((128-offset2)+current_pos)&127)+(pattern*128);   // seq_pos+offset 0-255

			//i_s=i*32;
				cue_counter=i*3;
				//offset=current_pos;
				//if (!offset) loop_note_list[i]=31;  // reset to start , rough but works

			//if ((loop_screen_note_on[offset] & (1<<i)) && (!retrigger_countdown[i])) // remove this
				if ((7==(offset&7)) && (!retrigger_countdown[i]))
			{note_enable=1;        // always triggered now

			if (loop_note_temp[i]>= loop_length_set[i]) loop_note_temp[i]=0; else loop_note_temp[i]++; } // works for notes only

						if ( (!mute_list[i])  	&&
								(note_enable)    // important , note on trigger now



						)     {    //  NOTE ON   ,disable if retrigger, plays from scene memory but only for note one



							loop_note_temp[i]=seq_pos>>3; // force for now

							offset_pitch=loop_note_temp[i]; // note position, 0-16 this is ok


							midi_cue[cue_counter]=0; // start with mute just in case
							//offset_vel=((looper_list_temp[(i*4)+1])+loop_note_temp[i])&15;
							offset_vel=(pattern_offset_list[pattern_select]+loop_note_temp[i])&15;


							if (midi_channel_list[i]!=9)offset_pitch=offset_vel; // force trigger to mvoe as well


							//if ((button_states_loop[data_temp2]!=1))   // keys

							uint16_t drum_byte_select= (offset_pitch>>2)+(i*4)+(pattern*drum_store);
							uint8_t drum_byte=drum_store_one[drum_byte_select];


							if (drum_byte & (1<<((offset_pitch&3)*2)))


							{retrigger_countdown[i]=3;    // start countdown to avoid fast note repeats

								midi_cue[cue_counter]=(midi_channel_list[i]&15)+144; } // get midi channel




							if(mute_list[i]) midi_cue[cue_counter]=0; //send nothing // IMPORTANT OR WILL SEND GARBAGE //


							velocity=127;


							if (midi_channel_list[i]==9)midi_cue[(cue_counter)+1]=drum_list[i];

							else{
								midi_cue_noteoff[cue_counter]=midi_cue[cue_counter]; // note off channel
								midi_cue_noteoff[cue_counter+1]=last_note_played[i]; // note off
								midi_cue[(cue_counter)+1]=random_list[offset_vel];  // use drum list if set to  channel 10
							last_note_played[i]= midi_cue[(cue_counter)+1]; // save note on the same channel

							}
							if ((scene_solo) && (scene!=i)) velocity=0;   // mute everything but solo


							if (!velocity) midi_cue[cue_counter]=0;  // simply disable

							midi_cue[(cue_counter)+2]=velocity&127;

						note_enable=0;
						}



		}   // end of loop
		//midi_cue[50]=cue_counter; // doesnt do anything

		memcpy(loop_note_list,loop_note_temp,sound_set*2);
		}






	void note_off(void) {      // not used



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
			uint8_t note_midi [70] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // seems to get some garbage ?
			uint8_t nrpn_temp[100];
			uint8_t note_off_midi[50];
			uint8_t cc_temp[22];
			//uint8_t seq_step_mod=seq_step_list[scene_buttons[0]]&31;
			uint8_t seq_step_mod=(seq_pos>>3)&31;

			//uint8_t cue_counter2=0;
			//uint8_t nrpn_chl=185;
			cc_temp[20]=0;
			//memcpy(cue_temp,midi_cue,25);



			cue_counter=0;
			for (i=0;i<sound_set;i++){  // short , ready to send notes only
				counterb=i*3;


				if (midi_cc_cue[0]){       // send a single  cc  if true

					cc_temp[0]=midi_cc_cue[0]; // channel cc
					cc_temp[1]=74;  // cutoff
					cc_temp[2]=midi_cc_cue[1];
					cc_temp[20]=3;
					midi_cc_cue[0]=0;

				}

				if (midi_cue[counterb]){						  // most data ,  note send if true

					note_midi[cue_counter]=midi_cue[counterb];
					note_midi[(cue_counter)+1]=midi_cue[counterb+1];
					note_midi[(cue_counter)+2]=midi_cue[counterb+2];
					 midi_cue[counterb]=0;   // clear after note off only when used, too quick

					cue_counter=cue_counter+3;


				}}
			memcpy(test_byte,note_midi,9);
			note_midi[50]=cue_counter;

			nrpn_temp[80]=0;   //disable nrpn for now

				//memcpy(cue_temp,midi_cue_noteoff,25);
				cue_counter=0;
				for (i=0;i<sound_set;i++){  // short , ready to send notes off only
					counterb=i*3;

					if (midi_cue_noteoff[counterb]){
					note_off_midi[cue_counter]=midi_cue_noteoff[counterb]-16;
					note_off_midi[(cue_counter)+1]=midi_cue_noteoff[counterb+1];
					note_off_midi[(cue_counter)+2]=0;
					midi_cue_noteoff[counterb]=0;  // clear when used
					//midi_cue_noteoff[counterb+1]=0;

					cue_counter=cue_counter+3;


					}}
				note_off_midi[50]=cue_counter;


				len1=note_off_midi[50];

					len=note_midi[50];
				//	if(pause) len=0;

			memcpy(send_temp,note_off_midi,len1); // adding  Note off first



			memcpy(send_temp+len1,note_midi,len);     // adding Note on

			 if (pause)  len=len1; else len=len+len1;     // add note off and note on

			 if (pause==2)   len=0;   // disable all after last send



			 serial_len=len;   // serial send length note off + note on



			 memcpy(serial_out, midi_extra_cue, midi_extra_cue[28]); // extra stuff sent , anything
			 // start of serial send
			 memcpy(serial_out + midi_extra_cue[28], send_temp, serial_len);
			 serial_len = serial_len + midi_extra_cue[28];
			 midi_extra_cue[28] = 0;  // reset
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


		//	uint8_t button_exception1=square_buttons_list[((seq_step_mod ) & 31)]; // 0-40

			uint8_t scene_select=scene_buttons[0];
			uint8_t button_exception1=loop_note_list[scene_select];
			uint8_t button_colour=0;
			//uint8_t loop_length=loop_screen_last_note[scene_select];
			//uint8_t divider;


			/*  if (loop_selector)
			  {
				  switch(loop_length>>2){

				  case 0:  divider=7;break;
				  case 3:divider=15;break;
				  case 7:divider=31;break;
				  default:divider=31;break;
				  }

				  button_exception1=seq_step_mod; // green moving button
				 }*/


			if (record) button_colour=4; else button_colour=1;

		all_update=1;

		if((all_update==1)){   //  test and update all square scene lights on main screen

			  for (i=0;i<32;i++)  {
				  uint8_t alt_list=square_buttons_list[i];
				  uint8_t display_button=button_states[alt_list]; // just holds normal 0-32 number value


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

			  for (i=0;i<8;i++)  {  //selection buttons

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
	} // end of cdc_len




//send all if possible , after each step midi notes first  // might change
			//HAL_UART_Transmit(&huart1,serial_out,serial_len,100); // uart send
//			if (midi_cc) midi_cue[50]= midi_cue[50]- midi_cc_list[12];   // remove cc
//			midi_cc_list[12]=0;
//			midi_cc=0;   // disable cc



		}


void nrpn_send(void){			 // all nrpn data goes here ,sends as well on serial



/*
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

*/


}

		void loop_lfo(void) {  // 0-64-0

			uint16_t temp_hold=0;
			uint8_t lfo_out[sound_set*3];
			memcpy(lfo_out,loop_lfo_out,sound_set*3);
			uint8_t mem;


			for (i=0;i<sound_set;i++){
				 mem=i+sound_set;


				if ((lfo_out[mem]==0) && (lfo_out[i]==63))	lfo_out[mem]=2;
				if (lfo_out[mem]==0) lfo_out[i]= (lfo_out[i]+1)&255;
				if ((lfo_out[mem]==2) && (lfo_out[i]==0))	lfo_out[mem]=0;
				if (lfo_out[mem]==2) lfo_out[i]= (lfo_out[i]-1)&255;

				temp_hold=((lfo_out[i]*looper_list[(i*4)+2])>>4);

						lfo_out[i+32]=temp_hold;




			}

			memcpy(loop_lfo_out,lfo_out,sound_set*3);



		}

