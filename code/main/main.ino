#include <SPI.h>
#include "src/Rotary/Rotary.h"
#include "src/button/src/ezButton.h"
#include "src/ST7920_SPI/ST7920_SPI.h"
#include "src/PropFonts/small5x6_font.h"
#include "src/PropFonts/c64enh_font.h"
#include "src/PropFonts/small4x5_font.h"
#include "src/PropFonts/tiny3x7sq_font.h"
#include "src/menu/Menu.h"
#include "src/MAX6675-library/max6675.h"
#include "src/PIDArduino/src/PIDcontroller.h"

//14(CIPO), 15(SCK), 16(COPI)

//below are micro pins
#define LCD_CLK 15
#define LCD_DATA 16
#define LCD_CS 17

#define ROT_CLK 5
#define ROT_SD 6
#define ROT_BTN 3

#define BTN2 4
#define BUZZER 2

#define SCR_CNTRL 11
#define FAN_CNTRL 10

//used pins : 2 3 4 5 6 7 8 9 10 11  15 16 17

#define THERMO_CLK 9
#define THERMO_DAT 7
#define THERMO_CS 8

#define SCR_WD 128
#define SCR_HT 64

//pwm at 3 and 11: 980 Hz
// pwm all: 3, 5, 6, 9, 10, 11, 13

MAX6675 thermocouple(THERMO_CLK, THERMO_CS, THERMO_DAT);
Rotary r1 = Rotary(ROT_CLK, ROT_SD);
ezButton rb1(ROT_BTN);
ezButton b2(BTN2);
ST7920_SPI lcd(LCD_CS);

enum input_mode {
  BUTN2,
  BUTN1,
  R_RIGHT,
  R_LEFT,
  CASE_ELSE
};

struct settings {
  int temperature_set;
  int temperature_current;
  int fanspeed;
  bool sound_feedback;
};

// SONG -----------

#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_DB3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_EB3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978
#define REST 0

//pirates of carribean
const uint16_t melody[] PROGMEM = {
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REST,

  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_D5, NOTE_E5, NOTE_A4, REST,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REST,
  NOTE_C5, NOTE_A4, NOTE_B4, REST,

  NOTE_A4, NOTE_A4,
  //Repeat of first part
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REST,

  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_D5, NOTE_E5, NOTE_A4, REST,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REST,
  NOTE_C5, NOTE_A4, NOTE_B4, REST,
  //End of Repeat

  NOTE_E5, REST, REST, NOTE_F5, REST, REST,
  NOTE_E5, NOTE_E5, REST, NOTE_G5, REST, NOTE_E5, NOTE_D5, REST, REST,
  NOTE_D5, REST, REST, NOTE_C5, REST, REST,
  NOTE_B4, NOTE_C5, REST, NOTE_B4, REST, NOTE_A4,

  NOTE_E5, REST, REST, NOTE_F5, REST, REST,
  NOTE_E5, NOTE_E5, REST, NOTE_G5, REST, NOTE_E5, NOTE_D5, REST, REST,
  NOTE_D5, REST, REST, NOTE_C5, REST, REST,
  NOTE_B4, NOTE_C5, REST, NOTE_B4, REST, NOTE_A4
};

const uint8_t durations[] PROGMEM = {
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,

  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,

  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 8, 4, 8,

  8, 8, 4, 8, 8,
  4, 8, 4, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 4,

  4, 8,
  //Repeat of First Part
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,

  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,

  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 8, 4, 8,

  8, 8, 4, 8, 8,
  4, 8, 4, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 4,
  //End of Repeat

  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 4,
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 2,

  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 4,
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 2
};

//---------



//MENU inits -----------

enum Icons {
  IconLeft = 0,
  IconRight = 1,
  IconBack = 2,
  IconBlock = 3
};

#define clampValue(val, lo, hi) \
  if (val > hi) val = hi; \
  if (val < lo) val = lo;
#define maxValue(a, b) ((a > b) ? a : b)
#define minValue(a, b) ((a < b) ? a : b)

