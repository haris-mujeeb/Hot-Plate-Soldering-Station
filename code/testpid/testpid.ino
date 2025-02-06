
#include "src/PIDArduino/src/PIDcontroller.h"
#include "src/MAX6675-library/max6675.h"


#define THERMO_CLK 9
#define THERMO_DAT 7
#define THERMO_CS 8

MAX6675 thermocouple(THERMO_CLK, THERMO_CS, THERMO_DAT);


 PIDController pid;

void setup () {
  Serial.begin(115200);   // Some methods require the Serial.begin() method to be called first
  pid.begin();          // initialize the PID instance
  pid.setpoint(28);    // The "goal" the PID controller tries to "reach"
  pid.tune(1, 1, 1);    // Tune the PID, arguments: kP, kI, kD
  pid.limit(0, 255);    // Limit the PID output between 0 and 255, this is important to get rid of integral windup!
}

void loop () {
  int sensorValue = thermocouple.readCelsius();         // Read the value from the sensor
  int output = pid.compute(sensorValue);    // Let the PID compute the value, returns the optimal output
  Serial.print(sensorValue);
  Serial.print("  ");
  Serial.println(output);
  delay(300);                                // Delay for 30 ms
}
