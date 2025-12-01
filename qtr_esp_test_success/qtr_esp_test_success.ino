// QTR-8A Analog Sensor Test with ESP32

// Define analog input pins connected to QTR-8A outputs
const int sensorPins[8] = {36, 39, 34, 35, 32, 33, 25, 26};

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("QTR-8A Sensor Test");
}

void loop() {
  Serial.print("Raw values: ");
  for (int i = 0; i < 8; i++) {
    int sensorValue = analogRead(sensorPins[i]);
    Serial.print(sensorValue);
    Serial.print("\t");
  }
  Serial.println();
  delay(300); // Adjust delay for sampling rate
}
