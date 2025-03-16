Akai APC key 25 sequencer project with STM32F411+flash , uses usb cdc to talk to pc then a serial to midi converter handles the rest (including lighting the buttons) also midi out on serial   .
Basic parts working 32 steps 8 parts pitch and velocity added using pot 1 and 2  , pot 3 is transpose , pot 4 is for looping which is enabled with up button. Right arrow moves notes 1 step in the sequence.  Drum part 0-3 on channel 3 then 4-8 is channel 4-8 for notes.  Saves everything when pressing Send then reloads on reset. Left arrow is looping. Pressing volume and wiggling pot 1, inputs velocity in real time ,to selected part.
Mute and solo works , saves mute on restart. Press keyboard ,turn pot 2 for velocity , press buttons to enter notes. 
// ignore 




Pot 3 = Transpose, disabled 

Pot 5-8 = crossfade  velocity between 1-2,3-4 etc  , disabled

device + shift =     pot8=temp , pot7= midi channel,
send = write to flash , 
send + shift = reload 
solo = solo current selected part only , 
mute = select any part to mute, 
up arrow = enable sounds 8-15, 
pot5=note start offset fine  8/note  ,pot6=offset velocities in time to move accent, pot 7=lfo gain on time offset  ,  pot8=velocity/accent   
shift + pot 5 = set part currently playing  length 0-15 
keyboard = sets pitch for programming notes , also play 
Stop all clips = stops and jumps to start 
clip_stop = enables all pots to set pitch for 8x2 notes  except drums

shift+pot 1 =program change 

  
pan = shows current loaded patch  , pot 8 to set load/save position 
 
pattern select = row 3 and 4 ,1-16
press shift and pattern  (turns red) to play  then return to previous pattern  

shift pitch and note for keys by pattern using pot6 