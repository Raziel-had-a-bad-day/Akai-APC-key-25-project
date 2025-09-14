
uint8_t pattern_scale_process(uint8_t value ) {    // scale notes from list

	uint8_t note=0;
	uint8_t count=0;

	uint8_t octave=0;
	uint8_t note_countup=0;
	//count=value;
	value=value>>3; // 0-15


	while (count<value){

		note=pattern_scale_data[note_countup+(pattern_scale_list[pattern_select]*8)];  // pattern scale with selected scale
		//note=pattern_scale_data[note_countup];  // pattern scale with selected scale  , might skip this and do only during playback

	if ((note_countup) && (note==0)) {octave++;note_countup=0;}  else  note_countup=(note_countup+1)&7; // reset counter or count up


	count++;
	}

	return (note+(octave*12)+36); // starts from 36

}


void USBD_MIDI_DataInHandler(uint8_t *usb_rx_buffer, uint8_t usb_rx_buffer_length)   // first byte is extra info then normal midi bytes
{


	 while (usb_rx_buffer_length && *usb_rx_buffer != 0x00)
	  {
		// memcpy(test_byte,usb_rx_buffer,usb_rx_buffer_length);

		 memcpy(cdc_buffer,usb_rx_buffer+1,9);

		 memset(usb_rx_buffer,0,usb_rx_buffer_length);


	  }



}

void USB_send(void){    // send to midi controller, clean atm

	  uint8_t seq_step_mod=(seq_pos>>3)&31;
	  uint8_t send_temp[100];  // need this for temp lights
	  memcpy(send_temp,button_states,100);

	if (send_buffer_sent == 1) { // moving light send and button change , current

		seq_step_mem = seq_step_mod;
		send_buffer_sent = 2;
	}
	////  temporary lights , not stored
	if (record) send_temp[square_buttons_list[green_position[0]]]=3; else send_temp[square_buttons_list[green_position[0]]]=1; // add moving green light

	counter_a = 0; // clear
	i=0;
	while((!counter_a)&& (i<100))
	{ // test for all button changes
		if ((other_buttons_hold[i] != send_temp[i]) // this is ok
				&& (!counter_a)) {
			counter_a = i + 1;
			other_buttons_hold[i] = send_temp[i];
		}  // can be used to reset all these buttons
	i++;}

	if (counter_a) {

		send_buffer[6] = MIDI_NOTE_ON;
		send_buffer[7] = (counter_a - 1) & 127;
		send_buffer[8] = send_temp[counter_a - 1] & 127;

	}

	send_buffer[5]=9;
		// if ((cdc_len>2)   )
	  			// {  memcpy (send_temp+1,cdc_send_cue+(cdc_len-3),3); USBD_MIDI_SendReport(&hUsbDeviceFS,send_temp,4); cdc_len=cdc_len-3;}  else cdc_len=0;  // usb send
	if (send_buffer[6]) {USBD_MIDI_SendReport(&hUsbDeviceFS,send_buffer+5,4);send_buffer[6]=0;} // only data for controller
		}


