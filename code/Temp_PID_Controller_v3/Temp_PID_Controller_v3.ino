#include "max6675.h"

const int hotPlatePin = 9;
const int thermoDO = 4;
const int thermoCS = 5;
const int thermoCLK = 6;
const float toleranceInDegrees = 0.5;
int dutyCycle;
float pidOutput, desiredTemp, tempSensor, error, integral, derivative, lastError;
const float Kp = 4;     // Proportional gain
const float Ki = 0.01;  // Integral gain
const float Kd = 5.0;   // Derivative gain
const float Kb = 0.00;  // Back-calculation gain

// Defining phases:
struct phase {
  const int desiredTemp;
  const float dy_dt;
  const float durationSeconds;
};
//                       { desiredTemp,          dy_dt,    durationSeconds }
struct phase RAMP_2_SOAK {  150,                  1,      (150 - 20) / 1  },   // 1-3 deg C / sec
  SOAK{                     180, (180.0 - 150.0) / 80.0,          80      },  // Preheating 150 +- 20 deg C
  RAMP_2_PEAK{              250,                  1,     (250 - 180) / 1  },   // 1-3 deg C / sec
  RAMP_2_REFLOW{            250,                  0,              60      },
  RAMP_2_COOLING{             0,                -3.0,           250 / 3   };  // 2-4 deg C / sec

// Array containing of all 5 phases
const int NUM_OF_PHASES = 5;
const phase Phases[NUM_OF_PHASES] = { RAMP_2_SOAK, SOAK, RAMP_2_PEAK, RAMP_2_REFLOW, RAMP_2_COOLING };

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


void setup() {
  // https://nerdytechy.com/how-to-change-the-pwm-frequency-of-arduino/
  // Pins D9 and D10 - 7.5 Hz 10bit
  TCCR1A = 0b00000011;  // 10bit
  TCCR1B = 0b00000101;  // x1024 phase correct
  Serial.println("====== Start ======");
  Serial.begin(19200);
  pinMode(hotPlatePin, OUTPUT);  // Set heatPlatePin as output

  // wait for MAX chip to stabilize
  delay(500);
  desiredTemp = thermocouple.readCelsius();
}

void loop() {
  phase *ptrCurrentPhase = Phases;
  int phaseCount = 1;

  float deltaTime;
  static unsigned long startTimeMS = millis();
  static unsigned long elapsedTimeMS = millis() - startTimeMS;
  static unsigned long oldTimeMS = millis();

  while (phaseCount < NUM_OF_PHASES) {     
    // calculating delta time
    deltaTime = millis() - oldTimeMS;
    deltaTime = deltaTime / 1000;  // ms -> sec
    oldTimeMS = millis();          // for next cycle

    // increase desiredTemp
    if (desiredTemp < ptrCurrentPhase->desiredTemp) {
      desiredTemp = desiredTemp + (ptrCurrentPhase->dy_dt * deltaTime);
    }

    // PID Controller
    tempSensor = thermocouple.readCelsius();
    lastError = error;
    error = desiredTemp - tempSensor;
    integral += error;
    derivative = error - lastError;
    pidOutput = Kp * error + Ki * integral + Kd * derivative;

    // anti-windup using back calculation
    if (pidOutput < 255) {  // maxOuput = 255
      integral -= Kb * (pidOutput - 255);
    }
    if (pidOutput < 0) {  // minOuput = 0
      integral -= Kb * (pidOutput - 0);
    }

    // saturation block
    dutyCycle = constrain(pidOutput, 0, 255);

    // set PWM value
    analogWrite(hotPlatePin, dutyCycle);  // Turn on heat plate

    // change phase
    if (tempSensor >= ptrCurrentPhase->desiredTemp 
          && elapsedTimeMS >= (ptrCurrentPhase->durationSeconds * 1000.0)) {
      phaseCount++;
      ptrCurrentPhase++;
      startTimeMS = millis();
      Serial.print("Phase: ");
      Serial.print(phaseCount);
      Serial.print(", desiredTemp: ");
      Serial.print(ptrCurrentPhase->desiredTemp);
      Serial.print(", dy_dt: ");
      Serial.println(ptrCurrentPhase->dy_dt);
    }

    // print values
    Serial.print(dutyCycle);
    Serial.print(",");
    Serial.print(desiredTemp);
    Serial.print(",");
    Serial.println(tempSensor);

    // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
    delay(200);
    elapsedTimeMS = millis() - startTimeMS + 200;  // +200 ms = delay
    // elapsedTimeMS *= scale;
  }
  while(1){
    desiredTemp = 0;
    // set PWM value
    analogWrite(hotPlatePin, dutyCycle);  // Turn on heat plate

    // print values
    Serial.print(dutyCycle);
    Serial.print(",");
    Serial.print(desiredTemp);
    Serial.print(",");
    Serial.println(tempSensor);
  }
}