Menu::Engine *engine;
uint8_t menuItemsVisible = 4;
uint8_t menuItemHeight = 15;



void renderMenuItem(const Menu::Item_t *mi, uint8_t pos) {

  uint8_t y = pos * menuItemHeight + 2;

  //lcd.drawRect(2, y - 2, 115, menuItemHeight, 1);
  lcd.printStr(10, y, const_cast<char *>(engine->getLabel(mi)));


  // mark items that have children
  if (engine->getChild(mi) != &Menu::NullItem) {
    lcd.printStr(100, y, " >   ");
  }

  if (engine->currentItem == mi) {
    lcd.fillRect(2, y - 2, 115, menuItemHeight, 2);
  }

  lcd.display(0);
}


namespace State {
typedef enum SystemMode_e {
  None = 0,
  Default = (1 << 0),
  Settings = (1 << 1),
  Edit = (1 << 2)
} SystemMode;
};

uint8_t systemState = State::Default;
bool lastEncoderAccelerationState = true;
uint8_t previousSystemState = State::None;


bool menuExit(const Menu::Action_t a) {
  systemState = State::Default;
  return true;
}

bool menuDummy(const Menu::Action_t a) {
  return true;
}


bool menuBack(const Menu::Action_t a) {
  if (a == Menu::actionDisplay) {
    engine->navigate(engine->getParent(engine->getParent()));
  }
  return true;
}

// MenuItem(Name, Label, Next, Previous, Parent, Child, Callback)

MenuItem(miExit, "", Menu::NullItem, Menu::NullItem, Menu::NullItem, miMainmenu, menuExit);
MenuItem(miMainmenu, "< back", miProcedure, Menu::NullItem, miExit, Menu::NullItem, menuDummy);
MenuItem(miProcedure, "Procedure         >", miFan, miMainmenu, miExit, Menu::NullItem, menuDummy);
MenuItem(miFan, "Fan :", miSound, miProcedure, miExit, Menu::NullItem, menuDummy);
MenuItem(miSound, "Sound :", miAbout, miFan, miExit, Menu::NullItem, menuDummy);
MenuItem(miAbout, "About               >", Menu::NullItem, miSound, miExit, Menu::NullItem, menuDummy);




const uint8_t table_icon[] PROGMEM = { 16, 16,
                                       0x80, 0xC0, 0x60, 0x10, 0x0, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x88, 0xC8, 0x28, 0xF8, 0xF8,
                                       0xFF, 0xFF, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0xFF, 0xFF, 0x0, 0xF, 0xF };

const uint8_t fire_icon[] PROGMEM = { 16, 16,
                                      12, 159, 243, 96, 0, 0, 0, 12, 159, 243, 96, 0, 12, 159, 243, 96,
                                      3, 7, 28, 24, 0, 0, 0, 3, 7, 28, 24, 0, 3, 7, 28, 24 };

const uint8_t fan_icon[] PROGMEM = { 16, 16,
                                     254, 255, 3, 27, 51, 195, 195, 51, 27, 3, 255, 254, 0, 0, 0, 0,
                                     31, 63, 48, 50, 51, 48, 48, 51, 50, 48, 63, 31, 0, 0, 0, 0 };

const uint8_t fan2_icon[] PROGMEM = { 16, 16,
                                      254, 255, 3, 99, 99, 195, 251, 155, 131, 3, 255, 254, 0, 0, 0, 0,
                                      31, 63, 48, 48, 54, 55, 48, 49, 49, 48, 63, 31, 0, 0, 0, 0 };


//----------------



#define REFRESH_LOOP_INTERVAL 200
float Kp = 2;     // Proportional gain
float Ki = 0.01;  // Integral gain
float Kd = 5;     // Derivative gain
int minOutput = 0;
int maxOutput = 255;

PIDController pid;
struct settings config;


#ifndef ARDPRINTF
#define ARDPRINTF
#define ARDBUFFER 16
#include <stdarg.h>
#include <Arduino.h>

