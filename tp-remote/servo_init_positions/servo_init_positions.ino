#include <Servo.h>

Servo lServo;
Servo rServo;
const int lServoPin = 9;
const int rServoPin = 10;
const int lStart = 0;
const int rStart = 170;
const int degrees = 80;

void offPosition() {
  Serial.println("Moving servos to off position");
  rServo.write(rStart);
  lServo.write(lStart);
}

void onPosition() {
  Serial.println("Moving servos to on position");
  int lEnd = lStart + degrees;
  int rEnd = rStart - degrees;
  lServo.write(lEnd);
  rServo.write(rEnd);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing left servo");
  lServo.attach(lServoPin);
  Serial.println("Initializing right servo");
  rServo.attach(rServoPin);
  Serial.println("Done with servo initialization");
  offPosition();
}

void loop() {
  delay(1000);
  onPosition();
  delay(1000);
  offPosition();
}
