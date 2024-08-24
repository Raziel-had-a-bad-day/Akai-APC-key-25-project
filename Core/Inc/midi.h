




void midi_send(void){  // only for midi music no info return

		uint8_t cue_counter=0;
		uint16_t velocity=0;
		uint8_t seq_step_mod=seq_step;
		uint8_t scene=scene_buttons[0];
		 uint8_t data_temp2;
		 uint8_t retrigger=0; // enable if seq_step hasn't_ moved



		for (i=0;i<8;i++){    // drums
			cue_counter=i*3;
			data_temp2=(play_position>>2)+(i*8);


			seq_step_mod=seq_step_list[i]&31;
			if (seq_step_list[i]>>5) retrigger=1; else retrigger=0;



			if((bar_looping) && (i==(bar_looping-1)) && loop_selector<2 )  seq_step_mod=((seq_step&7)+(pot_states[3]>>2))&31; //else seq_step_mod=seq_step_list[i];  // enables looping on particular scene
						if((bar_looping) && (i==(bar_looping-1)) && loop_selector>1 )  seq_step_mod=((seq_step&7)+((loop_selector-2)*8))&31;



						if ((scene_memory[seq_step_mod+(i*32)]) && (!mute_list[i]) &&  (play_list[data_temp2 ]&(1<<(play_position&3)))	&& (!retrigger) )     {    // only on note on

				midi_cue[cue_counter]=midi_channel_list[i]+144;  // channel 3

					midi_cue[(cue_counter)+1]=((scene_pitch[seq_step_mod+(i*32)])+pot_tracking[(seq_step_mod>>3)+((i)*4)]+scene_transpose[i])& 127;;  //  pitch info ,pot tracking  ?

				if ((nrpn_cue[(cue_counter+1)])!=(midi_cue[(cue_counter)+1]))				{	nrpn_cue[cue_counter]=i+1; nrpn_cue[(cue_counter+1)]=midi_cue[(cue_counter)+1];  }// change nrpn value only if needed
					else 	nrpn_cue[cue_counter]=0;

					if (midi_channel_list[i]==9)midi_cue[(cue_counter)+1]=drum_list[i];  // use drum list if set to  channel 10

					if(mute_list[i]) midi_cue[cue_counter]=0; //send nothing // IMPORTANT OR WILL SEND GARBAGE //
					velocity=(scene_velocity[seq_step_mod+(i*32)])&127;   // use only 3 bit msb

				//if (velocity>=scene_volume[i]) velocity=velocity-scene_volume[i]; else velocity=0;  // simple cutoff notes below a level
			velocity= (velocity*scene_volume[i])>>7;


				if ((scene_solo) && (scene!=i)) velocity=0;   // mute everything but solo
				midi_cue[(cue_counter)+2]=velocity&127;
				//cue_counter++;
			} else {midi_cue[cue_counter]=0;nrpn_cue[cue_counter]=0;}


		//	if ((nrpn_cue[cue_counter]==0)  && (es_filter[i&3])!=(es_filter[(i&3)+4]))    	{	nrpn_cue[cue_counter]=i+16; nrpn_cue[(cue_counter+1)]=es_filter[(i&3)+4];  es_filter[i&3]=(es_filter[(i&3)+4]);}   // add filter value pref between notes

		}
//		for (i=4;i<8;i++){   // notes in order or empty 0-7
//
//			cue_counter=i*3;
//			data_temp2=(play_position&7)+(i*8);
//			if((bar_looping) && (i==(bar_looping-1)) && loop_selector<2 )  seq_step_mod=((seq_step&7)+(pot_states[3]>>2))&31; else seq_step_mod=seq_step_list[i];  // enables looping on particular scene
//			if((bar_looping) && (i==(bar_looping-1)) && loop_selector>1 )  seq_step_mod=((seq_step&7)+((loop_selector-2)*8))&31;
//
//
//			if ((scene_memory[seq_step_mod+(i*32)]) && (!mute_list[i]) &&  (play_list[data_temp2 ]&(1<<(play_position&3))))  {
//
//
//						midi_cue[cue_counter]=144+midi_channel_list[i];  // channel 4
//
//
//						midi_cue[(cue_counter)+1]=((scene_pitch[seq_step_mod+(i*32)])+pot_tracking[(seq_step_mod>>3)+((i)*4)]+scene_transpose[i])& 127;;  //  pitch info ,pot tracking  ?
//						if(mute_list[i]) midi_cue[cue_counter]=0;    //send nothing // IMPORTANT OR WILL SEND GARBAGE //
//
//				velocity=(scene_velocity[seq_step_mod+(i*32)])&127;   // use only 3 bit msb
//			//	if (!velocity) velocity=127; //missing velocity info still
//				velocity= (velocity*scene_volume[i])>>7;
//
//			//	if (velocity>=scene_volume[i]) velocity=velocity-scene_volume[i]; else velocity=0;  // simple cutoff notes below a level
//				//if (velocity<=scene_volume[i]) velocity=0;  // simple cutoff notes below a level
//				//velocity= (velocity-scene_volume[i])>>7;
//
//
//
//
//				if ((scene_solo) & (scene!=i)) velocity=0;   // mute everything but solo
//
//				midi_cue[(cue_counter)+2]=velocity&127;
//				//cue_counter++;
//			} else  midi_cue[cue_counter]=0;




			midi_cue[50]=cue_counter;


		}






	void note_off(void) {      // way off here



		uint8_t counterb=0;
		//uint8_t send_temp3[180];


			//memcpy(send_temp3,midi_cue_noteoff,len1);
			for (i=0;i<8;i++){

				counterb=i*3;

				if (midi_cue[counterb])   {    // only if new message otherwise leave alone , not enough

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


void cdc_send(void){

	UART_HandleTypeDef huart1;
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
					if (nrpn_cue[counterb] ){				// NRPN section
					nrpn_temp[cue_counter2] = nrpn_chl; // CC99  , ch 10
					nrpn_temp[cue_counter2+1] =99;
					nrpn_temp[cue_counter2+2] =5;
										nrpn_temp[cue_counter2+3] = nrpn_chl;  //CC98
										nrpn_temp[cue_counter2+4] =98;
							//		if 	(!es_filter_cue[0])

									{nrpn_temp[cue_counter2+5] =(nrpn_cue[counterb]-1)*8;   nrpn_temp[cue_counter2+8] =pitch_lut[nrpn_cue[counterb+1]&127]; } // data,   pitch translate


								//	else  {  nrpn_temp[cue_counter2+5] =((es_filter_cue[0]-1)*8)+2;  															// es filter
							//		nrpn_temp[cue_counter2+8] =es_filter_cue[ 1];
								//	es_filter_cue[0]=0;
									//}

									nrpn_temp[cue_counter2+6] = nrpn_chl;
														nrpn_temp[cue_counter2+7] =6;


															cue_counter2=cue_counter2+9;

				}

				if (midi_cc_cue[0]){       // send a single  cc

					cc_temp[0]=midi_cc_cue[0]; // channel cc
					cc_temp[1]=74;  // cutoff
					cc_temp[2]=midi_cc_cue[1];
					cc_temp[20]=3;
					midi_cc_cue[0]=0;

				}

				if (midi_cue[counterb]){

					note_midi[cue_counter]=midi_cue[counterb];
					note_midi[(cue_counter)+1]=midi_cue[counterb+1];
					note_midi[(cue_counter)+2]=midi_cue[counterb+2];
					cue_counter=cue_counter+3;


				}}

			note_midi[50]=cue_counter;
			nrpn_temp[80]=cue_counter2;  // can be a trimmed a lot


				//memcpy(cue_temp,midi_cue_noteoff,25);
				cue_counter=0;
				for (i=0;i<8;i++){  // short , ready to send notes off only
					counterb=i*3;

					if (midi_cue_noteoff[counterb]){
					note_off_midi[cue_counter]=midi_cue_noteoff[counterb];
					note_off_midi[(cue_counter)+1]=midi_cue_noteoff[counterb+1];
					note_off_midi[(cue_counter)+2]=midi_cue_noteoff[counterb+2];
						cue_counter=cue_counter+3;


					}}
				note_off_midi[50]=cue_counter;


				len1=note_off_midi[50];

					len=note_midi[50];
				//	if(pause) len=0;

			memcpy(send_temp,note_off_midi,len1); // adding  Note off



			memcpy(send_temp+len1,note_midi,len);     // adding Note on

			 if (pause)  len=len1; else len=len+len1;     // add note off and note on

					serial_len=len;




			if (send_buffer[9] )        {memcpy(send_temp+len,send_buffer,12);  len=len+12;}                   // adding send_buffer
			else {memcpy(send_temp+len,send_buffer,9);len=len+9;}


			memcpy(serial_out,midi_extra_cue,midi_extra_cue[28]);   // extra stuff sent , anything


			// start of serial send
			memcpy(serial_out+midi_extra_cue[28],send_temp,serial_len);
			serial_len=serial_len+midi_extra_cue[28];
					midi_extra_cue[28]=0;


					memcpy(serial_out+serial_len,nrpn_temp,nrpn_temp[80]);    // temp only !  add nrpn

					serial_len=serial_len+nrpn_temp[80];
					nrpn_temp[80]=0;  // empty
//					memcpy(serial_out+serial_len,cc_temp,cc_temp[20]);    // cc send
//
//
//
//					serial_len=serial_len+cc_temp[20];
					cc_temp[20]=0;
			// end of serial send

			//  only for midi controller
			if (all_update==2){  // send on note off

				memcpy(send_temp+len,send_all,120);
				len=len+120;
				all_update=0;

			}

			if (all_update==10){  // send on note off

							memcpy(send_temp+len,send_all,24);
							len=len+24;
							all_update=0;

						}

// NRPN


		//	nrpn_cue[50]=0;
//
			CDC_Transmit_FS(send_temp, len); //send all if possible , after each step midi notes first  // might change
			//HAL_UART_Transmit(&huart1,serial_out,serial_len,100); // uart send
//			if (midi_cc) midi_cue[50]= midi_cue[50]- midi_cc_list[12];   // remove cc
//			midi_cc_list[12]=0;
//			midi_cc=0;   // disable cc



		}