void cdc_send(void){     // all midi runs often , need to separate  , will go back to the old way ,less confusing

	//UART_HandleTypeDef huart1;
			uint8_t len;  // note on
			uint8_t send_temp[256];
			uint8_t len1;  // note off
			uint8_t cue_counter;
			//uint16_t counterb;   // midi_cue position
			uint8_t note_midi [70] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // 3*16 ,   seems to get some garbage ?
			uint8_t nrpn_temp[16]={185,99,5,185,98,0,185,6,0};  // last byte is pitch
			uint8_t note_off_midi[50];
			//uint8_t cc_temp[22];
			//uint8_t seq_step_mod=seq_step_list[scene_buttons[0]]&31;

			uint8_t current_scene=scene_buttons[0];
			//uint8_t cue_counter2=0;
			//uint8_t nrpn_chl=185;
			//cc_temp[20]=0;
			//memcpy(cue_temp,midi_cue,25);
			cue_counter=0;

			i=0;
			// not super important but good for testing , below
			uint8_t current_velocity=64;
			uint8_t offset_pitch=seq_pos>>3; // 0-15
				//counterb=(pattern_select*512) +(offset_pitch*32) ; // pattern=512bytes or 16*32
				uint16_t drum_byte_select;   // selects a trigger 16 + (i*4) 16*64 ... 0-256
				uint8_t drum_byte;
				 uint16_t pattern=pattern_select;
				 uint8_t random_list[16]; // random notes for now
				 memcpy (random_list,alt_pots+(pattern*16),16); // load tones for current pattern
					//uint8_t pitch_seq=alt_pots[((seq_pos>>3)&7)+(pattern_select*16)]; // loops 0-7 ,steps
						uint8_t	pitch_seq=pattern_scale_process(random_list[(offset_pitch&7)]);  // change pitch from pots , maybe run always and ignore original pitch

						uint8_t button_states_temp[16]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
						uint8_t high_row_enable=0; // select second sounds scene ,default on

						uint8_t high_row=0;


						//memcpy(button_states_temp,button_states,8); // transfer bottom row data for blinky lights

						if (current_scene>7) high_row_enable=1;


				if ((seq_pos&7)==1) {    // fixed time for now , note generator
					uint8_t note_timing[16];
					uint8_t midi_channel_select=9;
					memcpy(note_timing,last_note_end_count,16);

					button_states_temp[current_scene]=5;  // selected sound yellow

		for (i = 0; i < sound_set; i++) { // transfer from midi_cue to note_midi to be sent

			midi_channel_select = midi_channel_list[i] & 15;
			if (note_timing[i] > 1)
				note_timing[i]--; //count down for notes ,seems to skip

			drum_byte_select = (offset_pitch >> 2) + (i * 4) + (pattern * drum_store); //based on time,  selects a trigger 16 + (i*4) 16*64 ... 0-256 (64bytes per pattern, 4 bytes per per )
			drum_byte = drum_store_one[drum_byte_select]; // this could be smaller

			if ((high_row_enable == 0) && (i < 8)) high_row = 1;
			else high_row = 0; // enable only on first half

			if ((high_row_enable) && (i > 7)) high_row = 1; // enable only on second half

			if ((note_timing[i] == 1) && (last_note_on_channel[i])) { // creates note off  for non drum sounds
				note_midi[cue_counter] = midi_channel_select + MIDI_NOTE_OFF; // note off
				note_midi[(cue_counter) + 1] = last_note_on_channel[i];
				note_midi[(cue_counter) + 2] = 0;
				cue_counter = cue_counter + 3;
				last_note_on_channel[i] = 0; // clear note
			}


			if (drum_byte & (1 << ((((offset_pitch) & 3) * 2)+1)) ) current_velocity=note_accent[i]; else current_velocity=96;   // get accent info

			if (mute_list[i] || pause) {current_velocity=0;} // mutes sound also sound button button goes dark

			if ((midi_channel_select == 9)&&(current_velocity)) {

				if (drum_byte & (1 << (((offset_pitch) & 3) * 2))) { // ok , drums
					if (high_row)
						button_states_temp[i] = 0;
					note_midi[cue_counter] = 153; // add note on
					note_midi[(cue_counter) + 1] = drum_list[i];
					note_midi[(cue_counter) + 2] = current_velocity;
					cue_counter = cue_counter + 3;
					last_note_on_channel[i] = drum_list[i];
					note_timing[i] = 4;

				}
			}
			if ((midi_channel_select != 9)&&(current_velocity)) {

				if (drum_byte & (1 << (((offset_pitch) & 3) * 2))) { // ok , notes
					if (high_row)
						button_states_temp[i] = 0; // button dark
					note_midi[cue_counter] = midi_channel_select + MIDI_NOTE_ON; // add note on
					note_midi[(cue_counter) + 1] = pitch_seq; // only first pitch set for now
					note_midi[(cue_counter) + 2] = current_velocity;
					cue_counter = cue_counter + 3;

					if (note_timing[i]>1 ){    // in case old note hasnt finished
						note_midi[cue_counter] =midi_channel_select+ MIDI_NOTE_OFF; // note off
						note_midi[(cue_counter) + 1] =last_note_on_channel[i];
						note_midi[(cue_counter) + 2] = 0;
						cue_counter = cue_counter + 3;

					}

					last_note_on_channel[i] = pitch_seq; // saves last pitch
					note_timing[i] = 4; // resets counter
				}
			}

			if (mute_list[i]) {button_states_temp[i] =3;} // show mute


			 if ((cue_counter + 3) >= 47) cue_counter=47;  // test for buffer size

					} //end of i count

					//if (cue_counter)	{memcpy(midi_cue_noteoff,note_midi,cue_counter); midi_cue_noteoff[49]=cue_counter;}   // save for note off


		//if (!pause)

		{if (high_row_enable )  memcpy(button_states,button_states_temp+8,8); else memcpy(button_states,button_states_temp,8);}   //copy back blinky lights


		memcpy(last_note_end_count,note_timing,16);
				} // end of note on generator , limited run

		// everything from here on runs on every seq_pos 8xseq_step

		if (pitch_change_flag){   // sends pitch nrpn section

					nrpn_temp[5]=((current_scene&7)*8)&127;  // select part pitch
					nrpn_temp[15]=9;// length of send
					nrpn_temp[8]=(pitch_list_for_drums[(pitch_selected_for_drums[current_scene&7])+(current_scene*8)])&127;  // sets pitch
					pitch_change_flag=0;


				}



		if (lfo_full_send_enable){   // sends filter nrpn section ,this needs to cycle through 16x

							uint8_t counterb=lfo_full_send_enable-1;

							nrpn_temp[5]=((counterb*8)+2)&127;  // select part filter
							nrpn_temp[15]=9; // length of send
							nrpn_temp[8]=loop_lfo_out[counterb+32] &127;  // sets filter
							if (lfo_full_send_enable<8) lfo_full_send_enable++; else lfo_full_send_enable=0; // cycle through until finish


						}


				//if(note_midi[0])  memcpy(test_byte, note_midi, 20);

			//memcpy(test_byte,note_midi,9);
			note_midi[50]=cue_counter;

			//nrpn_temp[15]=0;   //disable nrpn for now

				//memcpy(cue_temp,midi_cue_noteoff,25);
				cue_counter=0;

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
			 memcpy(serial_out + serial_len, nrpn_temp, nrpn_temp[15]); // temp only !  add nrpn
			 serial_len = serial_len + nrpn_temp[15];
			 nrpn_temp[15] = 0;
			 if (pause)
				 pause = 2;

					// empty
//					memcpy(serial_out+serial_len,cc_temp,cc_temp[20]);    // cc send
//
//
//
//					serial_len=serial_len+cc_temp[20];
					//cc_temp[20]=0;
			// end of serial send

			//  only for midi controller

// NRPN


		//	nrpn_cue[50]=0;
//

	if (cdc_len == 0) {	// only process after cdc send and only cdc send stuff , only  controller now

		len=0;
		all_update=0;

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
			int8_t lfo_out[sound_set*3];
			memcpy(lfo_out,loop_lfo_out,sound_set*3);
			uint8_t mem;
			uint8_t lfo_rate=1;


			for (i=0;i<sound_set;i++){
				 mem=i+sound_set;  // bytes 16-31 ,tracks lfo info
				 lfo_rate=lfo_settings_list[(i*2)];

				if ((lfo_out[mem]==0) && (lfo_out[i]>=63))	lfo_out[mem]=2;
				if (lfo_out[mem]==0) lfo_out[i]= (lfo_out[i]+lfo_rate);
				if ((lfo_out[mem]==2) && (lfo_out[i]<=0))	lfo_out[mem]=0;
				if (lfo_out[mem]==2) lfo_out[i]= (lfo_out[i]-lfo_rate);
				if (lfo_out[i]<0) lfo_out[i]=0;

				temp_hold=((lfo_out[i]*lfo_settings_list[(i*2)+1])>>6);   // level  64*lfo level

						lfo_out[i+32]=127-temp_hold;  // bytes 32-48 ,lfo output
			}

			memcpy(loop_lfo_out,lfo_out,sound_set*3);

		}

void midi_send_control(void){   // 16notes 1/8 res , 16 pattern , for one pattern only for now testing

			midi_send_time=(midi_send_time+1)&2047; // counts up every
			//if (!midi_send_time) midi_send_current=0; // reset counter when time on zero



		}

