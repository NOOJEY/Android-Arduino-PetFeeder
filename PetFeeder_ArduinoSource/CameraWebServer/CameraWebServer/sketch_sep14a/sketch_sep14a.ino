int motorA = 6;
int motorB = 5;
int ENA = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(motorB, OUTPUT);
  pinMode(motorA, OUTPUT);
  digitalWrite(ENA, LOW);
  pinMode(ENA, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(motorA, HIGH);
      delay(3000);
}
