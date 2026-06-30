int pirPin = 2;
int red = 3;
int green = 4;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  Serial.begin(9600);

}
void loop()
{
  digitalWrite(green,HIGH);
  int value = digitalRead(pirPin);
  if (value==HIGH)
  {
    Serial.println("Motion detected");
    digitalWrite(red,HIGH);
    digitalWrite(green,LOW);
  }
  else{
    Serial.println("not detected");
    digitalWrite(red,LOW);
    digitalWrite(green,HIGH);

  }
  delay(500);

}
