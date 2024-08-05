Akai APC key 25 sequencer project with STM32F411+flash , uses usb cdc to talk to pc then a serial to midi converter handles the rest .
Basic parts working 32 steps 8 parts pitch and velocity added using pot 0 and 1  , pot 2 is trnaspose , pot 3 is for looping which is enabled with up button , down button 
enables muting part. Plays non stop for now. Drum part 0-3 on channel 3 then 4-8 is channel 4-8 for notes.  Saves everything when pressing Send then reloads on reset. 
