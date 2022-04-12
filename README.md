# ProjetCapteurGraphite2022-LM-TL
***
## Sommaire
1. [Description du projet, objectifs et livrables](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#1-description-du-projet-objectifs-et-livrables)
2. [Jauge de Déformation à base de graphite](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#2-jauge-de-d%C3%A9formation-%C3%A0-base-de-graphite)
3. [PCB Shield](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#3-pcb-shield)
- 3.1. Choix du circuit amplificateur transimpédance
- 3.2. Test circuit électrique sur LTSpice
- 3.3. Réalisation d'un PCB (KiCad)
- 3.4. Fabrication du SHIELD
- 3.4.1. Fabrication du PCB
- 3.4.2. Perçage et soudure
4. [Code Arduino](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#4-code-arduino)
5. [APK Arduino](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#5-apk-android)
6. [Banc de test](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#6-banc-de-test)
7. [Datasheet](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#7-datasheet)
8. [Solutions d'amélioration](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#8-solutions-dam%C3%A9lioration)

***
## 1. Description du projet, objectifs et livrables

### 1.1. Description du projet et objectifs
Cette UF est basée sur les travaux de chercheurs dans le domaine des capteurs afin d’évaluer les potentialités de leur technologie low-tech. Cette année, le capteur étudié est un [capteur de déformation à base de graphite](https://www.researchgate.net/publication/259846610_Pencil_Drawn_Strain_Gauges_and_Chemiresistors_on_Paper). 

### 1.2. Livrables
Pour réaliser ce projet
Nous avons réalisé le capteur graphite, un **shield PCB** associé à une carte Arduino UNO comprenant:
- **un amplificateur transimpédance**
- **un module bluetooth**
- un écran OLED
- un encodeur rotatif

Nous avons créer le **code Arduino** permettant de gérer:
- la mesure du capteur de déformation à base de graphite
- le module bluetooth
- l'affichage de l'écran OLED avec un menu
- l'encodeur rotatif pour naviguer sur l'OLED

Nous avons également créé un **APK Android** permettant d'afficher la valeur de la résistance mesurée grâce à notre shield PCB avec lequel on communique grâce au module bluetooth et au microcontrolleur Arduino UNO.

Enfin nous avons conçu un **banc de test** de notre capteur pour rédiger sa **datasheet** et pouvoir proposer des **solutions d'améliorations et les limites** pour ce type de capteur.
***

## 2. Jauge de déformation à base de graphite
Les dispositifs électroniques à base de papier sont de plus en plus étudiés pour des applications comme des antennes, des circuits imprimés, des capteurs... En effet ils sont:
- flexibles
- fins
- peu chers
- portables
- écologiques

Dans notre projet, nous utilisons du papier et du graphite comme une jauge de contrainte pour détecter des contraintes de courbure.
Le crayon à papier peut être vu comme un moyen de déposer de fines couches de particules de graphites à la surface d'une fauille de papier par friction.

Ces fines couches de particules de graphite peuvent devenir conducteurs en fonction du type de crayon utilisé et de la quantité déposée.

Dans notre cas, le type de crayon utilisé sera : **jsp**



***
## 3. PCB shield
### 3.1. Choix du circuit amplificateur transimpédance
Dans notre projet, nous souhaitons mesurer la résistance de notre capteur.
Nous décidons d'imposer une tension au borne de notre capteur et de mesurer le courant pour en déduire la résistance. 
Cependant le courant que nous mesurons est très faible du nano au micro-ampère.
Or notre microcontroleur Arduino Uno possède les caractéristiques suivantes:
- résolution 10 à 12 bits (1024 à 4096 points)
- pleine échelle (VRef) de 1.1 V à 5.0 V
- impédance de source max 1 kOhm à 10 kOhm
- fréquence d'échantillonnage max de l'ordre du MHz

La mesure directe d'un courant aussi faible que celui que fourni notre capteur ne parait pas possible.

Nous choisissons d'utiliser la solution d'un amplificateur transimpédance:
![Solution minimaliste : amplificateur transimpédance](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/circuit1.png)
Cette solution permet de :
- conversion I->U dans la résistance d'épreuve R1 (shunt)
- amplification à fort gain par montage non-inverseur
- la chute de tension sur R1 perturbe (un peu) la tension appliquée au capteur
- peut fonctionner avec seulement une alimentation positive (nécessaire avec l'utilisation d'un microcontroleur Arduino Uno)

Nous devons donc choisir un amplificateur possèdant les caractéristiques suivantes :
- faible courant d'entrée
- ultra-faible offset de tension
- supporte mode commun à la tension d'alim négative ("low rail") 

L'ampli LTC1050C satisfait toutes ces contraintes.

**Filtrage:**

Un montage aussi sensible étant vulnérable au bruit à 50Hz (+ harmoniques) résultant du couplage capacitif avec le réseau 230V, un **filtrage passe-bas est indispensable**.
D'autres sources de bruit à filtrer sont les horloges de circuits digitaux (microcontrôleur, afficheurs, etc...) et les transceivers RF (bluetooth, etc...). Ce filtrage peut être effectué en partie du coté digital, par programme, sur le microcontrôleur. Cependant un **filtrage analogique est indispensable**, en effet un excès de bruit à l'entrée de l'ADC pourrait amener celui-ci à saturation. 

Nous décidons de filtrer dans 3 régions de la chaîne de traitement analogique :
- **filtre passif à l'entrée** : permet d'éviter que les bruits HF causent de la distorsion dans les étages d'entrée
- **filtre actif basé sur les amplificateurs opérationnels** : maximum d'efficacité
- **filtre passif en fin de chaine**, coté ADC, pour retirer le bruit introduit en cours de traitement (bruit d'alimentation, bruit d'horloge des amplificateurs à découpage)

![Amplificateur transimpédance avec filtre](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/circuit2.png)

- R5 en entrée protège l'ampli opérationnel contre les décharges électrostatiques, en forme avec C1 un filtre pour les bruits en tension
- C1 avec R1 forme un filtre pour le bruit en courant
- R2 sera interchangeable, pour permettre une adaptation du calibre
- C4 avec R3 forme un filtre actif
- C2 avec R6 forme le filtre de sortie
- C3 filtre le bruit d'alimentation 

Nous allons maintenant tester ce circuit à l'aide du logiciel d'électronique analogique LTSpice.

### 3.2. Test circuit électrique sur LTSpice



### 3.3. Réalisation d'un PCB (KiCad)
- Schematique de l'OLED
![SchematiqueOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiqueOLED.png)
- Schematique ddu module bluetooth HC-O5
![SchematiqueBluetooth](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiqueModuleBluetooth.png)
- Schematique de l'encodeur rotatif KY-040
![SchematiqueEncodeur](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiqueEncodeurRotatif.png)
- Schematique de l'amplificateur opérationel LTC1050
![SchematiqueAmpliOp](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiqueAmpliOp.png)
- Schematique des pins de la carte Arduino
![SchematiquePinArduino](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiquePinArduino.png)
- Schematique du circuit amplificateur transimpédance
![SchematiqueCircuitKicad](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiqueCircuitKicad.png)
- Empreinte de l'OLED
![EmpreinteOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/EmpreinteOLED.png)
- Empreinte du module Bluetooth HC-05
![EmpreinteBluetooth](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/EmpreinteBluetooth.png)
- Empreinte de l'encodeur rotatif KY-040
![EmpreinteEncodeur](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/EmpreinteEncodeur.png)
- Empreinte de l'amplificateur opérationel LTC1050
![EmpreinteAmpliOp](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/EmpreinteAmpliOp.png)
- Empreinte du PCB complet
![EmpreintePCB](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/EmpreintePCB.png)
- Visualisation 3D du PCB
![Visu3D](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/Visu3D.png)


### 3.4. Fabrication du SHIELD
#### 3.4.1. Fabrication du PCB
Le PCB est fabriqué sur une plaquette recouverte d'une fine couche de cuivre elle même recouverte d'une couche de résine.
Nous avons tout d'abord imprimé notre modélisation du circuit sur un film plastique qui nous sert de calque. Une insolation sous UV pendant 2 minutes permet de transposer ce calque sur notre couche d'epoxy, permettant ainsi d'enlever la partie de la résine insolée.
Ensuite, la plaquette a été déposée dans un bain de perchlorure de fer afin d'être gravée (environ 8 minutes). Seule la partie du cuivre non protégée par la résine est décapée.
Pour finir, de l'acétone a été appliquée sur la plaquette pour éliminer les dernières traces de résine.

## 3.4.2. Perçage et soudure
Une fois le PCB réalisé, nous sommes passés au perçage de trous sur notre plaquette à l'aide d'une perçeuse électrique afin d'insérer nos composants :

0.8mm de diamètre pour l'AOP, les résistances et les capacités
1mm de diamètre pour les connecteurs de l'Arduino et des headers des différents modules (pinces du capteur, bluetooth, encodeur rotatoire, écran OLED)
Enfin, nous avons soudé chaque composant à l'aide d'un fer à souder.
***
## 4. Code Arduino
Voici le code arduino qui permet de 
```
  // Encodeur Rotatif

#define encoder0PinA  3  //CLK Output A Do not use other pin for clock as we are using interrupt
#define encoder0PinB  4  //DT Output B
#define Switch 2 // Switch connection if available
volatile unsigned int encoder0Menu_temp = 0;
volatile unsigned int encoder0Menu = 0;
volatile unsigned int Menu_State = 0 ;
volatile unsigned int action_encoder = 0; // retient quelle action effectuer pour l'encodeur
volatile unsigned int temps_acquisition_temp = 0;
volatile unsigned int temps_acquisition = 0;
volatile unsigned int val_R2_temp = 0;
volatile unsigned int val_R2 = 0;
int val_choisie = 0;
int temps_choisi = 0;
int State;
int Old_State;

  // Oled 

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };


  // Bluetooth

#include <SoftwareSerial.h>
#define rxPin 10 //Broche 10 en tant que RX, � raccorder sur TX du HC-05
#define txPin 11 //Broche 11 en tant que RX, � raccorder sur TX du HC-05
#define baudrate 9600 
SoftwareSerial Serial_Phone(rxPin ,txPin); //D�finition du software serial

  // Transimp

#define ADCpin  0

  // Global variables 

float resistance = 0;
float tension;
int R1 = 100;
int R2 = 22;
int R3 = 100;
int R5 = 10;
int voltage_ADC;


  // Fonctions Encodeur

// doEncoder increments or decrements the variable encoder0Menu that is used to navigate into menues, between 4 of them, also used to increments the "temps_acquisition" variable
void doEncoder() {

  State = digitalRead(encoder0PinA);

  if (action_encoder == 0) {
  
    if (State != Old_State)
    {
      if (digitalRead(encoder0PinB) != State)
      {
        encoder0Menu_temp = (encoder0Menu_temp + 1) % 8;
        encoder0Menu = encoder0Menu_temp/2;
      }
      else {
        encoder0Menu_temp = (encoder0Menu_temp - 1) % 8;
        encoder0Menu = encoder0Menu_temp/2;
      }
    }

  }

  else if ((action_encoder == 1) and (temps_choisi == 0) and (Menu_State == 2)) {

    if (State != Old_State)
    {
      if (digitalRead(encoder0PinB) != State)
      {
        temps_acquisition_temp = (temps_acquisition_temp + 1) % 60;
        temps_acquisition = temps_acquisition_temp / 2;
      }
      else {
        temps_acquisition_temp = (temps_acquisition_temp - 1) % 60;
        temps_acquisition = temps_acquisition_temp / 2;
      }
    }
    
  }

  else if ((action_encoder == 1) and (val_choisie == 0) and (Menu_State == 3)) {

    if (State != Old_State)
    {
      if (digitalRead(encoder0PinB) != State)
      {
        val_R2_temp = (val_R2_temp + 1) % 100;
        val_R2 = val_R2_temp / 2;
      }
      else {
        val_R2_temp = (val_R2_temp - 1) % 100;
        val_R2 = val_R2_temp / 2;
      }
    }
  }
}


// doEncoderButton is activated when the button is clicked and affects a variable Menu_State with the encoder0Menu value, also checks if the time is chosen when the third menu is activated
void doEncoderButton() {

  if (action_encoder == 0) {
    Menu_State = encoder0Menu ;
    action_encoder = 1 ;
    }

  else if ((action_encoder == 1) and (Menu_State == 2)) {

    if (temps_choisi == 0) {
      temps_choisi = 1;
    }

    else if (temps_choisi == 1) {
      action_encoder = 0 ;
      temps_choisi = 0;
    }
  }

  else if ((action_encoder == 1) and (Menu_State == 3)) {

    if (val_choisie == 0) {
      val_choisie = 1;
    }

    else if (val_choisie == 1) {
      action_encoder = 0 ;
      val_choisie = 0;
      R2 = val_R2;
    }
  }

  else if (action_encoder == 1) {
    action_encoder = 0 ;
  }

  delay(200);
}

  // Fonctions de simplification

void setup_display() {

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  
}


void setup() {

  Serial.begin(9600);

  // encodeur rotatoire

  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor

  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor

  pinMode(Switch, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(3), doEncoder, RISING); 
  attachInterrupt(digitalPinToInterrupt(2), doEncoderButton, FALLING); 


  // Bluetooth

  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  Serial_Phone.begin(baudrate);

  // Oled

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  
} 



void loop() {

  // Encodeur et OLED -> affichage

  if (action_encoder == 0) {
  
    if (encoder0Menu == 0) {
      // affiche les actions possibles du menu choisi
      setup_display();
      display.println(F("MENU 0 : \nReleve de resistance"));
      display.display();
    }
  
    else if (encoder0Menu == 1) {
      setup_display();
      display.println(F("MENU 1 : \nReleve de tension"));
      display.display();
    }
  
    else if (encoder0Menu == 2) {
      setup_display();
      display.println(F("MENU 2 : \nDefinir le temps \nd'aquisition"));
      display.display();
    }
  }

  // Encodeur et OLED -> action
  
  else if (action_encoder == 1) {
 
    if (Menu_State == 0) {
    
        // Relevé ADC
      voltage_ADC = analogRead(ADCpin) / 4;
        // Bluetooth
      Serial_Phone.write(voltage_ADC);
      delay(500);
      Serial.print(voltage_ADC + String(" "));
      Serial.println(millis());

      resistance = (R1 * (1 + (R3/R2)) * (5 / (voltage_ADC*0.01952)) - R1 - R5) / 1000;

      setup_display();
      display.println("Resistance : ");
      if (resistance != 0) {
        display.print(resistance);
        display.println(" MOhm  - Envoi BT");
      }
      else {
        display.print("Infini");
        display.println("   - Envoi BT");
      }
      display.println("\n        Press to quit");
      display.display();
      
    
    }
  
    else if (Menu_State == 1) {

        // Relevé ADC
      voltage_ADC = analogRead(ADCpin) / 4;
        // Bluetooth
      Serial_Phone.write(voltage_ADC);

      tension = voltage_ADC * 0.01952;
      
      setup_display();
      display.println("Tension : ");
      display.print(tension);
      display.println(" V   - Envoi BT");
      display.println("\n        Press to quit");
      display.display();
      
    }

    else if (Menu_State == 2) {

      if (temps_choisi == 0) {

        setup_display();
        display.println("Temps d'acquisition :");
        display.print(temps_acquisition);
        display.println(" s");
        display.println("\n       Press to chose");
        display.display();

      }

      else if (temps_choisi == 1) {

        setup_display();
        display.println("Temps d'acquisition :");
        display.print(temps_acquisition);
        display.println(" s   - Envoi BT");
        display.println("\n        Press to quit");
        display.display();

        Serial_Phone.write(temps_acquisition);

      }
    
    }

  }

}
```
***
## 5. APK Android

***
## 6. Banc de test
***
## 7. Datasheet
***
## 8. Solutions d'amélioration
***