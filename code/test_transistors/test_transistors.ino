
#define SCR_CNTRL 11
#define FAN_CNTRL 10

void setup() {
  // put your setup code here, to run once:
  pinMode(SCR_CNTRL, OUTPUT);
  pinMode(FAN_CNTRL, OUTPUT);
  digitalWrite(SCR_CNTRL, LOW);
  digitalWrite(FAN_CNTRL, LOW);
  analogWrite(SCR_CNTRL,70);
}

void loop() {
  // put your main code here, to run repeatedly:
   
  delay(20);
}