int ardprintf(char *str, ...) {
  int i, count = 0, j = 0, flag = 0;
  char temp[ARDBUFFER + 1];
  for (i = 0; str[i] != '\0'; i++)
    if (str[i] == '%') count++;

  va_list argv;
  va_start(argv, count);
  for (i = 0, j = 0; str[i] != '\0'; i++) {
    if (str[i] == '%') {
      temp[j] = '\0';
      Serial.print(temp);
      j = 0;
      temp[0] = '\0';

      switch (str[++i]) {
        case 'd':
          Serial.print(va_arg(argv, int));
          break;
        case 'l':
          Serial.print(va_arg(argv, long));
          break;
        case 'f':
          Serial.print(va_arg(argv, double));
          break;
        case 'c':
          Serial.print((char)va_arg(argv, int));
          break;
        case 's':
          Serial.print(va_arg(argv, char *));
          break;
        default:;
      };
    } else {
      temp[j] = str[i];
      j = (j + 1) % ARDBUFFER;
      if (j == 0) {
        temp[ARDBUFFER] = '\0';
        Serial.print(temp);
        temp[0] = '\0';
      }
    }
  };
  Serial.println();
  return count + 1;
}
#undef ARDBUFFER
#endif

input_mode getinput() {

  rb1.loop();
  b2.loop();
  unsigned char result = r1.process();
  if (result) {
    if (config.sound_feedback) {
      digitalWrite(BUZZER, HIGH);
      delay(20);
      digitalWrite(BUZZER, LOW);
    }
    return result == DIR_CW ? R_RIGHT : R_LEFT;
  }


  if (rb1.isPressed()) {
    if (config.sound_feedback) {
      digitalWrite(BUZZER, HIGH);
      delay(20);
      digitalWrite(BUZZER, LOW);
    }
    return BUTN1;
  }

  if (b2.isPressed()) {
    if (config.sound_feedback) {
      digitalWrite(BUZZER, HIGH);
      delay(20);
      digitalWrite(BUZZER, LOW);
    }

    return BUTN2;
  }
  return CASE_ELSE;
}

