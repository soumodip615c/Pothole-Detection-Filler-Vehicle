#include <Servo.h>

// Motor 1 pins
const int motor1Pin1 = 3;  // IN1 pin connected to pin 3 on Arduino
const int motor1Pin2 = 4;  // IN2 pin connected to pin 4 on Arduino

// Motor 2 pins
const int motor2Pin1 = 5;  // IN3 pin connected to pin 5 on Arduino
const int motor2Pin2 = 6;  // IN4 pin connected to pin 6 on Arduino

// Ultrasonic sensor pins
const int trigPin = 9;   // Trigger pin
const int echoPin = 10;  // Echo pin

// LED and Buzzer pins
const int ledPin = 13;    
const int buzzerPin = 12; 

// Servo
Servo myServo;           
const int servoPin = 7;  // Servo connected to pin 7

// Variables
long duration;   
float distance;  
bool servoActivated = false; // to avoid repeat triggering

void setup() {
  // Motors
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // LED + buzzer
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Servo
  myServo.attach(servoPin);
  myServo.write(0); // initial position

  // Serial for debugging
  Serial.begin(9600);
}

void loop() {
  // Measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0344 / 2; // cm

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If object is close (≤ 4.5 cm) → motors forward + buzzer + LED
  if (distance <= 4.5) {
    moveForward();
    digitalWrite(ledPin, LOW);    
    digitalWrite(buzzerPin, LOW); 
    Serial.println("Object detected, moving forward");

    servoActivated = false; // reset so servo can run again later
  } 
  else { // distance > 4.5 cm
    stopMotors();
    digitalWrite(ledPin, HIGH);    
    digitalWrite(buzzerPin, HIGH); 
    Serial.println("No object detected, stopping motors");
    servoActivated = true;

    // Run servo only once until reset
    if (!servoActivated) {
      myServo.write(90);   // Rotate servo to 90°
      delay(20000);        // Hold for 20 seconds
      myServo.write(0);    // Return to 0° initial position
      servoActivated = true;
    }
  }

  delay(100);
}

// ---------------- Functions ----------------
void moveBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}