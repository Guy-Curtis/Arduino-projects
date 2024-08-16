/*
 * This Arduino sketch is designed for an indoor plant irrigation system that uses a soil moisture sensor
 * and an ultrasonic sensor to protect a water pump. The system includes an LCD display to 
 * show the status of the water pump and soil moisture levels.
 *
 * Components:
 * - Arduino Uno
 * - 16x2 LCD display (I2C)
 * - Soil moisture sensor
 * - AJ-SR04M ultrasonic sensor
 * - 5v Relay module
 * - 5v Water pump
 *
 * Functionality:
 * - The soil moisture sensor measures the moisture level in the soil.
 * - The ultrasonic sensor measures the water level in the tank.
 * - The relay controls the water pump based on the soil moisture level and water level.
 * - The LCD displays the status of the water pump and soil moisture levels.
 * - The water pump will not turn on if the water level is below the set threshold
 * 
 * Author: Guy Curtis
 * Date: 16/08/2024
 */

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 7;
const int echoPin = 6;
const int relayPin = 2;
const int moistureSensorPin = A0;
long duration;
int distance;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("IRRIGATION");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM IS ON ");
  delay(3000);
  lcd.clear();
}

void loop() {
  // Measure water level
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Convert to cm

  // Read soil moisture
  int moistureValue = analogRead(moistureSensorPin);
  Serial.print("Moisture: ");
  Serial.println(moistureValue);
  Serial.print("Water Level: ");
  Serial.println(distance);

  // Check water level and soil moisture
  if (distance < 30) { // Adjusted threshold (cm)
    digitalWrite(relayPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Water Level LOW ");
  } else if (moistureValue > 950) {
    digitalWrite(relayPin, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Water Pump is ON ");
  } else {
    digitalWrite(relayPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Water Pump is OFF");
  }

  // Display moisture level
  if (moistureValue < 300) {
    lcd.setCursor(0, 1);
    lcd.print("Moisture : HIGH");
  } else if (moistureValue > 300 && moistureValue < 950) {
    lcd.setCursor(0, 1);
    lcd.print("Moisture : MID ");
  } else if (moistureValue > 950) {
    lcd.setCursor(0, 1);
    lcd.print("Moisture : LOW ");
  }

  delay(1000); // Wait for a second before the next loop
}
