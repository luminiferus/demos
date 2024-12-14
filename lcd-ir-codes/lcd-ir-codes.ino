#include <LiquidCrystal.h>
#include "IRremote.h"

int receiver = 10;
IRrecv irrecv(receiver);
decode_results results;

LiquidCrystal lcd(2,3,4,5,6,7);

void setup() {
  Serial.begin(9600);
  Serial.println("Display received IR Codes");

  irrecv.enableIRIn();

  lcd.begin(16, 2);
  lcd.print("IR Codes");
}

void loop() {
  lcd.setCursor(0, 1);
  if (irrecv.decode(&results)) {
    Serial.print("Code: ");
    Serial.println(results.value, HEX);
    Serial.println(results.value);

    String irCodeHex = String(results.value, HEX);
    if (irCodeHex.equals("ffffffff")) {
      // skip
    } else {
      lcd.print(irCodeHex);
    }
    irrecv.resume();
    delay(1000);
  }
}
