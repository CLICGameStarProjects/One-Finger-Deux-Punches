#include <BleKeyboard.h>

#define gloveLeft 2
#define gloveRight 3
#define bag 4

// LEDs
#define ledGloveLeft 8
#define ledGloveRight 9
#define ledBag 10
#define ledPunchLeft 11
#define ledPunchRight 12
#define ledControl 13

BleKeyboard bleKeyboard("One Finger Deux Punches", "Stache", 69);

int contactGloveLeft, contactGloveRight, contactBag;
bool punchLeft, punchRight;

void blinkLed(int pin, int times, float seconds) {
  for(int i = 0; i < times; ++i) {
    digitalWrite(pin, HIGH);
    delay(seconds * 1000);
    digitalWrite(pin, LOW);
    delay(seconds * 1000);
  }
}

void setLed(int pin, bool state) {
  if(state) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}
 
void setup() {
  Serial.begin(115200);
  
  pinMode(gloveLeft, INPUT);
  pinMode(gloveRight, INPUT);
  pinMode(bag, INPUT);

  // LEDs
  pinMode(ledGloveLeft, OUTPUT);
  pinMode(ledGloveRight, OUTPUT);
  pinMode(ledBag, OUTPUT);
  pinMode(ledPunchLeft, OUTPUT);
  pinMode(ledPunchRight, OUTPUT);
  pinMode(ledControl, OUTPUT);

  contactGloveLeft = LOW;
  contactGloveRight = LOW;
  contactBag = LOW;

  punchLeft = false;
  punchRight = false;
  
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();

  blinkLed(ledControl, 2, 0.5);
}
 
void loop() {
  if(bleKeyboard.isConnected()) {
    setLed(ledControl, true);
    
    contactGloveLeft = digitalRead(gloveLeft);
    contactGloveRight = digitalRead(gloveRight);
    contactBag = digitalRead(bag);

    setLed(ledGloveLeft, contactGloveLeft == LOW);
    setLed(ledGloveRight, contactGloveRight == LOW);
    setLed(ledBag, contactBag == LOW);

    Serial.print("contactGloveLeft: ");
    Serial.print(ledGloveLeft);
    Serial.print(" & contactGloveRight: ");
    Serial.print(ledGloveRight);
    Serial.print(" & contactBag: ");
    Serial.println(ledBag);

    punchLeft = contactGloveLeft == LOW && contactBag == LOW;
    punchRight = contactGloveRight == LOW && contactBag == LOW;

    setLed(ledPunchLeft, punchLeft);
    setLed(ledPunchRight, punchRight);

    if(punchLeft) {
      Serial.println("== LEFT PUNCH! ==");
      bleKeyboard.write(KEY_LEFT_ARROW);
    }
    
    if(punchRight) {
      Serial.println("== RIGHT PUNCH! ==");
      bleKeyboard.write(KEY_RIGHT_ARROW);
    }
    
    delay(100);
  } else {
    Serial.println("Not connected!");
    blinkLed(ledControl, 5, 1);
  }
}
