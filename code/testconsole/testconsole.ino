#include <Rotary.h>
#include <ezButton.h>
#include "ST7920_SPI.h"
#include <SPI.h>
#include "c64enh_font.h"

//uses https://github.com/brianlow/Rotary
//uses ST7920_SPI library
//uses ezbutton


//14(CIPO), 15(SCK), 16(COPI)

//below are micro pins
#define LCD_CLK 15
#define LCD_DATA 16
#define LCD_CS 17

#define ROT_CLK 6
#define ROT_SD 5
#define ROT_BTN 3

#define BTN2  4
#define BUZZER 2
#define SCR_CNTRL 4
#define FAN_CNTRL 3


Rotary r1 = Rotary(ROT_CLK, ROT_SD);
ezButton rb1(ROT_BTN);
ezButton b2(BTN2);
ST7920_SPI lcd(LCD_CS);

void setup(void) {
  Serial.begin(115200);
  r1.begin(true);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  SPI.begin();
  lcd.init();
  lcd.cls();
  lcd.setFont(c64enh);
  lcd.printStr(ALIGN_CENTER, 28, "Hello World!");
  lcd.drawRect(18, 20, 127 - 18 * 2, 63 - 20 * 2, 1);
  lcd.display(0);
  delay(100);
}

void loop(void) {
  //  u8g2.clearBuffer();         // clear the internal memory
  //  Serial.println("here");

    rb1.loop();
    b2.loop();
    unsigned char result = r1.process();
    if (result) {
      lcd.cls();
      lcd.printStr(ALIGN_CENTER, 28, result == DIR_CW ? "Right" : "Left");
      lcd.display(0);
    }

    if (rb1.isPressed()) {
      lcd.cls();
      lcd.printStr(ALIGN_CENTER, 28, "RB1 Pressed");
      lcd.display(0);
    }

    if (b2.isPressed()) {
      digitalWrite(BUZZER, HIGH);
      lcd.cls();
      lcd.printStr(ALIGN_CENTER, 28, "RB2 Pressed");
      lcd.display(0);
      delay(500);
      digitalWrite(BUZZER, LOW);
    }
  delay(10);
}
