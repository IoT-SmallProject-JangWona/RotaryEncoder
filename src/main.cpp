#include <Arduino.h>
#include <SSD1306.h>


SSD1306            display(0x3c,4, 5, GEOMETRY_128_32);

const int pulseA=12;
const int pulseB=13;
const int pushSW=2;
const int RELAY = 15;
volatile int lastEncoded=0;
volatile long encoderValue=0;

int inputPin = A0;

IRAM_ATTR void handleRotary(){
  int MSB=digitalRead(pulseA);
  int LSB=digitalRead(pulseB);

  int encoded=(MSB<<1)|LSB;
  int sum=(lastEncoded<<2)|encoded;
  if(sum==0b1101 || sum==0b0100 || sum==0b0010 || sum==0b1011) encoderValue++;
  if(sum==0b1110 || sum==0b0111 || sum==0b0001 || sum==0b1000) encoderValue--;
  lastEncoded=encoded;
  if(encoderValue>255){
    encoderValue=255;
  }else if(encoderValue<0){
    encoderValue=0;
  }
}

IRAM_ATTR void buttonClicked(){
  Serial.println("pushed");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pushSW,INPUT_PULLUP);
  pinMode(pulseB,INPUT_PULLUP);
  pinMode(pulseB,INPUT_PULLUP);
  attachInterrupt(pushSW,buttonClicked,FALLING);
  attachInterrupt(pulseA,handleRotary,CHANGE);
  attachInterrupt(pulseB,handleRotary,CHANGE);

  pinMode(RELAY,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  int lux = analogRead(inputPin);
  display.init(); 
  display.drawString(10, 5, "Lux: "+String(lux));
  display.drawString(10, 15, "Encoder: "+String(encoderValue));
  display.display();

  if(encoderValue > lux){
    digitalWrite(RELAY,HIGH);
    delay(500);
  }
  else {
    digitalWrite(RELAY,LOW);
    delay(500);
  }
}