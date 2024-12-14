
// see:
// https://github.com/salvadorrueda/SerialMP3Player/blob/1.0.0/src/SerialMP3Player.cpp
// https://github.com/cefaloide/ArduinoSerialMP3Player/blob/master/ArduinoSerialMP3Player/ArduinoSerialMP3Player.ino

#include "SerialMP3Player.h"

#define RX 10
#define TX 11
#define MOTION_DETECTOR 14 // A0

SerialMP3Player mp3(RX,TX);
uint8_t answer[10] = {0};
int fileCount = 0;
const long RESEND_INTERVAL = 3000;
const int PIR_INIT_TIME = 5; // 60 seconds to init PIR motion sensor

void setup() {
  randomSeed(analogRead(0));
  mp3.showDebug(1);
  Serial.begin(9600);
  mp3.begin(9600);
  delay(500);

  pinMode(MOTION_DETECTOR, INPUT);

  mp3.sendCommand(CMD_SEL_DEV, 0, 2);
  delay(500);

  fileCount = getFileCount();
  Serial.println("File count: " + (String) fileCount);

  waitForPirToInit();
}

void loop() {
  int motionDetector = digitalRead(MOTION_DETECTOR);
  // Serial.println("DETECTOR:" + (String)motionDetector);
  if (motionDetector == HIGH) {
    // if (!isPlaying()) {
      Serial.println("Motion detected, so requesting to play file");
      playRandomFile();
      delay(10000);
    // } else {
      // Serial.println("Motion detected, but file is already playing");
    // }
  } else {
    Serial.println("No motion detected at " + (String) millis());
  }
  delay(100);
}

void playRandomFile(void) {
  int whichFile = random(fileCount);
  // if (!isPlaying()) {
    Serial.println("Sending request to play file #" + (String) whichFile);
    mp3.play(whichFile);
  // } else {
    // Serial.println("Currently playing");
  // }
}

int getFileCount(void) {
  mp3.sendCommand(CMD_QUERY_TOT_TRACKS);
  // delay(100);
  int fc = getFileCountFromAnswer();
  unsigned long start = millis();
  while (fc == -1) {
    if ((millis() - start) >= RESEND_INTERVAL) {
      Serial.println("Answer not received, so resending request for file count");
      mp3.sendCommand(CMD_QUERY_TOT_TRACKS);
      // delay(100);
    }
    fc = getFileCountFromAnswer();
    // Serial.println("Is it hanging? " + (String) millis());
    // delay(100);
  }
  return fc;
}

int getFileCountFromAnswer(void) {
  readMp3Answer();
  if ((answer[0] == 0x7E) && (answer[3] == 0x48) && (answer[9] == 0xEF)) {
    return answer[6];
  } else {
    return -1;
  }
}

bool isPlaying(void) {
  mp3.sendCommand(CMD_QUERY_STATUS);
  // delay(100);
  int status = isPlayingFromAnswer();
  unsigned long start = millis();
  while (status == -1) {
    if ((millis() - start) >= RESEND_INTERVAL) {
      Serial.println("Answer not received, so resending request for status");
      mp3.sendCommand(CMD_QUERY_STATUS);
      // delay(100);
    }
    status = isPlayingFromAnswer();
    // Serial.println("Is it hanging? " + (String) millis());
    // delay(100);
  }
  if (status == 1) {
    return true;
  } else {
    return false;
  }
}
int isPlayingFromAnswer(void) {
  readMp3Answer();
  if ((answer[0] == 0x7E) && (answer[3] == 0x42) && (answer[9] == 0xEF)) {
    if (answer[6] == 0x00) {
      return 0; // stopped
    } else if (answer[6] == 0x01) {
      return 1; // playing
    } else if (answer[6] == 0x02) {
      return 2; // paused
    } else {
      return -2; // unknown
    }
  } else {
    return -1;
  }
}

void readMp3Answer(void) {
  answer[10] = {0};
  uint8_t i = 0;
  while (mp3.available() && (i < 10)) {
    uint8_t b = mp3.read();
    answer[i] = b;
    i++;
  }
  return;
}

void waitForPirToInit(void) {
  for (int i=0; i<PIR_INIT_TIME; i++) {
    Serial.println("Initializing PIR sensor: " + (String) (PIR_INIT_TIME - i));
    delay(1000);
  }
  Serial.println("PIR sensor initialized for " + (String) PIR_INIT_TIME + " seconds");
}
