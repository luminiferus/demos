#include <Servo.h>

Servo lServo;
Servo rServo;

const int lServoPin = 9;
const int rServoPin = 10;
const int remotePin = 12;
const int ledPin = 13;

const int lStart = 0;
const int rStart = 170;
const int degrees = 80;

int oldRemoteVal = HIGH;

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
  pinMode(remotePin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.println("Initializing left servo");
  lServo.attach(lServoPin);
  Serial.println("Initializing right servo");
  rServo.attach(rServoPin);
  Serial.println("Done with servo initialization");
  offPosition();
}

void loop() {
  int remoteVal = digitalRead(remotePin);
  Serial.print("remote value:");
  Serial.println(remoteVal);
  delay(1000);
  if (remoteVal != oldRemoteVal) {
    if (remoteVal == LOW) {
      digitalWrite(ledPin, HIGH);
      onPosition();
    } else {
      digitalWrite(ledPin, LOW);
      offPosition();
    }
    oldRemoteVal = remoteVal;
  }
}
