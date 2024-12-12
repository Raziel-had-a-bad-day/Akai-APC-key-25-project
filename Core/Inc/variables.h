
#define cpu_clock    96000000
#define prescaler 3905    // TIM10 values

uint16_t bpm_table[256];  // lut values for temp TIM10 ARR
uint8_t tempo; // tempo value

//buttons
uint8_t volume; // volume button
uint8_t pan;
uint8_t shift; // track shift button
uint8_t pause; // enable pause mode
uint8_t select; //select button
uint8_t right_arrow;
uint8_t left_arrow;
uint8_t up_arrow;
uint8_t down_arrow;

uint8_t device;
uint8_t clip_stop;
uint8_t send;
uint8_t record;

uint8_t serial1_temp;
uint8_t temp;

uint8_t serial1_hold[10];
uint8_t serial1_hold2[10];
uint8_t serial1_counter;
uint16_t i;  // always 16 bit or higher
uint16_t n;
uint8_t led_blink;
uint8_t note_bank[18];   //contains note on and off info
uint8_t last_note=0; // 1-3  last note played on note bank
uint8_t last_key=0;  // last actual key played
uint8_t clear[3]={0,0,0};
uint8_t velocity=0;   // enable for velocity data byte
uint8_t note_temp[3]={0,0,0};   // holds current note being filled
uint8_t note_replace_enable=0;
uint8_t note_length_list[10];   // keeps note count for note off
uint8_t program; // current loaded program pointer
uint8_t patch_load; // patch to be loaded 0-15
uint8_t patch_save;  //  patch save  0-15
char print_out[10][3];
uint8_t send_all[128]; //scene sends
uint8_t send_buffer[34]={144,5,3,144,5,3,144,0,0}; // light off, light on , scene light off ,only for controller,then midi
volatile uint16_t seq_pos;  // 16 bit  , 24/quater or 8/step  on 1/16th , sequencer clock, rarely used might change
uint16_t s_temp;
uint16_t seq_pos_mem;
uint16_t mtc_tick=0;  // incoming realtime clock counter
uint8_t seq_enable=1;  // start stop sequencer
uint8_t seq_step; // 0-32 steps
uint8_t realtime;
uint8_t message_cue[10];
uint8_t message_counter;// points to select byte in midi
volatile uint16_t timer_value=512; // sets timer ccr  def is 1100 for now
uint16_t seq_tmp;
uint8_t cdc_buffer[12];  // receive buffer on sub , check usbd_cdc_if.c and h for more
uint8_t cdc_send_cue[260];   //hold from cdc
uint8_t cdc_len;
uint32_t Len;
volatile uint8_t button_send_trigger;  // send button midi repeat
uint8_t trigger_mem;
uint8_t lfo; // temp storage
//  pot byte  are 176 , 48-55, 0-127
// square buttons byte   144 /128 ,   0-39,127,   top row is 32-39
//  round vertical  144/128,81-85,127
// play 91 . stop all clip =81 ,record 93 ,shift button 98 ,82 clip stop
// round horizontal 64-71
// velocity  1 is default or green, 2= default or green blink, 5=is yellow, 6 =yellow blink , 4 =red blink, 3=red,
static uint8_t square_buttons_list [40]= {32,33,34,35,36,37,38,39,24,25,26,27,28,29,30,31,16,17,18,19,20,21,22,23,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7}; // just reads buttons in top.down order
static uint8_t button_convert[41]=		  {32,33,34,35,36,37,38,39,24,25,26,27,28,29,30,31,16,17,18,19,20,21,22,23,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7};
uint8_t scene_buttons[10];  // scene select buttons , bottom square row , also [0] is last button pressed
uint8_t scene_memory[260];  // scene memory 8*32 stored in order , velocity msb 3 bits 7-5 , pitch 5 bits LSB 0-4
uint8_t button_states[100]={1,1,1,1,1,1,1,1} ; // storage for incoming data on button presses , 8-40 (0-32)  is out of order to make it easier on operations
uint8_t scene_pitch[260]; // stores a pitch value per field 8*32 , stored in order
uint8_t scene_velocity[260]; // stores a pitch value per field 8*32
uint8_t pot_states[8]={64,64,64,64,64,64,64,64}; // stores pots 1-8 current state
uint8_t note_off_flag[3]; // use this to detect held buttons 0 is on off ,1 is last button detected
uint8_t all_update=1; // update all buttons from button_states , 40 for now
uint8_t keyboard[3];  // store keys
static uint8_t drum_list[8] ={20,21,22,23,24,25,26,27}; // notes played for drum scenes
uint8_t midi_cue[50];  // data cue for midi max 8 notes [25] = message length
uint8_t midi_cue_noteoff[50];  // data cue for midi max 8 notes [25] = message length
uint32_t sys_cnt[3];
uint8_t note_off_enable;

uint8_t counter_a;
// SPI stuff
uint8_t spi_send[10];
uint8_t status_reg[2];


