Akai APC key 25 sequencer project with STM32F411+flash , uses usb cdc to talk to pc then a serial to midi converter handles the rest (including lighting the buttons) also midi out on serial   .
 



row 1 and 2 =input notes 
row=3 setting pitch for 8 Presets per sound on first page of drums , sends nrpnto 
row 4= to  select a pattern 
row 5 = select sound

Pot 1 + Shift =program change only for non drum channels
Pot 4 = set musical scale type ,
pot5=LFO rate  sends nrpn only for filter
pot6= LFO level
pot 7=  
pot8=velocity/accent   

pause =  shift + clip_stop : clear program changes on drums 
select=enables program change 1-8 for drums saved when set to record every 8 notes , might change this 

Hold Shift to enter accented notes.
Down arrow = copy , not used
Left arrow= paste
Device + Shift =     pot8=temp , pot7= midi channel,
send = write to flash , 
send + shift = reload 
solo = solo uses top 4 pots to select between 4 sounds each 
mute = select any part to mute, turns red 
up arrow = select sounds 8-15, 
record = allows overwriting program change for now

keyboard = sets pitch for programming notes , also play 
Stop all clips = stops and jumps to start 
clip_stop = enables all pots to set pitch for 8x2 notes  except drums

  
pan = shows current loaded patch  , pot 8 to set load/save position 
 
 
press shift and pattern  (turns red) to play  then return to previous pattern  

shift pitch and note for keys by pattern using pot6 