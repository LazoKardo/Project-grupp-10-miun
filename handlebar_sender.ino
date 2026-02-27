#include <SoftwareSerial.h>

// ===== Handlebar sender =====
// Reads controls on the handlebar and sends a compact packet over Bluetooth.
// Each button works as a toggle: one press = ON, next press = OFF.

// Bluetooth serial pins (Arduino side)
const int BT_RX_PIN = 10; // Arduino RX  <- BT TX
const int BT_TX_PIN = 11; // Arduino TX  -> BT RX (use voltage divider to BT RX)
SoftwareSerial bt(BT_RX_PIN, BT_TX_PIN);

// Inputs
const int THROTTLE_PIN = A0;   // Potentiometer / hall signal (0-1023)
const int BRAKE_PIN = 2;       // Brake button/switch to GND
const int IND_LEFT_PIN = 3;    // Left indicator button to GND
const int IND_RIGHT_PIN = 4;   // Right indicator button to GND

// Packet format: S,<thr>,<br>,<l>,<r>,<chk>\n
unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL_MS = 50;
const unsigned long DEBOUNCE_MS = 40;

bool brakeState = false;
bool leftState = false;
bool rightState = false;

bool lastBrakeReading = HIGH;
bool lastLeftReading = HIGH;
bool lastRightReading = HIGH;

unsigned long lastBrakeEdgeMs = 0;
unsigned long lastLeftEdgeMs = 0;
unsigned long lastRightEdgeMs = 0;

int clampByte(int value) {
  if (value < 0) return 0;
  if (value > 255) return 255;
  return value;
}

void updateToggle(int pin, bool &state, bool &lastReading, unsigned long &lastEdgeMs) {
  bool reading = digitalRead(pin);
  unsigned long now = millis();

  // Toggle only on falling edge (button press with INPUT_PULLUP).
  if (lastReading == HIGH && reading == LOW && (now - lastEdgeMs) >= DEBOUNCE_MS) {
    state = !state;
    lastEdgeMs = now;
  }

  lastReading = reading;
}

void setup() {
  pinMode(BRAKE_PIN, INPUT_PULLUP);
  pinMode(IND_LEFT_PIN, INPUT_PULLUP);
  pinMode(IND_RIGHT_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  bt.begin(9600); // Match your HC-05/HC-06 baud
}

void loop() {
  updateToggle(BRAKE_PIN, brakeState, lastBrakeReading, lastBrakeEdgeMs);
  updateToggle(IND_LEFT_PIN, leftState, lastLeftReading, lastLeftEdgeMs);
  updateToggle(IND_RIGHT_PIN, rightState, lastRightReading, lastRightEdgeMs);

  // Typical turn-signal behavior: enabling one side disables the other.
  if (leftState && rightState) {
    rightState = false;
  }

  if (millis() - lastSend < SEND_INTERVAL_MS) {
    return;
  }
  lastSend = millis();

  int rawThrottle = analogRead(THROTTLE_PIN);
  int throttle = map(rawThrottle, 0, 1023, 0, 255);
  throttle = clampByte(throttle);

  int brake = brakeState ? 1 : 0;
  int left = leftState ? 1 : 0;
  int right = rightState ? 1 : 0;

  int checksum = (throttle + brake + left + right) & 0xFF;

  bt.print("S,");
  bt.print(throttle);
  bt.print(',');
  bt.print(brake);
  bt.print(',');
  bt.print(left);
  bt.print(',');
  bt.print(right);
  bt.print(',');
  bt.print(checksum);
  bt.print('\n');

  // Optional debug over USB serial monitor
  Serial.print("TX -> throttle=");
  Serial.print(throttle);
  Serial.print(" brake=");
  Serial.print(brake);
  Serial.print(" left=");
  Serial.print(left);
  Serial.print(" right=");
  Serial.println(right);
}