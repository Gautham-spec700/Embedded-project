int ENA = 9;
int IN1 = 8;
int IN2 = 7;
int ENB = 10;
int IN3 = 11;
int IN4 = 12;



void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
 

}

void loop() {
  digitalWrite(IN1, HIGH);//FORWORD
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 150);//0-255 CONTROLS SPEED
  analogWrite(ENB, 150);//0-255 CONTROLS SPEED
  delay(1000);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 150);//0-255 CONTROLS SPEED
  analogWrite(ENB, 150);//0-255 CONTROLS SPEED
  delay(1000);
  digitalWrite(IN1, LOW);//BACKWARDS 
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 150);//0-255 CONTROLS SPEED
  analogWrite(ENB, 150);//0-255 CONTROLS SPEED
  delay(1000);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 150);//0-255 CONTROLS SPEED
  analogWrite(ENB, 150);//0-255 CONTROLS SPEED
  delay(1000);
  digitalWrite(IN1, HIGH);// RIGHT DIRACTION
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 150);//0-255 CONTROLS SPEED
  analogWrite(ENB, 150);//0-255 CONTROLS SPEED
  delay(1000);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 150);//0-255 CONTROLS SPEED
  analogWrite(ENB, 150);//0-255 CONTROLS SPEED
  delay(1000);
  digitalWrite(IN1, LOW);//LEFT DIRECTION 
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 150);//0-255 CONTROLS SPEED
  analogWrite(ENB, 150);//0-255 CONTROLS SPEED
  delay(1000);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 150);//0-255 CONTROLS SPEED
  analogWrite(ENB, 150);//0-255 CONTROLS SPEED
  delay(1000);
  
  


}
