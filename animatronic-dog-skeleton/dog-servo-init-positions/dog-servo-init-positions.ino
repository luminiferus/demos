#include <Servo.h>

Servo neckServo;
Servo jawServo;
const int neckServoPin = 9;
const int jawServoPin = 10;
const int neckLeft = 0;
const int neckStraight = 90;
const int neckRight = 180;
const int jawStart = 0;
const int jawEnd = 90;

void closeJaw() {
  Serial.println("Moving jaw to closed position");
  jawServo.write(jawStart);
}

void openJaw() {
  Serial.println("Moving jaw to open position");
  // int jawEnd = jawStart + degrees;
  jawServo.write(jawEnd);
}

void moveNeckStraight() {
  Serial.println("Moving neck to straight position");
  neckServo.write(neckStraight);
}

void moveNeckLeft() {
  Serial.println("Moving neck to left position");
  // int neckEnd = neckStart + degrees;
  neckServo.write(neckLeft);
}

void moveNeckRight() {
  Serial.println("Moving neck to right position");
  // int neckEnd = neckStart + degrees;
  neckServo.write(neckRight);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing neck servo");
  neckServo.attach(neckServoPin);
  Serial.println("Initializing jaw servo");
  jawServo.attach(jawServoPin);
  Serial.println("Done with servo initialization");
  closeJaw();
  moveNeckStraight();
}

void loop() {
  // delay(1000);
  // openJaw();
  // moveNeckLeft();
  // delay(1000);
  // closeJaw();
  // moveNeckStraight();
  // delay(1000);
  // moveNeckRight();
}
