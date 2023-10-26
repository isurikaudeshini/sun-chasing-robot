#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 Lcd = Adafruit_SSD1306(128, 64, &Wire);

#define ledPin 8
#define sensorPin A3  // soil moisture sensor pin
#define uvSensorPin A0  // UV sensor pin
#define uvThreshold 1  
#define moistureThreshold 900  

// Motor A connections
int enA = 6;
int in1 = 7;
int in2 = 5;

// Motor B connections
int enB = 3;
int in3 = 4;
int in4 = 2;

int readSensor(int pin) {
  int sensorValue = analogRead(pin);
  return sensorValue;
}

void setup() {

  Serial.begin(9600);
  Lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {
  int moistureLevel = readSensor(sensorPin);
  int uvValue = readSensor(uvSensorPin);

  Serial.print("Soil Moisture: ");
  Serial.println(moistureLevel);
  Serial.print("UV: ");
  Serial.println(uvValue);

  Lcd.clearDisplay();
  Lcd.setTextColor(WHITE);

  Lcd.setTextSize(1);
  Lcd.setCursor(0, 0);

  Lcd.setTextSize(4);
  Lcd.setCursor(20, 22);
  Lcd.print("Plant Care...");
  
  delay(100);

  Lcd.display();

  //Lcd.dim(true);
  //delay(500);
  //Lcd.dim(false);
  //delay(500);

  if (moistureThreshold < moistureLevel) {
    // On LED
    digitalWrite(ledPin, HIGH);

    if (uvValue > uvThreshold) {
      moveRobotBackward();
      
    } else {
      stopRobot();
    }
  } else if (moistureThreshold > moistureLevel) {
    
    if (uvValue < uvThreshold) {
      {
        // Blink LED
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);

        moveRobotForward();
      }
        
      } else {
        stopRobot();
      }
    } else {
      stopRobot();
    }

    delay(1000);
}

void moveRobotForward() {
  analogWrite(enA, 175);
  analogWrite(enB, 175);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void moveRobotBackward() {
  analogWrite(enA, 150);
  analogWrite(enB, 150);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stopRobot() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

