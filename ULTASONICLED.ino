const int trigPin = 9;
const int echoPin = 10;
int red = 5;
int green = 6;
int buzzer = 11;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode (echoPin, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green,OUTPUT);
   pinMode(buzzer,OUTPUT);
  
  Serial.begin(9600);


}

void loop() {
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  //
  duration = pulseIn(echoPin, HIGH);
  //
  distance = duration * 0.034 / 2;
  //
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  
  if(distance <= 10){
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    tone(buzzer,1500);
  }
  else if  (distance >10 && distance <30){
    digitalWrite(red,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(buzzer,LOW);
  }
  else
  {
    digitalWrite(green,HIGH);
    digitalWrite(red,LOW);
    digitalWrite(buzzer,LOW);
  }
  
  delay(500);

}
