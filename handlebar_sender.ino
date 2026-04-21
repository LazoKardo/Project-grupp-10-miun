// Pin assignments
const int leftButtonPin = 2;
const int rightButtonPin = 3;

const int leftLedPin = 12;
const int rightLedPin = 13;

// Variables for button states
int leftButtonState = 0;
int rightButtonState = 0;

void setup() {
  // LEDs as outputs
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);

  // Buttons as inputs
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
}

void loop() {
  // Read buttons
  leftButtonState = digitalRead(leftButtonPin);
  rightButtonState = digitalRead(rightButtonPin);

  // Control left light
  if (leftButtonState == HIGH) {
    digitalWrite(leftLedPin, HIGH);
  } else {
    digitalWrite(leftLedPin, LOW);
  }

  // Control right light
  if (rightButtonState == HIGH) {
    digitalWrite(rightLedPin, HIGH);
  } else {
    digitalWrite(rightLedPin, LOW);
  }
}