void adjustscreen(int *vars) {
  lcd.setFont(Tiny3x7SqPL);
  lcd.cls();


  char cy3[6];
  char cy2[6];

  char cx1[6];
  char cx2[6];
  char cx3[6];
  char cx4[6];
  char cx5[6];

  input_mode observe;

  int y2 = vars[5], y3 = vars[6];
  int x1 = vars[0], x2 = vars[1], x3 = vars[2], x4 = vars[3], x5 = vars[4];
  bool updatemenu = true;

  bool adjust_graph = true;
  int graph_parameter = 0;

  while (1) {
    if (updatemenu) {
      lcd.cls();
      sprintf(cy3, "%d", y3);
      sprintf(cy2, "%d", y2);

      sprintf(cx1, "%d", x1);
      sprintf(cx2, "%d", x2);
      sprintf(cx3, "%d", x3);
      sprintf(cx4, "%d", x4);
      sprintf(cx5, "%d", x5);


      int origin_x = 12;
      int origin_y = 55;
      int hy = 48;
      int wx = 110;
      int temp_max = 300;
      int second_max = 400;
      int y_scale = floor(temp_max / hy);
      int x_scale = ceil(second_max / wx) + 1;

      lcd.printStr(origin_x + wx - 5, origin_y - 8, "(s)");
      lcd.printStr(origin_x + 2, origin_y - hy - 5, "(C)");
      lcd.drawLine(origin_x, origin_y, origin_x + wx, origin_y, 1);
      lcd.drawLine(origin_x, origin_y, origin_x, origin_y - hy - 5, 1);

      int y_offset = 0;
      int x_offset = 0;

      lcd.printStr(0, y_offset + origin_y - (y3 / y_scale), const_cast<char *>(cy3));
      lcd.printStr(0, y_offset + origin_y - (y2 / y_scale), const_cast<char *>(cy2));


      lcd.printStr(x_offset + origin_x + (x1 / x_scale), origin_y + 2, const_cast<char *>(cx1));
      lcd.printStr(x_offset + origin_x + (x2 / x_scale), origin_y + 2, const_cast<char *>(cx2));
      lcd.printStr(x_offset + origin_x + (x3 / x_scale), origin_y + 2, const_cast<char *>(cx3));
      lcd.printStr(x_offset + origin_x + (x4 / x_scale), origin_y + 2, const_cast<char *>(cx4));
      lcd.printStr(x_offset + origin_x + (x5 / x_scale), origin_y + 2, const_cast<char *>(cx5));

      lcd.drawLine(origin_x, origin_y, origin_x + x1 / x_scale, origin_y - y2 / y_scale, 1);
      lcd.drawLine(origin_x + x1 / x_scale, origin_y - y2 / y_scale, origin_x + x2 / x_scale, origin_y - y2 / y_scale, 1);
      lcd.drawLine(origin_x + x2 / x_scale, origin_y - y2 / y_scale, origin_x + (x3 / x_scale), origin_y - (y3 / y_scale), 1);
      lcd.drawLine(origin_x + (x3 / x_scale), origin_y - (y3 / y_scale), origin_x + x4 / x_scale, origin_y - (y3 / y_scale), 1);
      lcd.drawLine(origin_x + x4 / x_scale, origin_y - (y3 / y_scale), origin_x + x5 / x_scale, origin_y, 1);

      lcd.display(0);
      updatemenu = false;
    }

    observe = getinput();
    if (((observe == R_RIGHT) || (observe == R_LEFT)) && adjust_graph) {
      switch (graph_parameter) {
        case 6:
          observe == R_RIGHT ? y3 += 10 : y3 -= 10;
          if (y3 > 300) y3 = 300;
          if (y3 < 50) y3 = 50;
          break;

        case 5:
          observe == R_RIGHT ? y2 += 10 : y2 -= 10;
          if (y2 > 300) y2 = 300;
          if (y2 < 50) y2 = 50;
          break;

        case 4:
          observe == R_RIGHT ? x1 += 10 : x1 -= 10;
          if (x1 > 400) x1 = 400;
          if (x1 < 0) x1 = 0;
          break;

        case 3:
          observe == R_RIGHT ? x2 += 10 : x2 -= 10;
          if (x2 > 400) x2 = 400;
          if (x2 < 0) x2 = 0;
          break;

        case 2:
          observe == R_RIGHT ? x3 += 10 : x3 -= 10;
          if (x3 > 400) x3 = 400;
          if (x3 < 0) x3 = 0;
          break;

        case 1:
          observe == R_RIGHT ? x4 += 10 : x4 -= 10;
          if (x4 > 400) x4 = 400;
          if (x4 < 0) x4 = 0;
          break;

        case 0:
          observe == R_RIGHT ? x5 += 10 : x5 -= 10;
          if (x5 > 400) x5 = 400;
          if (x5 < 0) x5 = 0;
          break;
      }
      updatemenu = true;
    }

    if (observe == BUTN1) {
      graph_parameter++;
      if (graph_parameter > 6) graph_parameter = 0;
      updatemenu = true;
    }

    if (observe == BUTN2) {
      vars[0] = x1;
      vars[1] = x2;
      vars[2] = x3;
      vars[3] = x4;
      vars[4] = x5;
      vars[5] = y2;
      vars[6] = y3;
      lcd.cls();
      return;
    }
  }
}

int getTempAtTime(int elapsed_time, int *gprms) {
  int y2 = gprms[5];
  int y3 = gprms[6];
  int x1 = gprms[0];
  int x2 = gprms[1];
  int x3 = gprms[2];
  int x4 = gprms[3];
  int x5 = gprms[4];

  if (elapsed_time < x1) return (y2 / x1) * elapsed_time;
  if (elapsed_time < x2) return y2;
  if (elapsed_time < x3) return ((y3 - y2) / (x3 - x2)) * elapsed_time - ((y3 - y2) / (x3 - x2)) * x2 + y2;
  if (elapsed_time < x4) return y3;
  if (elapsed_time < x5) return (-y3/(x5-x4))*elapsed_time - (-y3/(x5-x4))*x4 + y3;
}

