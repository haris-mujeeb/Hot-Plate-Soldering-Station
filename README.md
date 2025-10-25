# HotPlate Soldering Station

![HotPlate Soldering Station](https://github.com/user-attachments/assets/306d3a8b-0f5e-4c47-950f-54478c5a5198)

A DIY hot plate soldering station built with Arduino, featuring precise temperature control, a user-friendly interface, and customizable reflow profiles.

## Features

*   **Accurate Temperature Control:** Utilizes a MAX6675 thermocouple for real-time temperature sensing and a PID algorithm for precise heating control.
*   **Interactive Display:** An ST7920-based LCD provides live temperature readings, menu navigation, and graphical representation of temperature profiles.
*   **Customizable Reflow Profiles:** Users can define and execute temperature-time procedures for various soldering applications.
*   **Fan Control:** Automatic and manual control over a cooling fan for efficient temperature management.
*   **Sound Feedback:** Audible cues for user interactions and status updates.
*   **Manual Temperature Setting:** Allows users to set a target temperature manually.

## Demos

*   **Soldering Demo:** 

https://github.com/user-attachments/assets/88b22581-7505-4ef9-8fa4-fe2a85b40a39

*   **General Demo:** 

https://github.com/user-attachments/assets/b78ecd9c-0edc-4410-997e-d083a02bd01b

*   **Additional Demo:**

https://github.com/user-attachments/assets/c4e61a9d-8154-40fe-b36f-5ddaf2e73ea8


*   **Temperature vs. Time Plot:** 

https://github.com/haris-mujeeb/Temperature_PID_Controller/assets/57053470/dda725fb-990f-4ae4-9134-b8c20268912f


## Hardware Components

*   **Microcontroller:** Arduino (or compatible)
*   **Temperature Sensor:** MAX6675 Thermocouple Amplifier with K-Type Thermocouple
*   **Display:** ST7920 SPI LCD
*   **User Input:** Rotary Encoder with Push Button, additional push button
*   **Heating Element Control:** SCR (Silicon Controlled Rectifier)
*   **Cooling:** DC Fan
*   **Audio Feedback:** Buzzer

## Project Structure

The `code` directory contains the Arduino sketches for the project:

*   `main/`: The primary firmware for the hot plate soldering station.
    *   `main.ino`: The main Arduino sketch.
    *   `src/`: Contains various libraries used by the main sketch.
*   `Temp_PID_Controller_v3/`: An older or alternative PID controller sketch.
*   `test_transistors/`: Sketch for testing transistors.
*   `test1/`: General test sketch.
*   `testconsole/`: Sketch for console-based testing/debugging.
*   `testlcd/`: Sketch for testing the LCD display.
*   `testpid/`: Sketch for testing the PID controller.

## ToDo

*   [x] Optimize PID values or use predicted models to reduce overshoot, as rapid cooling is not feasible.
*   [x] Further refine and implement procedure control features.
*   [x] Enhance FAN and SOUND control functionalities.
*   [x] Complete the assembly of the physical station.
*   [x] Test all components and functionalities.
*   [x] Add necessary holes for USB, sensor, and heater wires in the enclosure.

## Libraries

The following libraries are used in this project:

*   **Rotary:** https://github.com/brianlow/Rotary.git (For rotary encoder input)
*   **ST7920_SPI:** https://github.com/cbm80amiga/ST7920_SPI.git (For controlling the ST7920 LCD)
*   **PropFonts:** https://github.com/cbm80amiga/PropFonts.git (Proportional fonts for the display)
*   **Menu:** https://github.com/0xPIT/menu.git (Menu navigation system)
*   **ezButton:** https://github.com/ArduinoGetStarted/button.git (For easy button handling)
*   **MAX6675-library:** (For MAX6675 thermocouple amplifier)
*   **PID-Library:** (For PID temperature control)
