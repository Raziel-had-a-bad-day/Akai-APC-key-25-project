




void midi_send(void){  // only for midi music no info return

		uint8_t cue_counter=0;
		uint16_t velocity=0;
		uint8_t seq_step_mod=seq_step;
		uint8_t scene=scene_buttons[0];
		uint8_t len;
		uint8_t cue_temp[50];

		for (i=0;i<4;i++){    // drums
			cue_counter=i*3;
			if((bar_looping) && (i==(bar_looping-1)) && loop_selector<2 )  seq_step_mod=((seq_step&7)+(pot_states[3]>>2))&31; else seq_step_mod=seq_step_list[i];  // enables looping on particular scene
						if((bar_looping) && (i==(bar_looping-1)) && loop_selector>1 )  seq_step_mod=((seq_step&7)+((loop_selector-2)*8))&31;



			if ((scene_memory[seq_step_mod+(i*32)]>>5) && (!mute_list[i]) )     {    // only on note on

				midi_cue[cue_counter]=midi_channel_list[i]+144;  // channel 3

				midi_cue[(cue_counter)+1]=drum_list[i];  // or pitch info
				velocity=(scene_velocity[seq_step_mod+(i*32)])&127;   // use only 3 bit msb
				if (!velocity) velocity=64; //missing velocity info still
				velocity= (velocity*scene_volume[i])>>7;

				if ((scene_solo) && (scene!=i)) velocity=0;   // mute everything but solo
				midi_cue[(cue_counter)+2]=velocity&127;
				//cue_counter++;
			} else midi_cue[cue_counter]=0;
		}
		for (i=4;i<8;i++){   // notes in order or empty 0-7

			cue_counter=i*3;
			if((bar_looping) && (i==(bar_looping-1)) && loop_selector<2 )  seq_step_mod=((seq_step&7)+(pot_states[3]>>2))&31; else seq_step_mod=seq_step_list[i];  // enables looping on particular scene
			if((bar_looping) && (i==(bar_looping-1)) && loop_selector>1 )  seq_step_mod=((seq_step&7)+((loop_selector-2)*8))&31;


			if ((scene_memory[seq_step_mod+(i*32)]>>5) && (!mute_list[i]))  {


						midi_cue[cue_counter]=144+midi_channel_list[i];  // channel 4
						if(mute_list[i]) midi_cue[cue_counter]=0;    //send nothing

						midi_cue[(cue_counter)+1]=((scene_pitch[seq_step_mod+(i*32)])+pot_tracking[(seq_step_mod>>3)+((i)*4)]+scene_transpose[i])& 127;;  //  pitch info ,pot tracking  ?


				velocity=(scene_velocity[seq_step_mod+(i*32)])&127;   // use only 3 bit msb
				if (!velocity) velocity=127; //missing velocity info still
				velocity= (velocity*scene_volume[i])>>7;

				if ((scene_solo) & (scene!=i)) velocity=0;   // mute everything but solo

				midi_cue[(cue_counter)+2]=velocity&127;
				//cue_counter++;
			} else  midi_cue[cue_counter]=0;


		if (keyboard[0]&&(seq_step_mod&1)&&(scene>3))  {cue_counter=cue_counter+3;midi_cue[cue_counter]=144+scene;
		midi_cue[(cue_counter)+1]=(keyboard[0])+pot_tracking[(seq_step_mod>>3)+((scene)*4)]+scene_transpose[i];midi_cue[(cue_counter)+2]=127;}  // keyboard midi send with transpose

			midi_cue[50]=cue_counter;


		}

		if (midi_cc){   // send cc info from here
			len=midi_cc_list[12];


			//for (i=0;i<len;i++){ cue_counter++;
			//midi_cue[cue_counter*3]=midi_cc;}

			memcpy(midi_cue+midi_cue[50],midi_cc_list,len);

			midi_cue[50]=midi_cue[50]+len;    // disable after note off or it gets reset


		}

	}



	void note_off(void) {      // way off here
		uint8_t len1=midi_cue[50];

		uint8_t counter=0;
		uint8_t counterb=0;
		//uint8_t send_temp3[180];
	//	if(((seq_pos&7)==4) && (note_off_enable)) {  // note off  section



		len1=len1-midi_cc_list[12]; //remove cc from list

		//midi_cc_list[12]=0;
	//	midi_cc=0;   // disable cc


			//memcpy(send_temp3,midi_cue_noteoff,len1);
			for (i=0;i<8;i++){

				counterb=i*3;

				if (midi_cue[counterb])   {    // only if new message otherwise leave alone

					midi_cue_noteoff[counterb] = noteoff_list[counterb];  // copy old to send out
					noteoff_list[counterb]= midi_cue[counterb];  // replace old with new for late



					midi_cue_noteoff[(counterb)+1] =noteoff_list[counterb+1];
					noteoff_list[(counterb)+1]= midi_cue[counterb+1];

					midi_cue_noteoff[(counterb)+2] =0;
					noteoff_list[(counterb)+2]= 0;

			} else    midi_cue_noteoff[counterb] =0;



			}
			//if ((seq_step==31 ) ) memcpy(midi_cue_noteoff,noteoff_list,24);   // flush everything at end ?  may be a little much






		}


void cdc_send(void){


			uint8_t len;
			uint8_t send_temp[256];
			uint8_t len1;
			uint8_t cue_counter;
			uint8_t counterb;
			uint8_t note_midi[50];
			uint8_t note_off_midi[50];


			//memcpy(cue_temp,midi_cue,25);



			cue_counter=0;
			for (i=0;i<8;i++){  // short , ready to send notes only
				counterb=i*3;

				if (midi_cue[counterb]){
					note_midi[cue_counter]=midi_cue[counterb];
					note_midi[(cue_counter)+1]=midi_cue[counterb+1];
					note_midi[(cue_counter)+2]=midi_cue[counterb+2];
					cue_counter=cue_counter+3;


				}}
				note_midi[50]=cue_counter;

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

			memcpy(send_temp,note_off_midi,len1); // from last send
			len=len+len1;
			serial_len=len;
			memcpy(send_temp+len1,note_midi,len);
			memcpy(send_temp+len,send_buffer,12);  // midi first then lights
			len=len+12;

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



			memcpy(serial_out,send_temp,serial_len);


			CDC_Transmit_FS(send_temp, len); //send all if possible , after each step midi notes first  // might change

			if (midi_cc) midi_cue[50]= midi_cue[50]- midi_cc_list[12];   // remove cc
			midi_cc_list[12]=0;
			midi_cc=0;   // disable cc



		}



