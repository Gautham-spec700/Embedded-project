const int trigPin = 9;
const int echoPin = 10;
#include <Servo.h>

Servo myservo;


long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode (echoPin, INPUT);
  myservo.attach(11);
  
  
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
    myservo.write(0); 
  }
  else if  (distance >10 && distance <30){
   myservo.write(90); 
  }
  else
  {
    myservo.write(180); 
  }
  
  delay(500);

}
