#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Rotary.h>
#include <ezButton.h>

//uses https://github.com/brianlow/Rotary
//uses u8g2 library
//uses ezbutton


//14(CIPO), 15(SCK), 16(COPI)

//below are micro pins
//#define LCD_CLK 15
//#define LCD_DATA 16
//#define LCD_CS 10

#define LCD_CLK 13
#define LCD_DATA 11
#define LCD_CS 10

#define ROT_CLK 2
#define ROT_SD 3
#define ROT_BTN 7
#define BTN2 6
#define BUZZER 5
#define SCR_CNTRL 4
#define FAN_CNTRL 3





Rotary r1 = Rotary(ROT_CLK, ROT_SD);
ezButton rb1(ROT_BTN);
ezButton b2(BTN2);


U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, LCD_CS);

void setup(void) {
  Serial.begin(115200);
  u8g2.begin();
  r1.begin(true);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  u8g2.setFont(u8g2_font_ncenB14_tr);
  delay(100);
}

void loop(void) {
//  u8g2.clearBuffer();         // clear the internal memory
//  Serial.println("here");
    u8g2.firstPage();
    do {
    rb1.loop();
    b2.loop();
    unsigned char result = r1.process();
    if (result) {
      u8g2.drawStr(0, 12, result == DIR_CW ? "Right" : "Left");
    }

    if (rb1.isPressed()) {
      rb1Draw();

    }

    if (b2.isPressed()) {
      digitalWrite(BUZZER, HIGH);
      u8g2.setFont(u8g2_font_fub11_tf);
      u8g2.setFontPosTop();
      u8g2.drawStr(0, 0, "b2 bu");
      delay(500);
      digitalWrite(BUZZER, LOW);
    }
    delay(100); //changing this sometimes help, try 10ms or 20ms

    } while ( u8g2.nextPage() );
//  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(100); 
}

void rb1Draw(){
  u8g2.setFont(u8g2_font_fub11_tf);
  u8g2.setFontPosTop();
  u8g2.drawStr(0, 15, "Rb1 bu");
  u8g2.setFont(u8g2_font_fub11_tf);
  u8g2.setFontPosTop();
  u8g2.drawStr(108, 14, "V"); // 105,13
  u8g2.setFont(u8g2_font_micro_mr );
  u8g2.drawStr(22, 33, "AMPS");
  u8g2.setFont(u8g2_font_micro_mr );
  u8g2.drawStr(74, 33, "SET AMPS");
}