#define REFRESH_TIMEPROCEDURE 1000
void procedurescreen() {
  lcd.setFont(Tiny3x7SqPL);
  lcd.cls();


  char cy3[6];
  char cy2[6];

  char cx1[6];
  char cx2[6];
  char cx3[6];
  char cx4[6];
  char cx5[6];

  char ctempersture_cur[6];
  char celapsed_time[6];

  input_mode observe;

  int y2 = 150, y3 = 270;
  int x1 = 70, x2 = 130, x3 = 190, x4 = 280, x5 = 370;
  int gprms[] = { x1, x2, x3, x4, x5, y2, y3 };

  int menu_id = 0;
  int elapsed_time = 0;
  bool updatemenu = true;

  int origin_x = 12;
  int origin_y = 55;
  int hy = 38;
  int wx = 80;
  int temp_max = 300;
  int second_max = 400;
  int y_scale = floor(temp_max / hy);
  int x_scale = ceil(second_max / wx) + 1;

  unsigned long previousMillis_PIDLOOP = 0;
  unsigned long currentMillis;
  int procedure_status = 1;  // -1 for pause 0 for stop and 1 for started (ongoing)
  char disp_option[] = "Start ";
  config.temperature_set = 40;
  pid.setpoint(config.temperature_set);
  int pid_output;

  while (1) {
    currentMillis = millis();
    if (currentMillis - previousMillis_PIDLOOP > REFRESH_LOOP_INTERVAL) {
      previousMillis_PIDLOOP = currentMillis;
      config.temperature_current = thermocouple.readCelsius();
      pid_output = pid.compute(config.temperature_current);
      Serial.print(config.temperature_current);
      Serial.print(" ");
      Serial.println(pid_output);
      analogWrite(SCR_CNTRL, pid_output);
    }

    if (currentMillis - previousMillis_PIDLOOP > REFRESH_TIMEPROCEDURE) {
      if (procedure_status == 1) {
        elapsed_time++;
        config.temperature_set = getTempAtTime(elapsed_time, gprms);
        pid.setpoint(config.temperature_set);
        Serial.println(config.temperature_set);
      } else if (procedure_status == -1) {
        Serial.println("Procedure paused");
      }
    }

    if (updatemenu) {
      lcd.cls();
      sprintf(cy3, "%d", y3);
      sprintf(cy2, "%d", y2);

      sprintf(cx1, "%d", x1);
      sprintf(cx2, "%d", x2);
      sprintf(cx3, "%d", x3);
      sprintf(cx4, "%d", x4);
      sprintf(cx5, "%d", x5);
      sprintf(celapsed_time, "%d s", elapsed_time);
      sprintf(ctempersture_cur, "%d C", config.temperature_current);

      lcd.setFont(Small5x6PL);

      lcd.printStr(origin_x + wx + 2, 2, disp_option);
      lcd.printStr(origin_x + wx + 2, 13, "Cancel");
      lcd.printStr(origin_x + wx + 2, 24, "Adjust");
      lcd.printStr(origin_x + wx + 2, 35, "< back");

      lcd.setFont(Tiny3x7SqPL);


      lcd.printStr(2, 2, "Time:");
      lcd.printStr(23, 2, const_cast<char *>(celapsed_time));
      lcd.printStr(48, 2, "Temp:");
      lcd.printStr(69, 2, const_cast<char *>(ctempersture_cur));


      lcd.printStr(origin_x + wx - 5, origin_y - 8, "(s)");
      lcd.printStr(origin_x + 2, origin_y - hy - 5, "(C)");
      lcd.drawLine(origin_x, origin_y, origin_x + wx, origin_y, 1);
      lcd.drawLine(origin_x, origin_y, origin_x, origin_y - hy - 5, 1);

      int y_offset = 0;
      int x_offset = 0;

      lcd.printStr(0, y_offset + origin_y - (y3 / y_scale), const_cast<char *>(cy3));
      lcd.printStr(0, y_offset + origin_y - (y2 / y_scale), const_cast<char *>(cy2));


      lcd.printStr(x_offset + origin_x + (x1 / x_scale), origin_y + 2, const_cast<char *>(cx1));
      lcd.printStr(x_offset + origin_x + (x2 / x_scale), origin_y - 8, const_cast<char *>(cx2));
      lcd.printStr(x_offset + origin_x + (x3 / x_scale), origin_y + 2, const_cast<char *>(cx3));
      lcd.printStr(x_offset + origin_x + (x4 / x_scale), origin_y - 8, const_cast<char *>(cx4));
      lcd.printStr(x_offset + origin_x + (x5 / x_scale), origin_y + 2, const_cast<char *>(cx5));

      lcd.drawLine(origin_x, origin_y, origin_x + x1 / x_scale, origin_y - y2 / y_scale, 1);
      lcd.drawLine(origin_x + x1 / x_scale, origin_y - y2 / y_scale, origin_x + x2 / x_scale, origin_y - y2 / y_scale, 1);
      lcd.drawLine(origin_x + x2 / x_scale, origin_y - y2 / y_scale, origin_x + (x3 / x_scale), origin_y - (y3 / y_scale), 1);
      lcd.drawLine(origin_x + (x3 / x_scale), origin_y - (y3 / y_scale), origin_x + x4 / x_scale, origin_y - (y3 / y_scale), 1);
      lcd.drawLine(origin_x + x4 / x_scale, origin_y - (y3 / y_scale), origin_x + x5 / x_scale, origin_y, 1);

      switch (menu_id) {
        case 3:
          lcd.fillRect(origin_x + wx, 0, 32, 10, 2);
          break;

        case 2:
          lcd.fillRect(origin_x + wx, 12, 37, 10, 2);
          break;

        case 1:
          lcd.fillRect(origin_x + wx, 23, 37, 10, 2);
          break;

        case 0:
          lcd.fillRect(origin_x + wx, 33, 35, 10, 2);
          break;
      }

      lcd.display(0);
      updatemenu = false;
    }



    observe = getinput();
    if (((observe == R_RIGHT) || (observe == R_LEFT))) {
      observe == R_LEFT ? menu_id++ : menu_id--;
      if (menu_id > 3) menu_id = 0;
      if (menu_id < 0) menu_id = 3;
      updatemenu = true;
      //set actual temperature here
    }

    if (observe == BUTN1) {
      menu_id++;
      if (menu_id > 3) menu_id = 0;
      updatemenu = true;
    }

    if (observe == BUTN2) {
      switch (menu_id) {
        case 3:
          if (procedure_status == 0) {
            procedure_status = 1;
            elapsed_time = 0;
            strcpy(disp_option, "Start ");
          } else if (procedure_status == 1) {
            procedure_status = -1;
            strcpy(disp_option, "Paused");
          }
          updatemenu = true;
          break;


        case 2:
          procedure_status = 0;
          elapsed_time = 0;
          config.temperature_set = 40;
          pid.setpoint(config.temperature_set);
          analogWrite(SCR_CNTRL, 0);
          updatemenu = true;
          break;

        case 1:
          adjustscreen(gprms);
          y2 = gprms[5];
          y3 = gprms[6];
          x1 = gprms[0];
          x2 = gprms[1];
          x3 = gprms[2];
          x4 = gprms[3];
          x5 = gprms[4];

          updatemenu = true;
          break;

        case 0:
          return;
          break;
      }
    }
  }
}

