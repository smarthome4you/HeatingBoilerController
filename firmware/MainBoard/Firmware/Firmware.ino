#include <Wire.h>
const int hallPin = 8; //sygnal - czarny, niebieski - GND, brązowy - VCC
int hallState = 0;

int pompaCO=2; 
int pompaCWU=3;
int pompaPDL=4;
int podajnik=5;
int nadmuch=10;
int tempKotla=11;
int tempCWU=12;
int tempSpalin=13;
int tempUjscia=14;

int state [][3] = {
                   {1,2,3},
                   {2,3,42},
                   {2,2,3}
                   };


void WireInit() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Wire.begin(8); 
  Serial.print("START\n");
}

void setup(){
  WireInit();
  digitalWrite(hallPin, HIGH);
/*
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  */
}

/*
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
*/

void loop(){
  hallState = digitalRead(hallPin);


if (hallState == HIGH){
    Serial.print(" No \n");
  }
  else{
    Serial.print(" Yes \n");
  }
}
