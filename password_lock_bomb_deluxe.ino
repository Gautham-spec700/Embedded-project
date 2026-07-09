/*
  ===============================================
   4-Digit Password Lock - BOMB EDITION (DELUXE+)
  ===============================================
  Display: 16x2 I2C LCD (address 0x27)
  Input:   4x4 Matrix Keypad
  No buzzer or LEDs required.

  Libraries required (install via Library Manager):
    - LiquidCrystal_I2C
    - Keypad

  Wiring:
    LCD:      SDA -> A4, SCL -> A5, VCC -> 5V, GND -> GND
    Keypad:   Rows -> 9, 8, 7, 6
              Cols -> 5, 4, 3, 2

  Keypad layout used:
    1 2 3 A
    4 5 6 B
    7 8 9 C
    * 0 # D
    (* = Clear, # = Enter/Submit)

  FUN FEATURES:
    - Scrolling marquee boot title
    - Filling loading bar animation
    - Bouncing "idle" heart animation (like Pong) when untouched
    - "Hacker flicker" effect while typing digits
    - Wrong password -> bomb WALKS across the screen,
      fuse spark animation, countdown, then an
      explosion that SPREADS OUTWARD from the center
    - 3 wrong tries in a row -> escalates to a bigger
      "MEGA MELTDOWN" explosion
    - Random taunt messages after each failure
    - Correct password -> sliding lock-opening animation
      + confetti stars bouncing across the screen
    - Hidden secret code easter egg (try 0007 for a
      surprise - doesn't count as a wrong attempt!)
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- Keypad ----------------
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ---------------- Password ----------------
const String correctPassword = "1234";  // <-- change to your own 4-digit code
const String secretCode = "0007";       // <-- hidden easter egg code
String enteredPassword = "";

int wrongStreak = 0;  // tracks consecutive wrong entries

unsigned long lastActivity = 0;
const unsigned long idleTimeout = 8000;
bool idleShown = false;

// ---------------- Custom Characters (max 8 slots: 0-7) ----------------
byte lockClosed[8]  = {0b01110,0b10001,0b10001,0b11111,0b11011,0b11011,0b11111,0b00000};
byte lockOpen[8]    = {0b01110,0b10000,0b10000,0b11111,0b11011,0b11011,0b11111,0b00000};
byte smiley[8]      = {0b00000,0b01010,0b01010,0b00000,0b10001,0b01110,0b00000,0b00000};
byte bombIcon[8]    = {0b00110,0b01001,0b01001,0b11111,0b10101,0b11111,0b01110,0b00000};
byte explosionIcon[8]={0b10101,0b01110,0b11111,0b11111,0b01110,0b10101,0b00000,0b00000};
byte heart[8]       = {0b00000,0b01010,0b11111,0b11111,0b01110,0b00100,0b00000,0b00000};
byte sparkIcon[8]   = {0b00100,0b01110,0b11111,0b01110,0b10001,0b00100,0b00000,0b00000};
byte starIcon[8]    = {0b00100,0b00100,0b10101,0b01110,0b11111,0b01110,0b10101,0b00000};

#define ICON_LOCK_CLOSED 0
#define ICON_LOCK_OPEN   1
#define ICON_SMILEY      2
#define ICON_BOMB        3
#define ICON_EXPLOSION   4
#define ICON_HEART       5
#define ICON_SPARK       6
#define ICON_STAR        7

// Fun taunt messages shown after a wrong entry
const char* taunts[] = {
  "Nice try, agent!",
  "So close... NOT!",
  "Boom! Not today",
  "Hackers gonna hack",
  "Try again, champ!"
};
const int numTaunts = 5;

void setup() {
  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(A0)); // seed randomness from floating pin

  lcd.createChar(ICON_LOCK_CLOSED, lockClosed);
  lcd.createChar(ICON_LOCK_OPEN, lockOpen);
  lcd.createChar(ICON_SMILEY, smiley);
  lcd.createChar(ICON_BOMB, bombIcon);
  lcd.createChar(ICON_EXPLOSION, explosionIcon);
  lcd.createChar(ICON_HEART, heart);
  lcd.createChar(ICON_SPARK, sparkIcon);
  lcd.createChar(ICON_STAR, starIcon);

  startupAnimation();
  showPasswordPrompt();
  lastActivity = millis();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    lastActivity = millis();
    idleShown = false;
    handleKey(key);
  } else if (!idleShown && millis() - lastActivity > idleTimeout) {
    idleAnimation();
    idleShown = true;
  }
}

// ================= STARTUP ANIMATION =================
void startupAnimation() {
  lcd.clear();
  scrollText("*** SECURE LOCK SYSTEM BOOTING ***", 0);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Loading:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 1);
    lcd.write(ICON_LOCK_CLOSED);
    delay(70);
  }
  delay(300);
}

// Scrolls a message across the top row like a marquee
void scrollText(String text, int row) {
  String padded = "                " + text + "                ";
  lcd.setCursor(0, row);
  lcd.print(padded.substring(0, 16));
  for (int i = 1; i < padded.length() - 16; i++) {
    delay(180);
    lcd.setCursor(0, row);
    lcd.print(padded.substring(i, i + 16));
  }
}

// ================= IDLE ANIMATION (bouncing heart) =================
void idleAnimation() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Zzz... idle");

  int pos = 0;
  int dir = 1;
  for (int frame = 0; frame < 25; frame++) {
    // stop early if a key gets pressed mid-animation
    if (keypad.getKey()) {
      showPasswordPrompt();
      return;
    }
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(pos, 1);
    lcd.write(ICON_HEART);
    delay(140);
    pos += dir;
    if (pos <= 0 || pos >= 15) dir = -dir;
  }
  showPasswordPrompt();
}

// ================= PASSWORD PROMPT =================
void showPasswordPrompt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  lcd.setCursor(15, 0);
  lcd.write(ICON_LOCK_CLOSED);
  lcd.setCursor(0, 1);
  enteredPassword = "";
}

// ================= KEY HANDLING =================
void handleKey(char key) {
  if (key >= '0' && key <= '9') {
    handleDigit(key);
  } else if (key == '*') {
    showPasswordPrompt();
  } else if (key == '#') {
    handleSubmit();
  }
}

void handleDigit(char digit) {
  if (enteredPassword.length() < 4) {
    int pos = enteredPassword.length();

    // "hacker flicker" effect: random chars flash before settling
    const char flickerChars[] = "!@#$%^&*0123456789";
    for (int i = 0; i < 4; i++) {
      lcd.setCursor(pos, 1);
      lcd.print(flickerChars[random(0, 18)]);
      delay(25);
    }

    enteredPassword += digit;
    lcd.setCursor(pos, 1);
    lcd.print('*');
  }
}

void handleSubmit() {
  if (enteredPassword.length() != 4) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter 4 digits!");
    delay(1200);
    showPasswordPrompt();
    return;
  }

  if (enteredPassword == secretCode) {
    easterEggAnimation();
  } else if (enteredPassword == correctPassword) {
    wrongStreak = 0;
    accessGrantedAnimation();
  } else {
    wrongStreak++;
    if (wrongStreak >= 3) {
      megaMeltdown();
      wrongStreak = 0;
    } else {
      bombSequence();
    }
  }

  showPasswordPrompt();
}

// ================= SUCCESS =================
void accessGrantedAnimation() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("ACCESS GRANTED");

  // lock flickers open
  for (int i = 0; i < 3; i++) {
    lcd.setCursor(15, 0);
    lcd.write(ICON_LOCK_CLOSED);
    delay(150);
    lcd.setCursor(15, 0);
    lcd.write(ICON_LOCK_OPEN);
    delay(150);
  }

  // confetti: star leads, smiley trails, moving left to right
  for (int frame = 0; frame < 16; frame++) {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(frame, 1);
    lcd.write(ICON_STAR);
    if (frame >= 2) {
      lcd.setCursor(frame - 2, 1);
      lcd.write(ICON_SMILEY);
    }
    delay(80);
  }

  delay(800);
}

// ================= WRONG PASSWORD: BOMB! =================
void bombSequence() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("BOMB ACTIVATED!");
  delay(500);

  // bomb walks across the top row
  for (int pos = 0; pos < 16; pos++) {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(pos, 0);
    lcd.write(ICON_BOMB);
    delay(55);
  }

  // countdown with a spark running along the fuse (bottom row)
  for (int t = 3; t >= 1; t--) {
    lcd.setCursor(0, 0);
    lcd.print("T-minus  " + String(t) + "s     ");
    for (int i = 0; i < 16; i++) {
      lcd.setCursor(i, 1);
      lcd.write(ICON_SPARK);
      delay(35);
      lcd.setCursor(i, 1);
      lcd.print(' ');
    }
  }

  boomAnimation("Access Denied");
}

// Explosion spreads outward from the center of the screen
void boomAnimation(String finalMessage) {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("* BOOOOM! *");

  int center = 7;
  for (int radius = 0; radius <= 8; radius++) {
    int leftPos = center - radius;
    int rightPos = center + radius;
    if (leftPos >= 0) {
      lcd.setCursor(leftPos, 1);
      lcd.write(ICON_EXPLOSION);
    }
    if (rightPos <= 15) {
      lcd.setCursor(rightPos, 1);
      lcd.write(ICON_EXPLOSION);
    }
    delay(90);
  }

  // screen flash for extra drama
  for (int f = 0; f < 3; f++) {
    lcd.noDisplay();
    delay(100);
    lcd.display();
    delay(100);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(finalMessage);
  lcd.setCursor(0, 1);
  lcd.print(taunts[random(0, numTaunts)]);
  delay(1800);
}

// ================= 3-IN-A-ROW ESCALATION =================
void megaMeltdown() {
  lcd.clear();
  scrollText("!!! MEGA MELTDOWN INCOMING !!!", 0);

  // bomb walks back and forth twice - panic mode
  for (int pass = 0; pass < 2; pass++) {
    for (int pos = 0; pos < 16; pos++) {
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(pos, 0);
      lcd.write(ICON_BOMB);
      lcd.setCursor(15 - pos, 1);
      lcd.write(ICON_BOMB);
      delay(35);
    }
  }

  // faster countdown
  for (int t = 3; t >= 1; t--) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("BOOM IN ");
    lcd.print(t);
    lcd.setCursor(6, 1);
    lcd.write(ICON_BOMB);
    delay(400);
  }

  // double-wide explosion sweeping both rows
  for (int radius = 0; radius <= 8; radius++) {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    int leftPos = 7 - radius;
    int rightPos = 7 + radius;
    if (leftPos >= 0) {
      lcd.setCursor(leftPos, 0); lcd.write(ICON_EXPLOSION);
      lcd.setCursor(leftPos, 1); lcd.write(ICON_EXPLOSION);
    }
    if (rightPos <= 15) {
      lcd.setCursor(rightPos, 0); lcd.write(ICON_EXPLOSION);
      lcd.setCursor(rightPos, 1); lcd.write(ICON_EXPLOSION);
    }
    delay(80);
  }

  for (int f = 0; f < 5; f++) {
    lcd.noDisplay();
    delay(90);
    lcd.display();
    delay(90);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System rebooting");
  lcd.setCursor(0, 1);
  lcd.print("Try again, agent");
  delay(2000);
}

// ================= HIDDEN EASTER EGG =================
void easterEggAnimation() {
  lcd.clear();
  scrollText("*** 007 MODE ACTIVATED ***", 0);

  // spinning icon effect using alternating star/heart
  for (int i = 0; i < 10; i++) {
    lcd.setCursor(7, 1);
    lcd.write(i % 2 == 0 ? ICON_STAR : ICON_HEART);
    delay(150);
    lcd.setCursor(7, 1);
    lcd.print(' ');
    delay(80);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Licensed to");
  lcd.setCursor(0, 1);
  lcd.print("unlock. Shaken.");
  delay(1800);
}
