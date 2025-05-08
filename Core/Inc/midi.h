
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
	send_temp[square_buttons_list[green_position[0]]]=1; // add moving green light

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

		send_buffer[6] = 144;
		send_buffer[7] = (counter_a - 1) & 127;
		send_buffer[8] = send_temp[counter_a - 1] & 127;

	}

	send_buffer[5]=9;
		// if ((cdc_len>2)   )
	  			// {  memcpy (send_temp+1,cdc_send_cue+(cdc_len-3),3); USBD_MIDI_SendReport(&hUsbDeviceFS,send_temp,4); cdc_len=cdc_len-3;}  else cdc_len=0;  // usb send
	if (send_buffer[6]) {USBD_MIDI_SendReport(&hUsbDeviceFS,send_buffer+5,4);send_buffer[6]=0;} // only data for controller
		}


void midi_send(void){  // produces midi data from notes etc ,only for midi music no info return ,  runs 8x per step, rewrite to record time, location and size for entire sequence all patterns


		uint16_t velocity=0;
		//uint16_t seq_step_mod=seq_step_list[0];
		//uint8_t scene=scene_buttons[0]; // only for drums atm
		uint8_t random_list[256]={12,18,12,15,12,13,22,22,12,12,12,12,46,65,23,45}; // random notes for now
		memcpy (random_list,alt_pots,256); // 8+8 *16
		//memcpy (random_list+8,alt_pots,8);



		uint8_t looper_list_temp[sound_set*4];
		uint8_t lfo_out[sound_set];
		uint16_t loop_note_temp[sound_set];
		memcpy (looper_list_temp,looper_list,sound_set*4); // need this only for fine time offset, might use for other stuff
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
		 //uint16_t current_pos=seq_pos&127;   // +8 to 2048 , gotta reset not much point otherwise
		 uint16_t current_pos=midi_send_time; // only for counting up time during initial fill

		 uint16_t offset=0;   // time offset
		 uint8_t offset_pitch=0;
		 uint8_t offset_vel=0; // velocity offset compared to notes
		// uint8_t offset_lfo=0;
		 uint16_t offset2=0;
		 uint16_t pattern=pattern_select;
		 uint16_t cue_counter=midi_send_current*4; // current data position
		 uint16_t cue_counter2=midi_send_current; //also add to cue_counter




		for (i=0;i<sound_set;i++){    //  this really needs to go away


			//offset_lfo= lfo_out[i]; // 0-4
			if (retrigger_countdown[i]) retrigger_countdown[i]=retrigger_countdown[i]-1;// space out notes, always run, blocks playback for a minimum amount of time
			//offset2=((32-looper_list_temp[i*4])+offset_lfo)&31;  // base offset ,fine 1/8
			//offset2=((looper_list_temp[i*4])+offset_lfo)&31;  // base offset ,fine 1/8    , disabled as done elsewhere from now

			offset=(((128-offset2)+current_pos)&127)+(pattern*128);   // seq_pos+offset 0-255

			//i_s=i*32;
				//cue_counter=i*3; // always counting now
				//offset=current_pos;
				//if (!offset) loop_note_list[i]=31;  // reset to start , rough but works

			//if ((loop_screen_note_on[offset] & (1<<i)) && (!retrigger_countdown[i])) // remove this
				if ((7==(offset&7)) && (!retrigger_countdown[i]))  // triggers on 7 ?
			{note_enable=1;        // always triggered now

			if (loop_note_temp[i]>= loop_length_set[i]) loop_note_temp[i]=0; else loop_note_temp[i]++; } // works for notes only

						if ( (!mute_list[i])  	&&
								(note_enable)    // important , note on trigger now



						)     {    //  NOTE ON   ,disable if retrigger, plays from scene memory but only for note one



							loop_note_temp[i]=(current_pos&127)>>3; // force for now

							offset_pitch=loop_note_temp[i]; // note position, 0-16 this is ok


						//	midi_cue[cue_counter]=0; // start with mute just in case
						//	offset_vel=((looper_list_temp[(i*4)+1])+loop_note_temp[i])&15; // this has be on or only a single note repeats, disable for now
							//offset_vel=(pattern_offset_list[pattern_select]+loop_note_temp[i])&15;  // modify note position for keys
							offset_vel=loop_note_temp[i];


							if (midi_channel_list[i]!=9)offset_pitch=offset_vel; // only on keys
							//green_position[i]=offset_pitch; // set running green light position

							//if ((button_states_loop[data_temp2]!=1))   // keys

							uint16_t drum_byte_select= (offset_pitch>>2)+(i*4)+(pattern*drum_store);   // selects a trigger 16 + (i*4) 16*64 ... 0-256
							uint8_t drum_byte=drum_store_one[drum_byte_select];


							if (drum_byte & (1<<((offset_pitch&3)*2)))   // should only write on note


							{

								//retrigger_countdown[i]=3;    // start countdown to avoid fast note repeats

								midi_cue[cue_counter]=(midi_channel_list[i]&15)+144;  // get midi channel




							//if(mute_list[i]) midi_cue[cue_counter]=0; //send nothing // IMPORTANT OR WILL SEND GARBAGE //


							velocity=rand_velocities[((offset_pitch&15)+i)&31]&127;
							//velocity=127;

							if (midi_channel_list[i]==9)midi_cue[(cue_counter)+1]=drum_list[i];  // select note for drum sound

							else{
							//	midi_cue_noteoff[cue_counter]=midi_cue[cue_counter]; // note off channel
							//	midi_cue_noteoff[cue_counter+1]=last_note_played[i]; // note off


								midi_cue[(cue_counter)+1]=(random_list[(offset_vel&7)+(pattern*16)])&127;  // key pitch with offset using first 8 notes


							//	last_note_played[i]= midi_cue[(cue_counter)+1]; // save note on the same channel

							}
						//	if ((scene_solo) && (scene!=i)) velocity=0;   // mute everything but solo


						//	if (!velocity) midi_cue[cue_counter]=0;  // simply disable

							midi_cue[(cue_counter)+2]=velocity&127;

						//	if (midi_cue[cue_counter])

								midi_cue_time[cue_counter2]=current_pos;      // 0-2048 for now ,0-127*16
								midi_cue_size[cue_counter2]=4;					// data size
								midi_cue_loc[cue_counter2]=cue_counter;      // data location
								cue_counter=cue_counter+4; cue_counter2++;       // step up counters 0-8192



							} // end of drum byte trigger



							note_enable=0;
						}



		}   // end of loop


		midi_send_current=cue_counter2; // write back position
		midi_cue_count=cue_counter2;


		//midi_cue[50]=cue_counter; // total data

		memcpy(loop_note_list,loop_note_temp,sound_set*2); // counts up steps
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
			uint16_t counterb;
			uint8_t note_midi [70] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // seems to get some garbage ?
			uint8_t nrpn_temp[100];
			uint8_t note_off_midi[50];
			//uint8_t cc_temp[22];
			//uint8_t seq_step_mod=seq_step_list[scene_buttons[0]]&31;


			//uint8_t cue_counter2=0;
			//uint8_t nrpn_chl=185;
			//cc_temp[20]=0;
			//memcpy(cue_temp,midi_cue,25);



			cue_counter=0;

			uint16_t pattern_loc=pattern_select*128;
			uint16_t seq_step_mod=(seq_pos&127)+pattern_loc;   // 0-2047
			uint8_t pitch_seq=alt_pots[((seq_pos>>3)&7)+(pattern_select*16)]; // loops 0-7 ,steps
			pitch_seq=pattern_scale_process(pitch_seq);  // change pitch from pots , maybe run always and ignore original pitch


			i=0;
			// not super important but good for testing , below

			//for(i=0;i<midi_cue_count;i++){				// new data search for sending , checks by time record of the notes and adds when found , pitch data is not needed
				while (i<=midi_cue_count){
				//pattern_set=pattern_loc+i;


				if ((midi_cue_time[i]==seq_step_mod) && (midi_cue_time[i] ))	{		// linked to seq_pos for now
									counterb=midi_cue_loc[i];  // gets midi_cue pos

									note_midi[cue_counter]=midi_cue[counterb];
									//if (clip_stop && (note_midi[cue_counter]!=153)) note_midi[(cue_counter)+1]=pitch_seq; else  note_midi[(cue_counter)+1]=midi_cue[counterb+1];  // enables live pitch change, ignoring pitch data for now
									if (note_midi[cue_counter]!=153)	note_midi[(cue_counter)+1]=pitch_seq; else  note_midi[(cue_counter)+1]=midi_cue[counterb+1]; // only timing data , pitch only orig if drums otherwise all new

									note_midi[(cue_counter)+2]=midi_cue[counterb+2];
									//midi_cue[counterb]=0;   // clear after note off only when used, too quick

									cue_counter=cue_counter+4;



			} i++ ; }


			if(note_midi[0])  memcpy(test_byte, note_midi, 20);



			/*
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

			*/


			//memcpy(test_byte,note_midi,9);
			note_midi[50]=cue_counter;

			nrpn_temp[80]=0;   //disable nrpn for now

				//memcpy(cue_temp,midi_cue_noteoff,25);
				cue_counter=0;
			/*	for (i=0;i<sound_set;i++){  // short , ready to send notes off only
					counterb=i*3;

					if (midi_cue_noteoff[counterb]){
					note_off_midi[cue_counter]=midi_cue_noteoff[counterb]-16;
					note_off_midi[(cue_counter)+1]=midi_cue_noteoff[counterb+1];
					note_off_midi[(cue_counter)+2]=0;
					midi_cue_noteoff[counterb]=0;  // clear when used
					//midi_cue_noteoff[counterb+1]=0;

					cue_counter=cue_counter+3;
					}


					}
				*/
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
					//cc_temp[20]=0;
			// end of serial send

			//  only for midi controller

// NRPN


		//	nrpn_cue[50]=0;
//

	if (cdc_len == 0) {	// only process after cdc send and only cdc send stuff , only  controller now

		len=0;


		//	uint8_t button_exception1=square_buttons_list[((seq_step_mod ) & 31)]; // 0-40

			//uint8_t scene_select=scene_buttons[0];
			//uint8_t button_exception1=loop_note_list[scene_select];
			//uint8_t button_exception1=green_position[scene_select]; //disbled for now
			//uint8_t button_exception1=green_position[0];
			//uint8_t button_colour=0;
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


			//if (record) button_colour=4; else button_colour=1;

		all_update=0;

/*
	while ((all_update==1)){   //  test and update all square scene lights on main screen , this needs to change

			  i=0;
			  while (all_update==1)  {
				  uint8_t alt_list=square_buttons_list[i];
				  uint8_t display_button=button_states[alt_list]; // just holds normal 0-32 number value


			  if (((button_states_save[i]) != display_button) && (i!=button_exception1))  // send if changed
			  {
				  send_temp[len]=144;
				  send_temp[len+1]=alt_list;
				  send_temp[len+2]=display_button;  // only this part needs to change , all other normal
				 	len=len+3;
				  button_states_save[i] = display_button;
				  all_update=0;

			  }

			  if((i==button_exception1) && (button_states_save[i]!=button_colour) && all_update ) {   // green moving button , seems ok
			  send_temp[len]=144;
			  send_temp[len+1]=alt_list;
			  send_temp[len+2]=button_colour;
			  len=len+3;
			  button_states_save[i] =button_colour;
			  all_update=0;
			  }

			  i++;
			  } // end of i loop
			  i=0;
			  while (all_update==1)  { //sound selection buttons

			  				  uint8_t display_button=button_states[i]; // just holds normal 0-32 number value


			  			  if ((button_states_save[i+32] != display_button))  // send if changed
			  			  {
			  				  send_temp[len]=144;
			  				  send_temp[len+1]=i;
			  				  send_temp[len+2]=display_button;  // only this part needs to change , all other normal
			  				 	len=len+3;
			  				  button_states_save[i+32] = display_button;
			  				  all_update=0;
			  			  }
			  i++;}





		if (send_buffer[6]) {
			memcpy(send_temp + len, send_buffer + 6, 3); // extra buttons, ok
			len = len + 3;
			send_buffer[6] = 0;

		}
		all_update = 0;
		} // end of all update
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

void midi_send_control(void){   // 16notes 1/8 res , 16 pattern , for one pattern only for now testing

			midi_send_time=(midi_send_time+1)&2047; // temp for testing
			//if (!midi_send_time) midi_send_current=0; // reset counter when time on zero
			midi_send();


		}

void midi_cue_delete(uint8_t scene,uint8_t step,uint8_t pattern) {   // delete a certain time record on a certain channel ,works



	uint16_t pattern_loc=(((pattern&15)*128)+((step&15)*8))>>3 ;  // time of the step  , needs to check for all 0-7 ticks

	uint8_t midi_noteon=midi_channel_list[scene]+144;
	uint16_t midi_cue_set;

	for(i=0;i<midi_cue_count;i++){				// new data search for sending , checks by time record of the notes

		if (((midi_cue_time[i] >> 3) == pattern_loc)) {	// linked to seq_pos for now

			test_byte[8] = test_byte[8] + 1;
			midi_cue_set = midi_cue_loc[i];   // get start byte

			if (midi_cue[midi_cue_set] == midi_noteon) { // if midi command matches

			//	memcpy(test_byte, midi_cue + midi_cue_set, 4);

				memset(midi_cue + midi_cue_set, 0, 4);   // delete 4 bytes

				midi_cue_loc[i] = 0; // delete loc info
				midi_cue_time[i] = 0; // delete time record



		}}}}  // end of midi cue delete
void midi_cue_add(uint8_t scene,uint8_t step,uint8_t pattern) {  // does add to midi_cue work , time also correct , still missing notes

	uint16_t pattern_loc=(((pattern&15)*128)+((step&15)*8))+7 ;  // time  of the step
	uint8_t midi_noteon=midi_channel_list[scene]+144;
	uint8_t note=64;


	uint8_t velocity=127;
	uint16_t next_free=(midi_cue_count+1);  // next free pos
	uint16_t last_free_location=next_free*4;

	if (midi_noteon==153) note=drum_list[scene]; else note=alt_pots[step&7];  // only basic alt pots data


	midi_cue[last_free_location]=midi_noteon;
	midi_cue[last_free_location+1]=note;
	midi_cue[last_free_location+2]=velocity;
	midi_cue_loc[next_free] = last_free_location; // add loc
	midi_cue_time[next_free] = pattern_loc;		// add time



	//memcpy(test_byte, midi_cue +last_free_location, 4);

	midi_cue_count=next_free;  // add one




	}