void about() {
  lcd.setFont(Small5x6PL);
  lcd.cls();
  lcd.printStr(ALIGN_CENTER, 10, "Programmed by Surya");
  lcd.printStr(ALIGN_CENTER, 20, "Assembled by Usama");
  lcd.printStr(ALIGN_CENTER, 30, "Modeled by Haris");
  lcd.printStr(ALIGN_CENTER, 50, "v1.01");
  lcd.display(0);
  int size = sizeof(durations) / sizeof(durations[0]);
  Serial.println(size);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    uint8_t dur = pgm_read_word(&durations[note]);
    int duration = 1000 / dur;
    tone(BUZZER, pgm_read_word(&melody[note]), duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing:
    noTone(BUZZER);
  }
  lcd.cls();
  lcd.printStr(ALIGN_CENTER, 30, "Denke Schonn!!");
  lcd.display(0);
  delay(1000);
}

void startupsceen() {
  lcd.cls();
  lcd.setFont(c64enh);
  //lcd.setCharMinWd(1);
  lcd.printStr(ALIGN_CENTER, 24, "HotPlate Station");
  lcd.printStr(ALIGN_CENTER, 34, "ELab v1.01");
  lcd.drawRect(8, 20, 127 - 15, 65 - 20 * 2, 1);
  lcd.display(0);
  digitalWrite(BUZZER, HIGH);
  delay(1000);
  digitalWrite(BUZZER, LOW);
  lcd.cls();
  lcd.display(0);
}


