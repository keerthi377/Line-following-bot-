#include <QTRSensors.h>

#define NUM_SENSORS 8
#define NUM_SAMPLES 4

QTRSensors qtra;

uint16_t sensorValues[NUM_SENSORS];

void setup() {
  Serial.begin(115200);
  delay(500);
  qtra.setTypeAnalog();
  // Define sensor pins (GPIOs connected to QTR-8A outputs)
  qtra.setSensorPins((const uint8_t[]){36, 39, 34, 35, 32, 33, 25, 26}, NUM_SENSORS);
  qtra.setSamplesPerSensor(NUM_SAMPLES);

  // Calibrate sensors
  Serial.println("Calibrating...");
  for (uint16_t i = 0; i < 2000; i++) {
    qtra.calibrate();
    delay(20);
  }
  Serial.println("Calibration done.\n");

  // Print calibration data
  const uint16_t* minVal = qtra.calibrationOn.minimum;
  const uint16_t* maxVal = qtra.calibrationOn.maximum;

  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" min: ");
    Serial.print(minVal[i]);
    Serial.print(" | max: ");
    Serial.println(maxVal[i]);
  }
}

void loop() {
  uint16_t position = qtra.readLineBlack(sensorValues);

  Serial.print("Position: ");
  Serial.print(position);
  Serial.print(" | Values: ");
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();

  delay(250);
}
