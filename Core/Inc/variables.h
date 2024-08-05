uint8_t serial1_temp;
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
char print_out[10][3];
uint8_t send_all[128]; //scene sends
uint8_t send_buffer[34]={144,5,3,144,5,3,144,0,0}; // light off, light on , scene light off ,then midi
volatile uint16_t seq_pos;  // 16 bit  , 24/quater or 8/step  on 1/16th , sequencer clock
uint16_t s_temp;
uint16_t mtc_tick=0;  // incoming realtime clock counter
uint8_t seq_enable;  // start stop sequencer
uint8_t seq_step; // 0-32 steps
uint8_t realtime;
uint8_t message_cue[10];
uint8_t message_counter;// points to select byte in midi
uint16_t timer_value=512; // sets timer ccr  def is 1100 for now
uint16_t seq_tmp;
uint8_t cdc_buffer[12];  // receive buffer on sub , check usbd_cdc_if.c and h for more
uint32_t Len;
//  pot byte  are 176 , 48-55, 0-127
// square buttons byte   144 /128 ,   0-39,127,   top row is 32-39
//  round vertical  144/128,81-85,127
// play 91 . record 93 ,shift button 98
// round horizontal 64-71
// velocity  1 is default or green, 2= default or green blink, 5=is yellow, 6 =yellow blink , 4 =red blink, 3=red,
uint8_t square_buttons_list [33]= {32,33,34,35,36,37,38,39,24,25,26,27,28,29,30,31,16,17,18,19,20,21,22,23,8,9,10,11,12,13,14,15}; // just reads buttons in top.down order

uint8_t scene_buttons[10];  // scene select buttons , bottom square row , also [0] is last button pressed
uint8_t scene_memory[260];  // scene memory 8*32 stored in order , velocity msb 3 bits 7-5 , pitch 5 bits LSB 0-4
uint8_t button_states[100]={1,1,1,1,1,1,1,1} ; // storage for incoming data on button presses , channel 1, only for display
uint8_t scene_pitch[260]; // stores a pitch value per field 8*32 , stored in order
uint8_t scene_velocity[260]; // stores a pitch value per field 8*32
uint8_t pot_states[8]={64,64,64,64,64,64,64,64}; // stores pots 1-8 current state
uint8_t note_off_flag[3]; // use this to detect held buttons 0 is on off ,1 is last button detected
uint8_t all_update=1; // update all buttons from button_states , 40 for now
uint8_t keyboard[3];  // store keys
uint8_t drum_list[8] ={20,21,22,23,24,25,26,27}; // notes played for drum scenes
uint8_t midi_cue[26];  // data cue for midi max 8 notes [25] = message length
uint32_t sys_cnt[3];
// SPI stuff
uint8_t spi_send[10];
uint8_t status_reg[2];
uint8_t stop_toggle;
uint8_t flash_flag=0;
uint8_t flash_read_block2[260] ={1,1,1,1,1,1,1,0}; // this should clearif flash read works
uint8_t send_spi2[260];
uint8_t write_once; // allow only a single flash write for now
uint8_t test_data[32]={0,0,0,0,1,0,5,1,1,0,1,5,1,1,0,1,1,3,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
uint8_t spi_hold[260]={0,10,0,0};
uint8_t all_settings[100];  // store all extra settings:  transpose , pots

//notes
uint8_t scene_transpose[9];
uint8_t last_button;
uint8_t bar_looping; // loop 8 notes on cueent scene if enabled
uint8_t bar_count; // relates to looping
uint8_t scene_mute; // muting
uint8_t last_incoming;