#define HOMEMENU_GOMENU 0
#define HOMEMENU_ADJUST_TEMP 1

void homescreen(void) {
  lcd.setFont(Small5x6PL);
  lcd.cls();
  int menu_setting = 0;
  bool menu_adjust_enable = true;

  char cur_temp[6];
  char set_temp[6];
  char fan_speed[6];
  bool anim_toggle = true;
  input_mode observe;
  const unsigned long REFRESH_INTERVAL = 1000;  // ms
  unsigned long previousMillis = 0;
  bool updatemenu = true;
  unsigned long currentMillis = 0;
  unsigned long previousMillis_PIDLOOP = 0;
  int pid_output = 0;

  while (1) {
    currentMillis = millis();
    if (currentMillis - previousMillis_PIDLOOP > REFRESH_LOOP_INTERVAL) {
      previousMillis_PIDLOOP = currentMillis;
      config.temperature_current = thermocouple.readCelsius();
      pid_output = pid.compute(config.temperature_current);
      Serial.print(config.temperature_current);
      Serial.print(" ");
      Serial.println(pid_output);
      analogWrite(SCR_CNTRL, pid_output);
    }
    if ((currentMillis - previousMillis > REFRESH_INTERVAL) || updatemenu) {
      previousMillis = currentMillis;


      lcd.cls();
      sprintf(set_temp, "%d C", config.temperature_set);
      sprintf(cur_temp, "%d C", config.temperature_current);
      sprintf(fan_speed, "%d %%", config.fanspeed);

      lcd.setFont(Small4x5PL);
      lcd.printStr(15, 2, "ELab Hot Soldering Bed");
      lcd.drawRect(10, 0, 110, 9, 1);
      lcd.setFont(Small5x6PL);

      lcd.printStr(5, 15, "Set:");
      lcd.printStr(5, 25, "Cur:");
      lcd.printStr(30, 15, const_cast<char *>(set_temp));
      lcd.printStr(30, 25, const_cast<char *>(cur_temp));
      lcd.printStr(74, 15, const_cast<char *>(fan_speed));
      lcd.drawBitmap(table_icon, 105, 25);
      if (config.temperature_set > config.temperature_current) {
        lcd.drawBitmap(fire_icon, 105, 13);
        lcd.setFont(Small4x5PL);
        lcd.printStr(5, 35, "Bed Heating ...");
        lcd.setFont(Small5x6PL);
      };



      anim_toggle ? lcd.drawBitmap(fan_icon, 75, 25) : lcd.drawBitmap(fan2_icon, 75, 25);
      anim_toggle = !anim_toggle;

      lcd.setFont(Small5x6PL);
      lcd.drawRect(15, 45, 29, 12, 1);
      lcd.printStr(18, 47, "Menu");

      lcd.drawRect(55, 45, 65, 12, 1);
      lcd.printStr(58, 47, "Adjust Temp");

      menu_setting == HOMEMENU_ADJUST_TEMP ? lcd.fillRect(55, 45, 65, 12, 2) : lcd.fillRect(15, 45, 29, 12, 2);

      lcd.display(0);
      updatemenu = false;
    }

    observe = getinput();
    if (((observe == R_RIGHT) || (observe == R_LEFT)) && menu_setting == HOMEMENU_ADJUST_TEMP && menu_adjust_enable) {
      observe == R_RIGHT ? config.temperature_set++ : config.temperature_set--;
      if (config.temperature_set < 40) config.temperature_set = 40;
      if (config.temperature_set > 300) config.temperature_set = 300;
      pid.setpoint(config.temperature_set);
      updatemenu = true;
      //set actual temperature here
    }
    if (observe == BUTN1) {
      menu_setting++;
      if (menu_setting > 1)
        menu_setting = 0;

      updatemenu = true;
    }

    if (observe == BUTN2) {
      if (menu_setting == 0)
        return;
    }
  }
}

