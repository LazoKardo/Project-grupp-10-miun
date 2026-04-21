#include <SoftwareSerial.h>

// Bluetooth module pins on light Arduino
// Arduino RX <- Bluetooth TX
// Arduino TX -> Bluetooth RX
SoftwareSerial BT(10, 11);  // RX, TX

const int leftLedPin = 12;
const int rightLedPin = 13;

char receivedChar = 'N';

void setup() {
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);

  digitalWrite(leftLedPin, LOW);
  digitalWrite(rightLedPin, LOW);

  BT.begin(9600);
}

void loop() {
  if (BT.available() > 0) {
    receivedChar = BT.read();
  }

  if (receivedChar == 'L') {
    digitalWrite(leftLedPin, HIGH);
    digitalWrite(rightLedPin, LOW);
  }
  else if (receivedChar == 'R') {
    digitalWrite(leftLedPin, LOW);
    digitalWrite(rightLedPin, HIGH);
  }
  else {
    digitalWrite(leftLedPin, LOW);
    digitalWrite(rightLedPin, LOW);
  }
}