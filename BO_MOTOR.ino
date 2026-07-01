int ENA = 9;
int IN1 = 8;
int IN2 = 7;


void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
 

}

void loop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(3000);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(3000);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(3000);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(3000);
  analogWrite(ENA, 50);//0-255 CONTROLS SPEED


}
