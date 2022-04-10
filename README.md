# ProjetCapteurGraphite2022-LM-TL
***
## Sommaire
1. [Description du projet, objectifs et livrables](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#1-description-du-projet-objectifs-et-livrables)
2. [Jauge de Déformation à base de graphite](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#2-jauge-de-d%C3%A9formation-%C3%A0-base-de-graphite)
3. [PCB Shield](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/tree/LM#3-pcb-shield)
3.1. Choix du circuit amplificateur transimpédance
3.2. Test circuit électrique sur LTSpice
3.3. Réalisation d'un PCB (KiCad)
3.4. Fabrication du PCB
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
- **un amplificateur de transimpédance**
- **un module bluetooth**
- un écran OLED
- un encodeur rotatif

Nous avons créer le **code Arduino** permettant de gérer:
- la mesure du capteur de déformation à base de graphite
- le module bluetooth
- l'affichage de l'écran OLED
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

![Solution minimaliste : amplificateur transimpédance](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/circuit1.png)

### 3.2. Test circuit électrique sur LTSpice
### 3.3. Réalisation d'un PCB (KiCad)
### 3.4. Fabrication du PCB

***
## 4. Code Arduino
***
## 5. APK Android
***
## 6. Banc de test
***
## 7. Datasheet
***
## 8. Solutions d'amélioration