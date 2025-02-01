#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, 17);

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,15,"Hello World!");
    delay(100);
  } while ( u8g2.nextPage() );
  delay(1000);
}