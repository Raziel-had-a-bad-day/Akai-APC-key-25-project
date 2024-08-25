Akai APC key 25 sequencer project with STM32F411+flash , uses usb cdc to talk to pc then a serial to midi converter handles the rest (including lighting the buttons) also midi out on serial   .
Basic parts working 32 steps 8 parts pitch and velocity added using pot 1 and 2  , pot 3 is transpose , pot 4 is for looping which is enabled with up button. Right arrow moves notes 1 step in the sequence.  Drum part 0-3 on channel 3 then 4-8 is channel 4-8 for notes.  Saves everything when pressing Send then reloads on reset. Left arrow is looping. Pressing volume and wiggling pot 1, inputs velocity in real time ,to selected part.
Mute and solo works , saves mute on restart. Press keyboard ,turn pot 2 for velocity , press buttons to enter notes. 



Pot 1 = pitch, nrpn data 
Pot 2 = Velocity,
Pot 3 = Transpose, 
Pot 4 = loop start position while looping, 
Pot 5-8 = crossfade  velocity between 1-2,3-4 etc  , 
Shift+Pot 8 = tempo , select+pot1 or pot2  to enter pitch and velocity or press on keyboard while pressing buttons, 
device+select = midi settings for parts using bottom row part buttons and pot 8 of entry red light shows current midi channel, 
send = write to flash , 
send+shift = reload 
solo = solo current selected part only , 
mute = select any part to mute, 
up arrow = press to select looping current part 1=pot4 2-5=bar 1-4 . 
keyboard = transpose change on selected part  
Stop all clips = stops and jumps to start 
clip stop = a muting menu , mute very 8 notes 8*32 in total per part 
down arrow = cycles playback speed on part 
shift and pot2 = while holding shift will modify velocity values for current part 
pause = play keyboard notes with transpose  
pause +select = enter pitch for notes using keyboard 