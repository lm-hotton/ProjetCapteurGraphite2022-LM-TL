//code Capteur
const int flexPin=A0;

//code bluetooth
#include <SoftwareSerial.h>
#define rxPin 11
#define txPin 10
#define baudrate 9600
SoftwareSerial mySerial(rxPin, txPin);

//code OLED
#include <Adafruit_SSD1306.h>
#define nombreDePixelsEnLargeur 128                             // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64                              // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED -1                                      // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED 0x3C                                // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

//code encodeur
#define encoder0PinA 2          // CLK pin
#define encoder0PinB 3          // DT pin
#define Switch 4                // SW pin

volatile unsigned int encoder0Pos = 0;
volatile unsigned int old_encoder0Pos = 0;
volatile unsigned int menu = 1;
volatile unsigned int swState = 0;
volatile unsigned int swLast = 0;
volatile unsigned int nombre_click = 0;
volatile unsigned int old_nombre_click = 0;
bool derouler=false;
bool clk=false;



void doEncoder() {
if (digitalRead(encoder0PinB)==HIGH){
encoder0Pos++;
}
else{
encoder0Pos--;
}
}


int Menu() {
if (Derouler()){
if (menu==1){
menu=2;
}
else if (menu==2){
menu=3;
}
else if (menu==3){
menu=1;
}
else if (menu==11||menu==22||menu==33) {
menu=1;
}
}

if (menu==1 && Clk()){
menu=11;
}
else if (menu==2 && Clk()){
menu=22;
}
else if (menu==3 && Clk()){
menu=33;
}

return menu;
}



bool Clk(){
swState = digitalRead(Switch);
if (swState == LOW && swLast == HIGH) {
nombre_click++;
delay(100);//debounce
}
swLast = swState;

if (nombre_click != old_nombre_click){
clk=true;
}
else{
clk=false;
}
old_nombre_click=nombre_click;
return clk;
}



bool Derouler(){
if (digitalRead(encoder0PinB)==HIGH){
encoder0Pos++;
}
else{
encoder0Pos--;

if (encoder0Pos != old_encoder0Pos){
  derouler=true;
}
old_encoder0Pos=encoder0Pos;
return derouler;
}
}



void setup() {

//code bluetooth
pinMode (rxPin, INPUT);
pinMode (txPin, OUTPUT);
Serial.begin(baudrate);
mySerial.begin(baudrate);

//code capteur
pinMode(flexPin,INPUT);

//code Encodeur
pinMode(encoder0PinA, INPUT);
digitalWrite(encoder0PinA, HIGH);

pinMode(encoder0PinB, INPUT);
digitalWrite(encoder0PinB, HIGH);

attachInterrupt(0, doEncoder, RISING);


//code OLED
if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
while(1); // Arrêt du programme (boucle infinie) si échec d'initialisation

ecranOLED.clearDisplay();
ecranOLED.setCursor(0, 0);
ecranOLED.setTextColor(SSD1306_WHITE);
ecranOLED.setTextSize(2);
}

 









void loop() {

//code capteur + moniteur série ///
float ADCflex=analogRead(flexPin);
Serial.println(String(ADCflex));
Serial.println();

float Volt=(ADCflex*5/1023);
Serial.println(String(Volt));
Serial.println();

float Res=((1+100000/1000)*100000*(1023/Volt)-100000-10000)/1000000000;
Serial.println(String(Res));
Serial.println();


// code bluetooth ///
mySerial.print(Res);


// OLED ///
ecranOLED.setCursor(0, 0);

if (Menu()==1){
ecranOLED.print("Mesure tension \n Cliquez pour pour voir la valeur + BT");
}
else if (Menu()==2){
ecranOLED.print("Mesure résistance \n Cliquez pour pour voir la valeur + BT");
}
else if (Menu()==3){
ecranOLED.print("Cliquez pour pour voir les infos du capteur et la fréquence");
}
else if (Menu()==11){
ecranOLED.print(Volt);
ecranOLED.print(" V \n");
}
else if (Menu()==22){
ecranOLED.print(Res);
ecranOLED.print(" MOhm \n");
}
else if (Menu()==33){
ecranOLED.print("Infos capteur \n Résistance: 1-100MO \n Tension: 0-5V\n Delay: ");
ecranOLED.print(100);
}

ecranOLED.display(); // Transfert le buffer à l'écran
delay(1000);
ecranOLED.clearDisplay();
}
