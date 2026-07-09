#include <WiFi.h>
#include <WebServer.h>

// ================= MOTOR PINS =================
int ENA = 13;
int IN1 = 12;
int IN2 = 14;
int ENB = 27;
int IN3 = 26;
int IN4 = 25;
// ================= WIFI ACCESS POINT =================
const char* ssid = "ROBOT122";
const char* password = "12345678";

WebServer server(80);

// ================= SPEED VARIABLES =================
int sliderSpeed = 255;  // Core speed variable from your single MIT App slider
int currentSpeed = 0;   // System speed variable used for ramping
const int accelStep = 5;

String targetDirection = "S";

// ================= MOTOR CONTROL =================
void setMotor(String dir, int speedLeft, int speedRight) {

  // -------- FORWARD --------
  if (dir == "F") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  // -------- BACKWARD --------
  else if (dir == "B") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  // -------- LEFT --------
  else if (dir == "L") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  // -------- RIGHT --------
  else if (dir == "R") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  // -------- STOP --------
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    speedLeft = 0;
    speedRight = 0;
  }

  // PWM SPEED CONTROL (Sends custom calculated speeds to each channel)
  ledcWrite(ENA, speedLeft);
  ledcWrite(ENB, speedRight);
}

// ================= HANDLE WEB REQUEST =================
void handleCommand() {

  // DIRECTION
  if (server.hasArg("State")) {
    targetDirection = server.arg("State");
    Serial.print("Direction: ");
    Serial.println(targetDirection);
  }

  // SPEED (Matches your current MIT App Inventor setup perfectly)
  if (server.hasArg("Speed")) {
    sliderSpeed = constrain(server.arg("Speed").toInt(), 0, 255);
    Serial.print("Global Speed Set to: ");
    Serial.println(sliderSpeed);
  }

  server.send(200, "text/plain", "OK");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  // MOTOR PINS
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // ================= PWM SETUP (ESP32 CORE 3.x STYLE) =================
  ledcAttach(ENA, 20000, 8);
  ledcAttach(ENB, 20000, 8);

  // ================= CREATE WIFI ACCESS POINT =================
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("WiFi AP Started");
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  // ================= WEB SERVER =================
  server.on("/", handleCommand);
  server.begin();

  Serial.println("Web Server Started");
}

// ================= MAIN LOOP =================
void loop() {
  server.handleClient();

  // STOP should gradually reduce speed to 0
  int targetSpeed = (targetDirection == "S") ? 0 : sliderSpeed;

  // SMOOTH ACCELERATION
  if (currentSpeed < targetSpeed) {
    currentSpeed += accelStep;
  }
  // SMOOTH DECELERATION
  else if (currentSpeed > targetSpeed) {
    currentSpeed -= accelStep;
  }
  currentSpeed = constrain(currentSpeed, 0, 255);

  // =======================================================================
  // INDIVIDUAL MOTOR CALIBRATION OPTIONS
  // Change these two values (between 0 and 255) to make your robot drive straight.
  // Lower the number of whichever motor is driving "too fast".
  // =======================================================================
  int maxLeftSpeed  = 215; // Explicit option to change Left Motor max speed
  int maxRightSpeed = 255; // Explicit option to change Right Motor max speed
  // =======================================================================

  int speedLeft = currentSpeed;
  int speedRight = currentSpeed;

  // Only apply individual speed balancing limits when driving in a straight line
  if (targetDirection == "F" || targetDirection == "B") {
    speedLeft = map(currentSpeed, 0, 255, 0, maxLeftSpeed);
    speedRight = map(currentSpeed, 0, 255, 0, maxRightSpeed);
  }

  // RUN MOTORS WITH BALANCED SPEEDS
  setMotor(targetDirection, speedLeft, speedRight);

  delay(20);
}