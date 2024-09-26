/*
 LiquidCrystal_I2C library downloaded from:
 https://github.com/johnrickman/LiquidCrystal_I2C/archive/refs/tags/1.1.3.zip
 */
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define SWITCH_REMOTE 2
#define SWITCH_MOTION 4
#define RGB_LED_R 3
#define RGB_LED_G 5
#define RGB_LED_B 6
#define L_SERVO 9
#define R_SERVO 10
#define REMOTE_SENSOR 12
#define MOTION_DETECTOR 14 // A0

const int PIR_INIT_TIME = 30; // 30 seconds to init PIR motion sensor
const int L_SERVO_START = 0;
const int R_SERVO_START = 170;
const int SERVO_DEGREES = 80;

unsigned long twoSecTimer = 0;
bool showFirst = true;

const int REMOTE_MODE = 1;
const int MOTION_MODE = 2;
const int UNKNOWN_MODE = 3;
int lastMode = 0;

bool doRemoteBlast = false;
int oldRemoteVal = HIGH;

bool doMotionBlast = false;
bool doMotionArm = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo lServo;
Servo rServo;

void setLedToRed() {
  digitalWrite(RGB_LED_R, HIGH);
  digitalWrite(RGB_LED_G, LOW);
  digitalWrite(RGB_LED_B, LOW);
}

void setLedToGreen() {
  digitalWrite(RGB_LED_R, LOW);
  digitalWrite(RGB_LED_G, HIGH);
  digitalWrite(RGB_LED_B, LOW);
}

void setLedToBlue() {
  digitalWrite(RGB_LED_R, LOW);
  digitalWrite(RGB_LED_G, LOW);
  digitalWrite(RGB_LED_B, HIGH);
}

void setLedToWhite() {
  digitalWrite(RGB_LED_R, HIGH);
  digitalWrite(RGB_LED_G, HIGH);
  digitalWrite(RGB_LED_B, HIGH);
}

void setLedToYellow() {
  digitalWrite(RGB_LED_R, HIGH);
  digitalWrite(RGB_LED_G, HIGH);
  digitalWrite(RGB_LED_B, LOW);
}

bool flip() {
  // Serial.println("Millis: " + (String) millis() + ", Timer:" + (String) twoSecTimer);
  if (millis() - twoSecTimer > 2000) {
    twoSecTimer = millis();
    return true;
  } else {
    return false;
  }
}

void blastOff() {
  Serial.println("Moving servos to off position");
  lServo.write(L_SERVO_START);
  rServo.write(R_SERVO_START);
}

void blastOn() {
  Serial.println("Moving servos to on position");
  int lEnd = L_SERVO_START + SERVO_DEGREES;
  int rEnd = R_SERVO_START - SERVO_DEGREES;
  lServo.write(lEnd);
  rServo.write(rEnd);
}

void setup() {
  Serial.begin(9600);

  twoSecTimer = millis();

  pinMode(REMOTE_SENSOR, INPUT);
  Serial.println("Initializing left servo");
  lServo.attach(L_SERVO);
  Serial.println("Initializing right servo");
  rServo.attach(R_SERVO);
  Serial.println("Done with servo initialization");
  blastOff();

  pinMode(SWITCH_REMOTE, INPUT_PULLUP);
  pinMode(SWITCH_MOTION, INPUT_PULLUP);

  pinMode(RGB_LED_R, OUTPUT);
  pinMode(RGB_LED_G, OUTPUT);
  pinMode(RGB_LED_B, OUTPUT);

  lcd.init();
  lcd.backlight();
}

