#include <QTRSensors.h>
// QTR analog sensor pins (fixed to ESP32 analog-capable GPIOs)
QTRSensors qtra;
#define NUM_SENSORS 8
#define NUM_SAMPLES 4
uint16_t sensorValues[NUM_SENSORS];  // Array to store QTR sensor values

// Motor Driver Pins - Swapped to use working Motor B channel
#define AIN1 18  // Was BIN1 
#define AIN2 19  // Was BIN2
#define PWMA 13  // Was PWMB
#define BIN1 14  // Was AIN1
#define BIN2 27  // Was AIN2
#define PWMB 12  // Was PWMA
#define STBY 15

// PD Constants
const float KP = 0.15;   // Increased for better response
const float KD = 0.05;  // Increased proportionally
int lastError = 0;
const int GOAL = 3500; // Center of line
const int MAX_SPEED = 60; // Increased slightly

void setup() {
  Serial.begin(115200);
  delay(500);
  
  // Motor control pin setup
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH); // Enable motor driver
  
  qtra.setTypeAnalog();
  // Define sensor pins (GPIOs connected to QTR-8A outputs)
  qtra.setSensorPins((const uint8_t[]){36, 39, 34, 35, 32, 33, 25, 26}, NUM_SENSORS);
  qtra.setSamplesPerSensor(NUM_SAMPLES);
  
  calibrateLineSensor();
}

void loop() {
  // First, read the sensors
  qtra.read(sensorValues);  // <-- this just fills the array
  
  // Debug: Print sensor values
  Serial.print("Sensors: ");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  
  // Try readLineBlack first - change to readLineWhite if avoiding line
  uint16_t position = qtra.readLineBlack(sensorValues);
  Serial.print("Position: ");
  Serial.print(position);
  
  int error = position - GOAL; // SWAPPED: position - GOAL instead of GOAL - position
  Serial.print(" Error: ");
  Serial.println(error);
  
  int motorSpeed = KP * error + KD * (error - lastError);
  lastError = error;
  
  int leftSpeed = MAX_SPEED + motorSpeed;  // SWAPPED: - instead of +
  int rightSpeed = MAX_SPEED - motorSpeed; // SWAPPED: + instead of -
  
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);
  
  Serial.print(" Left: "); Serial.print(leftSpeed);
  Serial.print(" Right: "); Serial.println(rightSpeed);
  
  setMotor(leftSpeed, rightSpeed);
  
  delay(100);  // Add delay to read serial output
}

void setMotor(int left, int right) {
  // Left Motor - Direction reversed
  digitalWrite(AIN1, left < 0);    // Swapped logic
  digitalWrite(AIN2, left >= 0);   // Swapped logic
  analogWrite(PWMA, abs(left));
  
  // Right Motor - Direction reversed
  digitalWrite(BIN1, right < 0);   // Swapped logic
  digitalWrite(BIN2, right >= 0);  // Swapped logic
  analogWrite(PWMB, abs(right));
}

void calibrateLineSensor() {
  delay(1000);
  Serial.println("Calibrating QTR...");
  Serial.println("Move the sensor over white and black areas for 10 seconds...");
  
  for (int i = 0; i < 400; i++) {
    qtra.calibrate();
    delay(25);
    
    // Print progress
    if (i % 40 == 0) {
      Serial.print(".");
    }
  }
  
  Serial.println();
  Serial.println("Calibration done!");
  
  // Print calibration values for debugging
  Serial.println("Calibrated minimum values:");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(qtra.calibrationOn.minimum[i]);
    Serial.print(" ");
  }
  Serial.println();
  
  Serial.println("Calibrated maximum values:");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(qtra.calibrationOn.maximum[i]);
    Serial.print(" ");
  }
  Serial.println();
}
