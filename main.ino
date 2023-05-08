#include <Bounce.h>
const int NUM_OF_BUTTONS = 3;

const int NUM_OF_MIDI = (NUM_OF_BUTTONS * 2) - 1;

const int MIDI_CHAN = 1;

const int DEBOUNCE_TIME = 5;

Bounce buttons[3];

int MIDI_NOTE_VALS[NUM_OF_MIDI];

for (int i = 0; i < NUM_OF_BUTTONS; i++){
  buttons[i] = Bounce (i, DEBOUNCE_TIME);
  MIDI_NOTE_VALS[(2*i)] = 60 + 2*i;
  MIDI_NOTE_VALS[(2*i)+1] = 60 + (2*i)+1;
};

bool button_state[NUM_OF_MIDI];
  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < NUM_OF_BUTTONS; i++) {
    pinMode (i, INPUT_PULLUP);
  };
};

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < NUM_OF_BUTTONS; i++){
      buttons[i].update();
    }
  
  for (int i = 0; i< NUM_OF_BUTTONS; i++){
    
    if (buttons[i].fallingEdge()){
      // the button above is also pressed
      if(button_state[i+1])){
        usbMIDI.sendNoteOn(MIDI_NOTE_VALS[NUM_OF_BUTTONS + i], 127, MIDI_CHAN);
        button_state[NUM_OF_BUTTONS + i] = 1;
        // turn off button above
        usbMIDI.sendNoteOff(MIDI_NOTE_VALS[i+1],127,MIDI_CHAN);
        button_state[i+1] = 0;
      // the button below is also pressed
      }else if ((i > 0 && button_state[i-1]){
        usbMIDI.sendNoteOn(MIDI_NOTE_VALS[NUM_OF_BUTTONS + i - 1], 127, MIDI_CHAN);
        button_state[NUM_OF_BUTTONS + i - 1] = 1;
        // turn off button below
        usbMIDI.sendNoteOff(MIDI_NOTE_VALS[i-1],127,MIDI_CHAN);
        button_state[i - 1] = 0;
      }else{
        // normal button press
        button_state[i] = 1;
        usbMIDI.sendNoteOn(MIDI_NOTE_VALS[i], 127, MIDI_CHAN);
      };
    
    }else if (buttons[i].risingEdge()){
      // check if either "diagonal" buttons are down
      if (button_state[NUM_OF_BUTTONS+i]){
        usbMIDI.sendNoteOff(MIDI_NOTE_VALS[NUM_OF_BUTTONS+i],0,MIDI_CHAN);
        button_state[NUM_OF_BUTTONS+i] = 0;
      }else if (i > 0 && button_state[NUM_OF_BUTTONS + i - 1]){
        usbMIDI.sendNoteOff(MIDI_NOTE_VALS[NUM_OF_BUTTONS+i-1],0,MIDI_CHAN);
        button_state[NUM_OF_BUTTONS+i-1] = 0;              
      }else{
        button_state[i] = 0;
        usbMIDI.sendNoteOff (MIDI_NOTE_VALS[i], 0, MIDI_CHAN);    
      }
      delay(25);
    }
  } 
}
