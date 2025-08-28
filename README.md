Akai APC key 25 sequencer project with STM32F411+flash , uses usb cdc to talk to pc then a serial to midi converter handles the rest (including lighting the buttons) also midi out on serial   .
 
// ignore 




not super important:
Pot 2= set number of patterns playing in sequence

Pot 3 = Repeat pattern times

,,,,,

Middle horizontal row is program change for drums 1-8 , saved when set to record every 8 notes  
Pot 1 + Shift =program change only for non drum channels
Pot 4 = set musical scale type ,
Hold Shift to enter accented notes.
Down arrow = copy current part of the current pattern
Left arrow= paste
Device + Shift =     pot8=temp , pot7= midi channel,
send = write to flash , 
send + shift = reload 
solo = solo current selected part only , 
mute = select any part to mute, 
up arrow = select sounds 8-15, 
pot5=note start offset fine  8/note  ,pot6=offset velocities in time to move accent, pot 7=lfo gain on time offset  ,  pot8=velocity/accent   
shift + pot 5 = set part currently playing  length 0-15 / disabled
keyboard = sets pitch for programming notes , also play 
Stop all clips = stops and jumps to start 
clip_stop = enables all pots to set pitch for 8x2 notes  except drums



  
pan = shows current loaded patch  , pot 8 to set load/save position 
 
pattern select = row 3 and 4 ,1-16
press shift and pattern  (turns red) to play  then return to previous pattern  

shift pitch and note for keys by pattern using pot6 