#include <SoftwareSerial.h>

// Bluetooth module pins on handlebar Arduino
// Arduino RX <- Bluetooth TX
// Arduino TX -> Bluetooth RX
SoftwareSerial BT(10, 11);  // RX, TX

const int leftButtonPin = 2;
const int rightButtonPin = 3;

char lastSent = 'X';  // stores last message sent

void setup() {
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);

  BT.begin(9600);
}

void loop() {
  bool leftPressed = (digitalRead(leftButtonPin) == LOW);
  bool rightPressed = (digitalRead(rightButtonPin) == LOW);

  char messageToSend = 'N';

  if (leftPressed && !rightPressed) {
    messageToSend = 'L';
  } 
  else if (rightPressed && !leftPressed) {
    messageToSend = 'R';
  } 
  else {
    messageToSend = 'N';
  }

  // Only send if state changed
  if (messageToSend != lastSent) {
    BT.write(messageToSend);
    lastSent = messageToSend;
  }

  delay(50);  // small debounce delay
}