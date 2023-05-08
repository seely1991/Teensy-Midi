#include <Bounce.h>

const int MIDI_CHAN = 1;

const int DEBOUNCE_TIME = 5;

Bounce buttons[3] = {
  buttons[0] = Bounce(0, DEBOUNCE_TIME),
  buttons[1] = Bounce(1, DEBOUNCE_TIME),
  buttons[2] = Bounce(2, DEBOUNCE_TIME)
};

int MIDI_NOTE_VALS[5] = {60, 61, 62, 63, 64};

bool button_state[5];
  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode (i, INPUT_PULLUP);
  };
};

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 3; i++){
      buttons[i].update();
    }
  
  for (int i = 0; i< 3; i++){
    if (buttons[i].fallingEdge()){
      // normal button press
      button_state[i] = 1;
      usbMIDI.sendNoteOn(MIDI_NOTE_VALS[i], 127, MIDI_CHAN);    
    }else if (buttons[i].risingEdge()){
      button_state[i] = 0;
      usbMIDI.sendNoteOff(MIDI_NOTE_VALS[i], 0, MIDI_CHAN);
      if (i < 2 && button_state[3]){
        button_state[3] = 0;
        usbMIDI.sendNoteOff(MIDI_NOT_VALS[3],0,MIDI_CHAN);
      };
      if (i > 0 && button_state[4]){
        button_state[4] = 0;
        usbMIDI.sendNoteOff(MIDI_NOT_VALS[4],0,MIDI_CHAN);
      };
    };
    delay(25);
  };
  
  if (button_state[0] && button_state[1]){
    button_state[3] = 1;
    usbMIDI.sendNoteOff(MIDI_NOTE_VALS[0], 0, MIDI_CHAN);
    usbMIDI.sendNoteOff(MIDI_NOTE_VALS[1], 0, MIDI_CHAN);
    usbMIDI.sendNoteOn(MIDI_NOT_VALS[3],127,MIDI_CHAN);
  };
  if (button_state[1] && button_state[2]){
    button_state[4] = 1;
    usbMIDI.sendNoteOff(MIDI_NOTE_VALS[1], 0, MIDI_CHAN);
    usbMIDI.sendNoteOff(MIDI_NOTE_VALS[2], 0, MIDI_CHAN);
    usbMIDI.sendNoteOn(MIDI_NOT_VALS[4],127,MIDI_CHAN);
  };  
};
