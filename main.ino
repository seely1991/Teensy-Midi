#include <Bounce.h>
const int NUM_OF_BUTTONS = 3;

const int MIDI_CHAN = 1;

const int DEBOUNCE_TIME = 5;

Bounce buttons[NUM_OF_BUTTONS] = {
  Bounce (0, DEBOUNCE_TIME),
  Bounce (1, DEBOUNCE_TIME),
  Bounce (2, DEBOUNCE_TIME)
  };

const int MIDI_NOTE_VALS[NUM_OF_BUTTONS] = {60, 62, 64};
  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < NUM_OF_BUTTONS; i++) {
    pinMode (i, INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < NUM_OF_BUTTONS; i++){
      buttons[i].update();
    }
  if (buttons[0].fallingEdge() && buttons[1].fallingEdge()){
      Serial.print("between 0 and 1");
      usbMIDI.sendNoteOff (MIDI_NOTE_VALS[0], 0, MIDI_CHAN);
      usbMIDI.sendNoteOff (MIDI_NOTE_VALS[1], 0, MIDI_CHAN);
      usbMIDI.sendNoteOn (61, 127, MIDI_CHAN);
    }
  else if (buttons[1].fallingEdge() && buttons[2].fallingEdge()){
      Serial.print("between 1 and 2");
      usbMIDI.sendNoteOff (MIDI_NOTE_VALS[0], 0, MIDI_CHAN);
      usbMIDI.sendNoteOff (MIDI_NOTE_VALS[1], 0, MIDI_CHAN);
      usbMIDI.sendNoteOn (63, 127, MIDI_CHAN);
    }
  for (int i = 0; i< NUM_OF_BUTTONS; i++){
    if (buttons[i].fallingEdge()){
      Serial.print(i);
        usbMIDI.sendNoteOn (MIDI_NOTE_VALS[i], 127, MIDI_CHAN);
      }else if (buttons[i].risingEdge()){
          usbMIDI.sendNoteOff (MIDI_NOTE_VALS[i], 0, MIDI_CHAN);
          usbMIDI.sendNoteOff (61, 0, MIDI_CHAN);
          usbMIDI.sendNoteOff (63, 0, MIDI_CHAN);
          delay(25);
        }
    }
}
