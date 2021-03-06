# ProjetCapteurGraphite2022-LM-TL
***
## Sommaire
1. [Description du projet, objectifs et livrables](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM#1-description-du-projet-objectifs-et-livrables)
2. [Jauge de Déformation à base de graphite](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM#2-jauge-de-d%C3%A9formation-%C3%A0-base-de-graphite)
3. [PCB Shield](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM#3-pcb-shield)
- 3.1. Choix du circuit amplificateur transimpédance
- 3.2. Test circuit électrique sur LTSpice
- 3.3. Réalisation d'un PCB (KiCad)
- 3.4. Fabrication du SHIELD
- 3.4.1. Fabrication du PCB
- 3.4.2. Perçage et soudure
4. [Code Arduino](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM#4-code-arduino)
5. [APK Arduino](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM#5-apk-android)
6. [Banc de test](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM#6-banc-de-test)
7. [Datasheet](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM#7-datasheet)
8. [Solutions d'amélioration](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM#8-solutions-dam%C3%A9lioration)

***
## 1. Description du projet, objectifs et livrables

### 1.1. Description du projet et objectifs
Ce projet est inscrit dans le cadre de l'UF "Du capteur au banc de test en open source hardware". Il est basée sur les travaux de chercheurs dans le domaine des capteurs afin d’évaluer les potentialités de leur technologie low-tech. Cette année, le capteur étudié est un [capteur de déformation à base de graphite](https://www.researchgate.net/publication/259846610_Pencil_Drawn_Strain_Gauges_and_Chemiresistors_on_Paper). 
Pour réaliser ce projet, il faut comprendre les propriétés physiques du capteur, concevoir et réaliser un système d'acquisition, élaborer un banc de test pour notre capteur pour enfin rédiger la datasheet de ce capteur.
### 1.2. Livrables
Les livrables de ce projet sont donc: 
- un **shield PCB** associé à une carte Arduino UNO comprenant:
  - **un circuit analogique amplificateur transimpédance**
  - **un module bluetooth** permettant de transmettre les mesures sur un téléphone
  - un écran OLED permettant de voir les mesures en temps réel
  - un encodeur rotatif permettant de naviguer dans le menu de l'écran OLED
- le **code Arduino** permettant:
  - la mesure de notre capteur de déformation à base de graphite
  - d'envoyer les données via un module bluetooth
  - l'affichage sur un écran OLED
  - l'encodeur rotatif permettant de naviguer dans le menu de l'OLED

Nous avons également créé un **APK Android** permettant d'afficher la valeur de la résistance mesurée sur un smartphone.

Enfin nous avons conçu un **banc de test** pour notre capteur afin de rédiger une **datasheet** associée et pouvoir proposer des **solutions d'améliorations et les limites** pour ce type de capteur.
***

## 2. Jauge de déformation à base de graphite
Les dispositifs électroniques à base de papier sont de plus en plus étudiés pour des applications comme des antennes, des circuits imprimés, des capteurs... En effet ils sont:
- flexibles
- fins
- peu chers
- portables
- écologiques

Quand on trace un trait au crayon sur du papier, la friction entre le crayon et le papier arrache des particules de graphite qui adhèrent sur les fibres de papier. Ainsi les crayons à papier sont un bon moyen de déposer de fines particules de graphite sur un support.
Ces traces de crayon peuvent être vues comme de fins films conducteurs fait de réseaus de particules de graphite.

Les différents types de crayon à papier varie en fonction de la composition de leurs mines. Les mines vont du 9H, les plus durs, jusqu'au 9B, les moins dur. Plus les mines sont dures, moins il y a de carbone dans leur composition, moins le film de graphite qu'elles déposent est conducteur.
Ainsi, le 9B est plus conducteur que le 9H.
Dans notre projet, nous avons utilisé des crayons **HB et B**.

Le principe physique du capteur repose sur l'organisation en **système granulaire** du réseau des nanoparticules de graphite sur le papier.

![Système granulaire](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_README/granulaire.png)

En effet, la **conductance d'un système granulaire** varie en fonction :
- des **distances internanoparticules**
- de la constante de l'effet tunnel
- de l'énergie de charge de Coulomb
- de la température

Ici, la compression et l'extension de la feuille de papier peut modifier la distance entre les Nano particules de graphite. La compression rapproche les particules de graphite, augmente la conductance, diminue la résistivité. Au contraire, l'extension de la feuille de papier Augmente la distance entre les particules de graphite, diminue la conductance, augmente la résistance.
Ainsi, en mesurant la variation de la résistance entre les deux électrodes de notre capteur, nous pouvons en déduire la déformation de notre capteur.

***
## 3. PCB shield
### 3.1. Choix du circuit amplificateur transimpédance
Dans notre projet, nous souhaitons mesurer la résistance de notre capteur.
Nous décidons d'imposer une tension au borne de notre capteur et de mesurer le courant pour en déduire la résistance. 
Cependant le courant que nous devons mesurer est très faible du nano au micro-ampère.
Or notre microcontroleur Arduino Uno possède les caractéristiques suivantes:
- résolution 10 à 12 bits (1024 à 4096 points)
- pleine échelle (VRef) de 1.1 V à 5.0 V
- impédance de source max 1 kOhm à 10 kOhm
- fréquence d'échantillonnage max de l'ordre du MHz

La mesure directe d'un courant aussi faible que celui que fourni notre capteur ne parait pas possible.

Nous choisissons d'utiliser la solution d'un amplificateur transimpédance:
![Solution minimaliste : amplificateur transimpédance](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_README/circuit1.png)
Cette solution permet :
- la conversion courant I -> tension U dans la résistance d'épreuve R1 (shunt)
- l' amplification à fort gain par montage non-inverseur
- la chute de tension sur R1 perturbe (un peu) la tension appliquée au capteur
- peut fonctionner avec seulement une alimentation positive (nécessaire avec l'utilisation d'un microcontroleur Arduino Uno)

Gain de l’étage de l’AOP : (R2+R3)/R2=(1k+500k)/1k=501

Tension aux bornes de R1 : le courant Isens est d’environ 100nA 
 -> V = 100kOhm*100nA=10-2V=10mV

 Tension sur l’ADC : Gain*Tension aux bornes de R1 => 501*10x10-3~5V 

Nous devons donc choisir un amplificateur possèdant les caractéristiques suivantes :
- un **faible courant d'entrée**: doit être très petit devant Isens pour avoir le Isens traverse la résistance R1 et
pas l’entrée 
- un **ultra-faible offset de tension** : c'est une erreur constante aux entrées de l’amplificateur différentiel dû à un
état non compensé sur l’étage d’entrée de l’amplificateur. On veut que cet offset soit
négligeable devant la tension mesurée sur R1 qui sera de 10mV. 
- **doit supporter un mode commun à la tension d'alimentation négative** ("low rail") : tension commune aux deux entrées, un bon amplifieur sera insensible
à ce mode commun. Si le Isens =0A ,on veut 0V sur l’ADC. LTC1050C => 5µV offset
voltage, et maximum offset voltage drift : 0.05µV/°C. Bien comparé aux 10mV aux
bornes de R1. 

L'[amplificateur opérationel LTC1050C](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Datasheet_LTC1050/Datasheet_LTC1050.pdf) satisfait bien toutes ces contraintes.

**Filtrage:**

Cependant un montage aussi sensible étant vulnérable au bruit à 50Hz (+ harmoniques) résultant du couplage capacitif avec le réseau 230V, un **filtrage passe-bas est indispensable**.
D'autres sources de bruit à filtrer sont les horloges de circuits digitaux (microcontrôleur, afficheurs, etc...) et les transceivers RF (bluetooth, etc...). Ce filtrage peut être effectué en partie du coté digital, par programme, sur le microcontrôleur. Cependant un **filtrage analogique est indispensable**, en effet un excès de bruit à l'entrée de l'ADC pourrait amener celui-ci à saturation. 

Nous décidons de filtrer dans 3 régions de la chaîne de traitement analogique :
- **filtre passif à l'entrée** : permet d'éviter que les bruits HF causent de la distorsion dans les étages d'entrée
- **filtre actif basé sur les amplificateurs opérationnels** : maximum d'efficacité
- **filtre passif en fin de chaine**, coté ADC, pour retirer le bruit introduit en cours de traitement (bruit d'alimentation, bruit d'horloge des amplificateurs à découpage)

![Amplificateur transimpédance avec filtre](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_README/circuit2.png)

- **R5** en entrée protège l'ampli opérationnel contre les décharges électrostatiques, en forme avec C1 un filtre pour les bruits en tension
- **C1 avec R1** forme un **filtre pour le bruit en courant**
- **C4 avec R3** forme un **filtre actif**
- **C2 avec R6** forme le **filtre de sortie**
- **C3** filtre le bruit d'alimentation 

Nous allons maintenant tester ce circuit à l'aide du logiciel d'électronique analogique LTSpice.

### 3.2. Test circuit électrique sur LTSpice
Nous réalisons un schéma de notre circuit d'amplification transimpédance sur LTSpice pour simuler son comportement.
![SchematiqueOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_README/circuit3.png)
#### Test fonctionalité dans les conditions nominales
![SchematiqueOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_LTSpice/gain.jpg)
Dans les conditions nominales, nous obtenons bien un gain de 100 en sortie du circuit amplificateur.
![SchematiqueOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_LTSpice/courantentrée.jpg)
Vérification de la tension de sortie avec un pulse de courant d'entrée à 100 nA.
#### Incidence de l'offset de l'amplificateur
La [datasheet de l'amplificateur opérationel LTC1050C](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Datasheet_LTC1050/Datasheet_LTC1050.pdf)
nous indique que l'offset de tension en sortie de l'amplificateur est de 5uV. On veut que cet offset soit
négligeable devant la tension mesurée sur R1 qui sera de 10mV. 
L'incidence de l'offset de cet amplificateur est donc acceptable.
#### Incidence du courant d'entrée de l'amplificateur
La [datasheet de l'amplificateur opérationel LTC1050C](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Datasheet_LTC1050/Datasheet_LTC1050.pdf)
nous indique que l'offset de courant en entrée de l'amplificateur est de +/- 200 pA, bien inférieur à 100nA. 
On a donc un faible courant d'entrée, très petit devant Isens pour avoir le Isens qui traverse la résistance R1 et
pas l’entrée de l'amplificatieur.
L'incidence de l'offset de cet amplificateur est donc acceptable.
#### Fréquence de coupure de chaque étage de filtre passe-bas
##### Etage C1-R1: Filtre pour le bruit en courant
![SchematiqueOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_LTSpice/C1R1.png)
- fc = 16 Hz
##### Etage C4-R3: Filtre actif
![SchematiqueOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_LTSpice/C4R3.png)
- fc = 1,6 Hz
##### Etage C2-R6: Filtre de sortie
![SchematiqueOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_LTSpice/C2R6.png)
- fc = 1,6 kHz
#### Atténaution globale d'un bruit en courant à la fréquence de limite de repliement dans le cas de l'ADC Arduino UNO
![SchematiqueOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_LTSpice/repliement.jpg)
On constate qu'il y a une atténuation d'un bruit en courant environ égale à **40dB pour une fréquence autour de 50Hz** et de **100 dB pour un bruit en courant au voisinage de la fréquence de repliement de spectre** (7,7 kHz).
#### Expression analytique de la résistance du capteur en fonction des éléments du circuit transimpédance
R1 = 100 000 Ohm
R2 = 1000 Ohm
R3 = 100 000 Ohm
R4 = 10 000 Ohm
Vcc = 5 V

-> Rsens (Ohm) = (1+R3/R2).R1.(Vcc/(Vadc.5/1024))-R1-R4

### 3.3. Réalisation d'un PCB (KiCad)
Pour réaliser les schématiques et les empreintes de de nos composants, nous nous servons de leurs [datasheets](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Datasheets_Compsants) en ligne. 
Pour faire le PCB, nous avons respecté quelques règles :
- Largeur de piste: 0.6 mm minimum,
- Isolation de piste : 0.9mm minimum
- Bluetooth, Encodeur rotatif, OLED, Connecteurs (header): pad à 1mm de percage, Ovale 2*2.54mm
- Résistance, Capacité, Support d'AOP: Pad à 0.8mm de percage, Cercle 2.54mm
- Support de l'AOP: pad n°1 rectangulaire, les autres sont ovales ou ronds

#### Schematique et empreinte de l'OLED
![SchematiqueOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/SchematiqueOLED.png) ![EmpreinteOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/EmpreinteOLED.png)
#### Schematique et empreinte du module bluetooth HC-O5
![SchematiqueBluetooth](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/SchematiqueModuleBluetooth.png) ![EmpreinteBluetooth](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/EmpreinteBluetooth.png)
#### Schematique et empreinte de l'encodeur rotatif KY-040
![SchematiqueEncodeur](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/SchematiqueEncodeurRotatif.png) ![EmpreinteEncodeur](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/EmpreinteEncodeur.png)
#### Schematique et empreinte l'amplificateur opérationel LTC1050
![SchematiqueAmpliOp](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/SchematiqueAmpliOp.png) ![EmpreinteAmpliOp](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/EmpreinteAmpliOp.png)
#### Schematique et empreinte des pins de la carte Arduino
![SchematiquePinArduino](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/SchematiquePinArduino.png)
#### Schematique du circuit amplificateur transimpédance
![SchematiqueCircuitKicad](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/SchematiqueCircuitKicad.png)


#### Empreinte du PCB complet
Nous avons utilisé l'empreinte d'une carte Arduino Uno existante dans les bibliothèques de KiCad, sur laquelle nous avons placé nos composants. 
Les masses (GND) sont toutes reliées entre elles par une piscine ce qui permet une meilleure homogénéité des équipotentiels sur tout le circuit, pour de meilleurs résultats à haute fréquence.
Il faut faire attention de ne par dessiner de pistes avec des angles droits pour éviter d'éventuels problèmes lors de la fabrication du PCB. 

Enfin, nous avons soudé chaque composant à l'aide d'un fer à souder.
![EmpreintePCB](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/EmpreintePCB.png)

#### **Visualisation 3D du PCB entier**
![Visu3D](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Photos_KiCad/Visu3D.png)


### 3.4. Fabrication du SHIELD
#### 3.4.1. Fabrication du PCB
Le PCB est fabriqué sur une plaquette recouverte d'une fine couche de cuivre, elle même recouverte d'une couche de résine photosensible aux UV.
Nous avons tout d'abord imprimé notre modélisation du circuit sur un film plastique qui nous sert de calque. 
![Gerber](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Schield/PDF/Calque.png)
Une insolation sous UV pendant 2 minutes permet de transposer ce calque sur notre couche de résine, permettant ainsi d'enlever la partie de la résine insolée.
Ensuite, la plaquette a été déposée dans un bain de perchlorure de fer afin d'être gravée (environ 8 minutes). Seule la partie du cuivre non protégée par la résine est décapée.
Pour finir, de l'acétone a été appliquée sur la plaquette pour éliminer les dernières traces de résine.
## 3.4.2. Perçage et soudure
Une fois le circuit en cuivre du PCB réalisé, nous avons percé les trous sur notre plaquette à l'aide d'une perçeuse électrique. 
***
## 4. Code Arduino
Voici le code arduino qui permet d'acquérir les mesures de notre capteur, les afficher sur l'écran OLED et les envoyer par bluetooth à un smatrphone.
```
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
 
  float Res=((1+100000/1000)*100000*(5/Volt)-100000-10000)/1000000;
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
```
***
## 5. APK Android
![block](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/APK_CapteurGraphite/APKblock.png)
![telephone](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/APK_CapteurGraphite/APKtel.png)
***
## 6. Banc de test
Nous avons élaborer un banc de test à l'aide d'un servomoteur, du scotch double face et des morceaux de carton et plastique d'empballage. 
Une partie du capteur est fixée sur le stator à l'aide d'un morceau de carton et d'un scotch double face, et l'autre partie du capteur est maitenue dans l'axe du rotor à l'aide d'un morceau de plastique et de scotch double face.

![Video Banc de test](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/ResultatsBancTest/VideoBanc.mp4)

### Premier test : DeltaR/R0 HB Compression
Pour réaliser cette partie, nous avons mesurer 5 cycles de compression que nous avons moyenner pour obtenir la variation de résistance relative en fonction de l'angle appliqué entre les deux extrémités du capteur.
![HBC](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/ResultatsBancTest/HBCompression.png)

### Second test : DeltaR/R0 HB Extension
Pour réaliser cette partie, nous avons mesurer 5 cycles d'extension que nous avons moyenner pour obtenir la variation de résistance relative en fonction de l'angle appliqué entre les deux extrémités du capteur.
![HBE](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/ResultatsBancTest/HBExtension.png)

### Troisième test : DeltaR/R0 B Compression
Pour réaliser cette partie, nous avons mesurer 5 cycles de compression que nous avons moyenner pour obtenir variation de la résistance relative en fonction de l'angle appliqué entre les deux extrémités du capteur.
![BC](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/ResultatsBancTest/BCompression.png)

### Quatrième test : DeltaR/R0 B Extension
Pour réaliser cette partie, nous avons mesurer 5 cycles d'extension que nous avons moyenner pour obtenir la variation de résistance relative en fonction de l'angle appliqué entre les deux extrémités du capteur.
![BE](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/ResultatsBancTest/BExtension.png)

### Cinquième test : Répetabilité des mesures HB
Pour réaliser cette partie, nous effectuons plusieurs cycles de compression-déflexion jusqu'à obtenir des valeurs divergentes des premières de plus de 10%.
![HBR](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/ResultatsBancTest/répétabilitéHB.png)
Nous pouvons voir qu'à partir de à peu près 260 cycles, le capteur ne peut plus garantir de bons résultats en deflexion.
Les mesures en compression restent valables au dela.

### Sixième test : Répetabilité des mesures B
Pour réaliser cette partie, nous effectuons plusieurs cycles de compression-déflexion jusqu'à obtenir des valeurs divergentes des premières de plus de 10%.
![BR](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/ResultatsBancTest/répétabilitéB.png)
Nous pouvons voir qu'à partir d'une centaine de cycles, le capteur ne peut plus garantir de bons résultats en deflexion.
Les mesures en compression restent valables au dela.

***
## 7. Datasheet
Voici la [datasheet](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/main/Datasheet_CapteurGraphite/Datasheet_CapteurGraphite.pdf) de notre capteur.
***
## 8. Solutions d'amélioration
### Solutions d'amélioration
- Trouver une méthode pour déposer la même quantité de graphite sur chaque capteur testé
- Anticiper les besoins du banc de test dans la conception du PCB
- Trouver un moyen de fixer les pinces sur les electrodes lors des mesures
***