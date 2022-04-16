//code Capteur
int flexPin=A0;

//code bluetooth
#include <SoftwareSerial.h>
#define rxPin 11
#define txPin 10
#define baudrate 9600
SoftwareSerial mySerial(rxPin, txPin);

//code OLED
#include <Adafruit_SSD1306.h>
#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

//code encodeur
int encoderPinA = 2; // CLK pin
int encoderPinB = 3; // DT pin
int encoderBtn = 4; // SW pin
int count = 0;
int encoderPinA_prev;
int encoderPinA_value;
boolean bool_CW;


void setup() {

  //code bluetooth
  pinMode (rxPin, INPUT);
  pinMode (txPin, OUTPUT);
  Serial.begin(baudrate);
  mySerial.begin(baudrate);

  //code capteur
  pinMode(flexPin,INPUT);

  //code Encodeur
  pinMode (encoderPinA, INPUT);
  pinMode (encoderPinB, INPUT);
  pinMode(encoderBtn, INPUT_PULLUP);
  encoderPinA_prev = digitalRead(encoderPinA);

  //code OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation

   ecranOLED.clearDisplay();
   ecranOLED.setCursor(0, 0);
   ecranOLED.setTextColor(SSD1306_WHITE);
   ecranOLED.setTextSize(2);
}


void loop() {

  //code capteur
  float ADCflex=analogRead(flexPin);
  //Serial.println(String(ADCflex));
  //Serial.println();
  
  float Volt=(ADCflex*5/1023);
  //Serial.println(String(Volt));
  //Serial.println();
 
  float Res=((1+100000/1000)*100000*(1023/Volt)-100000-10000)/1000000000;
  Serial.println(String(Res));
  //Serial.println();

  // code bluetooth
  mySerial.print(Res);


   // OLED
   ecranOLED.setCursor(0, 0);
   ecranOLED.print(Volt);
   ecranOLED.print(" V \n");
   ecranOLED.print(Res);
   ecranOLED.print(" MOhm \n");
   ecranOLED.display();                            // Transfert le buffer à l'écran
   delay(1000);
   ecranOLED.clearDisplay();
}
