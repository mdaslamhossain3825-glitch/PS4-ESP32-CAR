#include <PS4Controller.h>
// RIGHT MOTOR
#define ENA 14
#define IN1 27
#define IN2 26
// LEFT MOTOR
#define ENB 25
#define IN3 33
#define IN4 32

int deadzone = 20;

void setup() {
  Serial.begin(115200);
  // Right motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  // Left motor pins
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  PS4.begin("3c:8a:1f:a0:1e:74");  // Your ESP32 MAC address
}

// Motor function with 2 arguments: leftSpeed and rightSpeed
void motor(int leftSpeed, int rightSpeed) {
  // Limit the speed values
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  // LEFT MOTOR
  if (leftSpeed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, leftSpeed);
  } else if (leftSpeed < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, -leftSpeed);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
  // RIGHT MOTOR
  if (rightSpeed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, rightSpeed);
  } else if (rightSpeed < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, -rightSpeed);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }
}
void loop() {
  if (PS4.isConnected()) {
    int forward = PS4.RStickY();  // forward/back
    int turn = PS4.RStickX();     // steering
    // Apply deadzone
    if (abs(forward) < deadzone) forward = 0;
    if (abs(turn) < deadzone) turn = 0;
    // Convert joystick values (-128 to 127) to motor speed (-255 to 255)
    int leftMotorSpeed = map(forward, -128, 127, 255, -255) - map(turn, -128, 127, -255, 255);
    int rightMotorSpeed = map(forward, -128, 127, 255, -255) + map(turn, -128, 127, -255, 255);
    leftMotorSpeed = constrain(leftMotorSpeed, -255, 255);
    rightMotorSpeed = constrain(rightMotorSpeed, -255, 255);
    // Drive the motors
    motor(leftMotorSpeed, rightMotorSpeed);
  } else {
    motor(0, 0);  // stop motors if controller disconnects
  }
}