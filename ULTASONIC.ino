const int trigPin = 9;
const int echoPin = 10;
int red = 5;
int green = 6;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode (echoPin, INPUT);
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
    digitalWrite(red,HIGH);
    digitalWrite(green,LOW);
  }else
  {
    digitalWrite(green,HIGH);
    digitalWrite(red,LOW);
  }
  delay(500);

}
