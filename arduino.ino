#include <ACS712XX.h>

// Pin Definitions
const int voltageSensorIn = A2;
const int currentSensorIn = A0;
const int coinCounterInPin = A1;

// Sensor Setup
ACS712XX ACS712(ACS712_30A, currentSensorIn);

// Global Variables
float voltageInput = 0.0;
float lastCounterReading = 0;
float counterReading = 0;

int coinCount = 0;

void setup() {
  Serial.begin(9600);

  // Calibrate the current sensor
  float _offset = ACS712.autoCalibrate();
}

void loop() {
  // --- Voltage Measurement ---
  int voltageSensorRaw = analogRead(voltageSensorIn);
  float voltageSensor = voltageSensorRaw * (25.0 / 1023.0); // Assuming 0-25V sensor range

  // --- Current Measurement ---
  float currentSensor = ACS712.getDC();

  // --- Coin Detection ---
  int counterValueRaw = analogRead(coinCounterInPin);
  counterReading = counterValueRaw * (5.0 / 1023.0);

  bool coinDetected = false;
  if (counterReading < lastCounterReading - 0.2) {
    coinCount += 1;
  }
  lastCounterReading = counterReading;

  if (Serial.available()) {
    String serialIn = Serial.readStringUntil('\n');
    if (serialIn == "reset") {
      coinCount = 0;
    }
  }

  // --- Serial Output ---
  Serial.print(voltageSensor);
  Serial.print(",");

  Serial.print(currentSensor);
  Serial.print(",");

  Serial.println(coinCount);

  delay(100);
}