void menuscreen(void) {
  lcd.setFont(Small5x6PL);
  lcd.cls();
  analogWrite(SCR_CNTRL, 0);  //when browsing menu turn off the heater
  input_mode observe;
  engine->navigate(&miMainmenu);
  bool update_menu = true;
  while (1) {
    observe = getinput();
    if ((observe == R_RIGHT) || (observe == R_LEFT)) {
      if (!(engine->getItemInfo(engine->currentItem).position == 1 && observe == R_LEFT) && !(engine->getItemInfo(engine->currentItem).position == 5 && observe == R_RIGHT)) {
        Serial.println(engine->getItemInfo(engine->currentItem).position);
        engine->navigate((observe == R_RIGHT) ? engine->getNext() : engine->getPrev());
        update_menu = true;
      }
    }

    if (observe == BUTN1) {
      engine->navigate(engine->getNext());
    }

    if (observe == BUTN2) {
      if (engine->currentItem == &miMainmenu)
        return;

      if (engine->currentItem == &miAbout) {
        about();
        engine->navigate(&miMainmenu);

        lcd.setFont(Small5x6PL);
        update_menu = true;
      }

      if (engine->currentItem == &miProcedure) {
        procedurescreen();
        engine->navigate(&miMainmenu);

        lcd.setFont(Small5x6PL);
        update_menu = true;
      }
    }

    if (update_menu) {
      update_menu = false;
      Menu::Info_t mi = engine->getItemInfo(engine->currentItem);
      uint8_t sbTop = 0, sbWidth = 4, sbLeft = 120;
      uint8_t sbItems = minValue(menuItemsVisible, mi.siblings);
      uint8_t sbHeight = sbItems * menuItemHeight;
      uint8_t sbMarkHeight = sbHeight * (sbItems) / mi.siblings;
      uint8_t sbMarkTop = ((sbHeight - sbMarkHeight) / mi.siblings) * (mi.position - 1);

      lcd.cls();

      lcd.drawRect(sbLeft, sbTop, sbWidth, sbHeight, 1);
      lcd.drawRect(sbLeft, sbMarkTop, sbWidth, sbMarkHeight, 1);
      lcd.fillRect(sbLeft, sbMarkTop, sbWidth, sbMarkHeight + 3, 1);
      engine->render(renderMenuItem, menuItemsVisible);
    }
  }
}


void setup(void) {
  Serial.begin(115200);
  r1.begin(true);
  pinMode(BUZZER, OUTPUT);
  pinMode(SCR_CNTRL, OUTPUT);
  pinMode(FAN_CNTRL, OUTPUT);
  digitalWrite(BUZZER, LOW);
  digitalWrite(SCR_CNTRL, LOW);
  analogWrite(FAN_CNTRL, 50);

  SPI.begin();
  config.sound_feedback = true;
  config.temperature_set = 27;
  config.temperature_current = thermocouple.readCelsius();
  config.fanspeed = 80;
  pid.begin();           // initialize the PID instance
  pid.setpoint(27);      // The "goal" the PID controller tries to "reach"
  pid.tune(Kp, Ki, Kd);  // Tune the PID, arguments: kP, kI, kD
  pid.limit(0, 255);


  Serial.print("Started Base");
  lcd.init();
  lcd.cls();

  startupsceen();
  delay(100);

  engine = new Menu::Engine(&Menu::NullItem);
  menuExit(Menu::actionDisplay);  // reset to initial state
}

void loop(void) {
  homescreen();
  menuscreen();
}
