#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <LiquidCrystal.h>

#define SERVOMIN  0 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  255 // this is the 'maximum' pulse length count (out of 4096)

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


int horiz = 8, vert = 9, buttonPin = 13;

//servo with motorshield interface
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const final int NUM_ITEMS = 4;
String items[NUM_ITEMS];
int itemIndex = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  initPins();
  initItems();

  pwm.begin();

  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

}

void initPins() {
  pinMode(horiz, INPUT);
  pinMode(vert, INPUT);
  pinMode(buttonPin, INPUT);
}

void initItems() {
  for (int i = 0; i < NUM_ITEMS; i++) {
    items[i] = "Item " + (String) (i + 1) + " has been selected";
  }
}

void loop() {
  int butVal = digitalRead(buttonPin);
  if (butVal == HIGH) {
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++)
      pwm.setPWM(itemIndex, 0, pulselen);
  }
  else{
	//reset pulse
      pwm.setPWM(itemIndex, 0, 0);
  }
  
    int pulseX = pulseIn(horiz, HIGH);
    int pulseY = pulseIn(vert, HIGH);

    int accX = ((pulseX / 10.0f) - 500) * 8;
    int accY = ((pulseY / 10.0f) - 500) * 8;
    
    //Top Left
    if(accX < 0 && accY < 0){
      itemIndex = 0;
    }
    //Top Right
    if(accX > 0 && accY < 0){
      itemIndex = 1;
    }
    //Bottom Left
    if(accX < 0 && accY > 0){
      itemIndex = 2;
    }
    //Bottom Right
    if(accX > 0 && accY > 0){
      itemIndex = 3;
    }

    lcd.setCursor(0, 1);
    lcd.print(items[itemIndex]);
}