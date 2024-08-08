




void midi_send(void){  // only for midi music no info return

		uint8_t cue_counter=0;
		uint8_t velocity=0;
		uint8_t seq_step_mod=seq_step;
		uint8_t scene=scene_buttons[0];

		for (i=0;i<4;i++){    // drums

			if((bar_looping) && (i==(bar_looping-1)) && loop_selector<2 )  seq_step_mod=((seq_step&7)+(pot_states[3]>>2))&31; else seq_step_mod=seq_step_list[i];  // enables looping on particular scene
						if((bar_looping) && (i==(bar_looping-1)) && loop_selector>1 )  seq_step_mod=((seq_step&7)+((loop_selector-2)*8))&31;



			if (scene_memory[seq_step_mod+(i*32)]>>5){
				midi_cue[cue_counter*3]=146;  // channel 3
				midi_cue[(cue_counter*3)+1]=drum_list[i];  // or pitch info
				velocity=(scene_velocity[seq_step_mod+(i*32)])&127;   // use only 3 bit msb
				if (!velocity) velocity=64; //missing velocity info still
				if(mute_list[i]) velocity=0;
				//if (scene_mute & ((button_states[i])==3)) velocity=0;
				if ((scene_solo) & (scene!=i)) velocity=0;   // mute everything but solo
				midi_cue[(cue_counter*3)+2]=velocity;
				cue_counter++;
			}
		}
		for (i=4;i<8;i++){   // notes


			if((bar_looping) && (i==(bar_looping-1)) && loop_selector<2 )  seq_step_mod=((seq_step&7)+(pot_states[3]>>2))&31; else seq_step_mod=seq_step_list[i];  // enables looping on particular scene
			if((bar_looping) && (i==(bar_looping-1)) && loop_selector>1 )  seq_step_mod=((seq_step&7)+((loop_selector-2)*8))&31;


			if (scene_memory[seq_step_mod+(i*32)]>>5){


						midi_cue[cue_counter*3]=147+(i-4);  // channel 4
				midi_cue[(cue_counter*3)+1]=((scene_pitch[seq_step_mod+(i*32)])+pot_tracking[(seq_step>>3)+((i)*4)])& 127;;  //  pitch info ,pot tracking  ?
				//midi_cue[(cue_counter*3)+1]=((scene_pitch[seq_step_mod+(i*32)])+scene_transpose[i])& 127;;  //  pitch info ,pot tracking  ?

				velocity=(scene_velocity[seq_step_mod+(i*32)])&127;   // use only 3 bit msb
				if (!velocity) velocity=127; //missing velocity info still
				if(mute_list[i]) velocity=0;
				//if ((scene_mute) & ((button_states[i])==3)) velocity=0;
				if ((scene_solo) & (scene!=i)) velocity=0;   // mute everything but solo

				midi_cue[(cue_counter*3)+2]=velocity;
				cue_counter++;
			}


		if (keyboard[0]&&(seq_step_mod&1)&&(scene>3))  {cue_counter++;midi_cue[cue_counter*3]=143+scene;
		midi_cue[(cue_counter*3)+1]=(keyboard[0])+pot_tracking[(seq_step_mod>>3)+((scene)*4)];midi_cue[(cue_counter*3)+2]=127;}  // keyboard midi send with transpose

			midi_cue[50]=cue_counter*3;


		}


	}



	void note_off(void) {      // way off here
		uint8_t len1=midi_cue[50];
		//uint8_t send_temp3[180];
	//	if(((seq_pos&7)==4) && (note_off_enable)) {  // note off  section




			//memcpy(send_temp3,midi_cue_noteoff,len1);
			for (i=0;i<8;i++){

				midi_cue_noteoff[i*3] = midi_cue[i*3];
				midi_cue_noteoff[(i*3)+1] = (midi_cue[(i*3)+1])&127;
				midi_cue_noteoff[(i*3)+2] = 0;

			}






	//	if (len1)	CDC_Transmit_FS(send_temp3, len1); //send all if possible , after each step midi notes first  // might change
		//	note_off_enable=0;
			midi_cue_noteoff[50]=len1;


		}






