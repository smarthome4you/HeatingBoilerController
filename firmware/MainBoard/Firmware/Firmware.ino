#include <Wire.h>
const int hallPin = 8; //sygnal - czarny, niebieski - GND, brązowy - VCC
int hallState = 0;

int relay1=2; 
int relay2=3;
int relay3=4;
int relay4=5;


void WireInit() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Wire.begin(8); 
  Serial.print("START\n");
}

void setup(){
  WireInit();
  digitalWrite(hallPin, HIGH);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
}


void test_relay() {
  delay(2000); 
  digitalWrite(relay1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000); 
  digitalWrite(relay2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000); 
  digitalWrite(relay3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000); 
  digitalWrite(relay4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10000);
  digitalWrite(relay1, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(relay2, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(relay3, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(relay4, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(10000);
}

void loop(){
  hallState = digitalRead(hallPin);


if (hallState == HIGH){
    Serial.print(" No \n");
  }
  else{
    Serial.print(" Yes \n");
  }
}

