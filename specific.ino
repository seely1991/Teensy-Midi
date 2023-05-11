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
int note_end_time[5];
  
int duration = 200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode (i, INPUT_PULLUP);
  };
};

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 5; i++){
    if (note_end_time[i] && millis() > note_end_time[i]){
      usbMIDI.sendNoteOff(MIDI_NOTE_VALS[i],0,MIDI_CHAN);
      button_state[i] = 0;
      note_end_time[i] = 0;
    };
    if (i < 3){
      buttons[i].update();
    };
  };
  
  for (int i = 0; i< 3; i++){
    if (buttons[i].fallingEdge()){
      if (button_state[0] && button_state[1]){
        button_state[0], button_state[1] = 0;
        button_state[3] = 1;
        note_end_time[3] = millis() + duration;
        usbMIDI.sendNoteOn(MIDI_NOTE_VALS[3],127,MIDI_CHAN);
      }else if (button_state[1] && button_state[2]){
        button_state[1], button_state[2] = 0;
        button_state[4] = 1;
        note_end_time[4] = millis() + duration;
        usbMIDI.sendNoteOn(MIDI_NOTE_VALS[4],127,MIDI_CHAN);
      }else{ 
        // normal button press
        button_state[i] = 1;
      };
    }else if (buttons[i].risingEdge() && button_state[i]){
      button_state[i] = 0;
      usbMIDI.sendNoteOn(MIDI_NOTE_VALS[i],127,MIDI_CHAN);
      note_end_time[i] = millis() + duration;
    };
    delay(25);
  };
};
