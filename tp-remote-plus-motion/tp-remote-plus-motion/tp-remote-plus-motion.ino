/*
 LiquidCrystal_I2C library downloaded from:
 https://github.com/johnrickman/LiquidCrystal_I2C/archive/refs/tags/1.1.3.zip
 */
#include <LiquidCrystal_I2C.h>

#define SWITCH_REMOTE 2
#define SWITCH_MOTION 4
#define RGB_LED_R 3
#define RGB_LED_G 5
#define RGB_LED_B 6
#define MOTION_DETECTOR 14 // A0

const int PIR_INIT_TIME = 30; // 30 seconds to init PIR motion sensor

unsigned long twoSecTimer = 0;
bool showFirst = true;
bool motionArmed = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

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

void setup() {
  Serial.begin(9600);

  twoSecTimer = millis();

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
  unsigned long secondsSinceStart = millis() / 1000;

  Serial.print("Remote Mode: ");
  Serial.print(remoteMode);
  Serial.print(", Motion Mode: ");
  Serial.print(motionMode);
  Serial.print(", Motion Detector: ");
  Serial.print(motionDetector);
  Serial.print(", Seconds: ");
  Serial.println(secondsSinceStart);
  if (remoteMode == HIGH) {
    lcd.setCursor(0,0);
    lcd.print("TPBlaster Remote");
    lcd.setCursor(0, 1);
    if (flip()) {
      if (showFirst) {
        lcd.print("Press Remote    ");
        showFirst = false;
      } else {
        lcd.print("Button to Shoot ");
        showFirst = true;
      }
    }
    setLedToGreen();
  } else if (motionMode == HIGH) {
    lcd.setCursor(0,0);
    lcd.print("TPBlaster Motion");
    if (secondsSinceStart < PIR_INIT_TIME) {
      setLedToRed();
      // lcd.setCursor(14, 1);
      // lcd.print("  ");
      lcd.setCursor(0, 1);
      if ((PIR_INIT_TIME - secondsSinceStart) >= 10) {
        lcd.print("Initializing " + (String)(PIR_INIT_TIME - secondsSinceStart) + " ");
      } else {
        lcd.print("Initializing " + (String)(PIR_INIT_TIME - secondsSinceStart) + "  ");
      }
      Serial.println("Initializing");
      Serial.println(PIR_INIT_TIME - secondsSinceStart);
    } else {
      setLedToBlue();
      lcd.setCursor(0, 1);
      // lcd.print("Motion Ready    ");
      Serial.println("Motion Ready");
      if (flip()) {
        if (showFirst) {
          lcd.print("Press Remote    ");
          showFirst = false;
        } else {
          lcd.print("Button to Arm  ");
          showFirst = true;
        }
      }
    }
  } else {
    lcd.setCursor(0,0);
    lcd.print("Unknown Mode    ");
    lcd.setCursor(0,1);
    lcd.print("                ");
  }
  delay(100);
}