void loop() {
  int remoteMode = digitalRead(SWITCH_REMOTE);
  int motionMode = digitalRead(SWITCH_MOTION);
  int motionDetector = digitalRead(MOTION_DETECTOR);
  int remoteSensor = digitalRead(REMOTE_SENSOR);
  unsigned long secondsSinceStart = millis() / 1000;

  Serial.print("Remote Mode: ");
  Serial.print(remoteMode);
  Serial.print(", Motion Mode: ");
  Serial.print(motionMode);
  Serial.print(", Motion Detector: ");
  Serial.print(motionDetector);
  Serial.print(", Remote: ");
  Serial.print(remoteSensor);
  Serial.print(", Seconds: ");
  Serial.println(secondsSinceStart);
  if (remoteMode == HIGH) {
    doMotionArm = false; // needed?
    doMotionBlast = false; // needed?
    if (lastMode == 0) {
      lastMode = REMOTE_MODE;
      doRemoteBlast = false;
      oldRemoteVal = remoteSensor;
    } else if ((lastMode == MOTION_MODE) || (lastMode == UNKNOWN_MODE)) {
      // just switched mode
      lastMode = REMOTE_MODE;
      doRemoteBlast = false;
      oldRemoteVal = remoteSensor;
    } else if (remoteSensor != oldRemoteVal) {
      if (doRemoteBlast == false) {
        doRemoteBlast = true;
      } else {
        doRemoteBlast = false;
      }
      oldRemoteVal = remoteSensor;
    }
    lcd.setCursor(0,0);
    lcd.print("TPBlaster Remote");
    lcd.setCursor(0, 1);
    if (doRemoteBlast == false) {
      setLedToGreen();
      if (flip()) {
        if (showFirst) {
          lcd.print("Press Remote    ");
          showFirst = false;
        } else {
          lcd.print("Button to Blast!");
          showFirst = true;
        }
      }
      blastOff();
    } else {
      setLedToWhite();
      lcd.print("Blasting!!!     ");
      blastOn();
    }

  } else if (motionMode == HIGH) {
    doRemoteBlast = false; // needed?
    lcd.setCursor(0,0);
    lcd.print("TPBlaster Motion");
    if (secondsSinceStart < PIR_INIT_TIME) {
      setLedToRed();
      lcd.setCursor(0, 1);
      if ((PIR_INIT_TIME - secondsSinceStart) >= 10) {
        lcd.print("Initializing " + (String)(PIR_INIT_TIME - secondsSinceStart) + " ");
      } else {
        lcd.print("Initializing " + (String)(PIR_INIT_TIME - secondsSinceStart) + "  ");
      }
      Serial.println("Initializing");
      Serial.println(PIR_INIT_TIME - secondsSinceStart);
    } else {

      if (lastMode == 0) {
        lastMode = MOTION_MODE;
        doMotionArm = false;
        doMotionBlast = false;
        oldRemoteVal = remoteSensor;
      } else if ((lastMode == REMOTE_MODE) || (lastMode == UNKNOWN_MODE)) {
        // just switched mode
        lastMode = MOTION_MODE;
        doMotionArm = false;
        doMotionBlast = false;
        oldRemoteVal = remoteSensor;
      } else if (remoteSensor != oldRemoteVal) {
        // Serial.println("**** CHECKING");
        if (doMotionArm == false) {
          // Serial.println("$$$$$ FALSE to TRUE");
          doMotionArm = true;
        } else {
          // Serial.println("????? TRUE to FALSE");
          doMotionArm = false;
        }
        oldRemoteVal = remoteSensor;
      }

      if ((doMotionArm == true) && (motionDetector == HIGH)) {
        setLedToWhite();
        lcd.setCursor(0, 1);
        lcd.print("Blasting!!!     ");
        Serial.println("Motion Blasting");
        blastOn();
        delay(7000);
        blastOff();
        doMotionArm = false;
      } else if (doMotionArm == true) {
        setLedToYellow();
        lcd.setCursor(0, 1);
        Serial.println("Motion Detection Armed");
        if (flip()) {
          if (showFirst) {
            lcd.print("Motion Detection");
            showFirst = false;
          } else {
            lcd.print("Armed and Ready!");
            showFirst = true;
          }
        }
      } else if (doMotionArm == false) {
        setLedToBlue();
        lcd.setCursor(0, 1);
        Serial.println("Motion Ready");
        if (flip()) {
          if (showFirst) {
            lcd.print("Press Remote    ");
            showFirst = false;
          } else {
            lcd.print("Button to Arm   ");
            showFirst = true;
          }
        }
      }
    }
  } else {
    if (lastMode == 0) {
      lastMode = UNKNOWN_MODE;
    }
    lcd.setCursor(0,0);
    lcd.print("Unknown Mode    ");
    lcd.setCursor(0,1);
    lcd.print("                ");
  }
  delay(100);
}

