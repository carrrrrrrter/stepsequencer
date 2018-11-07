int ledPinArray[4] = {0, 1, 2, 3};
int buttonPinArray[4] = {4, 5, 6, 7};
int switchPin = 22;
int potPin = A9;
int midiArray[4] = {39, 39, 39, 39};
int currentStep = 0;
unsigned long lastStepTime = 0;
boolean lastButtonState[4] = {LOW, LOW, LOW, LOW};
boolean buttonState[4] = {LOW, LOW, LOW, LOW};
boolean on[4] = {false, false, false, false};

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPinArray[i], OUTPUT);
    pinMode(buttonPinArray[i], INPUT);
  }
  pinMode(switchPin, INPUT);
}

void loop() {
  sequence();
  checkButtons();
  setLeds();
}

void sequence() {
  int tempo = analogRead(A9);

  if (millis() > lastStepTime + tempo) {

    //previous step off
    digitalWrite(ledPinArray[currentStep], LOW);

    //increment
    if (digitalRead(switchPin) == LOW) {
      incrementUp();
    }
    else {
      incrementDown();
    }
    lastStepTime = millis();
    
    digitalWrite(ledPinArray[currentStep], HIGH);
    if (on[currentStep] == true) {
      usbMIDI.sendNoteOff(midiArray[currentStep], 0, 1);
      usbMIDI.sendNoteOn(midiArray[currentStep], 127, 1);
    }
  }
}

void checkButtons() {
  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPinArray[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (on[i] == false)
        on[i] = true;
      else if (on[i] == true)
        on[i] = false;
    }
  }
}

void setLeds() {
  for (int i = 0; i < 4; i++) {
    if (on[i] == true) {
      digitalWrite(ledPinArray[i], HIGH);
      digitalWrite(ledPinArray[currentStep], HIGH);

    }
    else if (on[i] == false) {
      digitalWrite(ledPinArray[i], LOW);
      digitalWrite(ledPinArray[currentStep], HIGH);
    }
  }
}

void incrementUp() {
  currentStep++;
  if (currentStep > 3)
    currentStep = 0;
}

void incrementDown() {
  currentStep--;
  if (currentStep < 0)
    currentStep = 3;
}