uint8_t flash_flag=0;
uint8_t flash_read_block2[260] ={1,1,1,1,1,1,1,0}; // this should clearif flash read works
uint8_t send_spi2[260];
uint8_t write_once; // allow only a single flash write for now
uint8_t test_data[32]={0,0,0,0,1,0,5,1,1,0,1,5,1,1,0,1,1,3,0,1,1,0,1,0,1,0,1,0,1,0,1};
uint8_t spi_hold[260]={0,10,0,0};
uint8_t all_settings[200];  // store all extra settings:  transpose , pots
uint8_t other_buttons; // update control button lights
uint8_t other_buttons_hold[20]; // keeps track of buttons
uint8_t send_buffer_sent;
uint8_t button_states_save[100]; // reference for button changes for controller
uint8_t button_states_loop[256];  // stored,loop screen buttons ,  pitch+accent (MSB),always on , 1 for empty
uint8_t  loop_screen_note_on[256];    //changed , 1 bit per note on , simply stores trigger info per 256 count for loop, always triggers on a step
uint16_t loop_screen_note_off[256]; 		// calculated, note off record also 2048 count
uint16_t loop_note_list[10]; //tracks playback note
uint8_t button_states_main[64]; // button states on main screen , copied
uint8_t loop_note_count[10];  // keeps track of number of notes in a loop
uint8_t loop_current_speed;
uint8_t loop_lfo_out[30];  // used for some level of lfo using pot7 for now 0-255

uint8_t pitch_hold[10]; //holds last not eplayed
uint8_t seq_step_mem;  // mem for looper

uint8_t pot_tracking[33] ; // record pot movements , maybe after 1 bar ,only transpose for now
uint8_t mute_list[9]; //track scene mutes
uint8_t noteoff_list[25]; //track note offs sorted 0-7 , empty when not in use
//notes
uint8_t scene_transpose[9];
uint8_t scene_volume[9];  // use it to control velocity only on midi out
uint8_t last_button;
uint8_t bar_looping; // loop 8 notes on cueent scene if enabled
uint8_t bar_count; // relates to looping
uint8_t scene_mute; // muting
uint8_t last_incoming;
uint8_t scene_solo; //enable solo mode
uint8_t stop_toggle=0; // use it for pause
uint8_t loop_selector;  //enables loop controls pot 4-8 when up arrow is on
uint8_t play_speed[20]={8,8,8,8,8,8,8,8,8,8,1,1,1,1,1,1,0};  // sets playback speed using seq_pos multiply 1/4 1/2 1/1 2/1 maybe 4/1 only notes for now ,also sets repeat bars 1-4 times
uint8_t speed_default[12]={8,4,2,1,8,4,2,1,0,0,0};     // play speed deafults   [8] hholds last value selected

uint8_t seq_step_list[20]; //store seq_step position per part  .for now just notes 4-8
uint16_t seq_step_fine[10];  // holds high count for seq_step_list   *8 res
uint8_t seq_current_step; // current position on selected from seq_step_list
uint8_t seq_step_reset[10];  // tracks when seq_step_list reset to start
uint8_t seq_step_long; // 32*32
uint8_t seq_step_enable[9]; // step change tracking
uint8_t loop_screen_scene;
uint8_t loop_screen_last_note[9]; //holds last enabled loop screen note
uint8_t note_latch[10]; // stays on when triggered
uint16_t note_latch_pos[10]; // holds pos of note latch

uint8_t serial_out[50];
uint8_t serial_len;
uint8_t midi_channel_list[21]={2,2,2,2,3,4,5,6 };   //holds midi channel settings 0=1 (midi channels 1-16)
uint8_t nrpn_cue[80]={186,99,5,186,98,16,186,6,32};  // stores message for nrpn on es1 only needs 1 initial c99=5  then only  2 bytes repeating  CC 98 =NRPN LSB and CC 6 =value , for now 9 bytes though  , initial normal 3 bytes then convrted to 9
uint8_t pitch_lut[127] ={0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,8,10,12,14,16,18,20,22,24,27,30,33,36,39,42,45,48,51,54,57,64,0,2,4,6,8,10,12,14,16,18,20,22,24,27,30,33,36,39,42,45,48,51,54,57,64,70,73,76,79,82,85,88,91,94,97,100,103,105,107,109,111,113,115,117,119,121,123,125,127,64,70,73,76,79,82,85,88,91,94,97,100,103,105,107,109,111,113,115,117,119,121,123,125,127};   // es1 pitch table  4 octaves
uint8_t es_filter[9]; // track es1 filters  old and new values  say 4+4
uint8_t es_filter_cue[20];   // hold filter data for nrpn
uint8_t midi_cc[9]; // enabled if sending midi cc
uint8_t midi_cc_cue[20];  //  0=part+1 ,1 is value
uint8_t midi_extra_cue[30] ; // extra stuff to be sent that ins't regular [28] is length
uint8_t current_midi; // holds selected channel
uint8_t note_accent[9]={127,127,127,127,127,127,127,127,127};  // stores accent data for tracks on/off

uint8_t play_list[257]={15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15};  // holds playback muting order 0-7,8-15,16-23,24-31  for 8x 4 bars  on all parts maybe the 4 bit LSB ,only needs 8 bytes per part, change to hold transpose
uint8_t play_screen=0;  // enable for secondary screen for muting setup
uint8_t play_position;  // track muting list 8*4     each 8 steps  +1
uint8_t play_list_write=0; // keeps writing while shift is held down
uint8_t write_velocity;  // keeps writing velocity while enable , holds velocity value as well
uint8_t looper_list[33]={0,32,0,8,0,32,0,8,0,32,0,8,0,32,0,8,0,32,0,8,0,32,0,8,0,32,0,8,0,32,0,8,0};  // holds looper settings , 0=start 2=length 3=gap between repeats 4=speed (default 0,32,0,8)
uint8_t loop_current_offset;
uint8_t loop_current_length;
uint8_t step_record; // works in stop mode


uint8_t test_byte[6];
