#include<SPI.h>
const byte csPin=10;
const int maxPositions=256;
const long rAB=50000;
const byte rWiper=125;
const byte pot0=0x11;
const byte post0Shutdown=0x21;
double voltage;
double resistance;

#include <Servo.h>
Servo myservo;
int posmoteur=0;

 

void setPotWiper(int addr,int pos){
pos=constrain(pos,0,255);
digitalWrite(csPin, LOW);
SPI.transfer(addr);
SPI.transfer(pos);
digitalWrite(csPin, HIGH);


for(posmoteur=0;posmoteur<=180;posmoteur+=1){
myservo.write(posmoteur);
voltage=analogRead(A0);

float angle=map(resistance, 0, 100000000, 0,90.0);
Serial.println("Bend:" +String(angle) + "degrees");
Serial.println();
delay(1);
}
}


void setup() {
Serial.begin(9600);

digitalWrite(csPin, HIGH);
pinMode(csPin, OUTPUT);
SPI.begin();

myservo.attach(9);
}

void loop() {
setPotWiper(pot0,5);
delay(1000);

}
