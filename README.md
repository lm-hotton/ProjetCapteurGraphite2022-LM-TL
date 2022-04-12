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

![SchematiqueOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiqueOLED.png)
![SchematiqueBluetooth](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiqueModuleBluetooth.png)
![SchematiqueEncodeur](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiqueEncodeurRotatif.png)
![SchematiqueAmpliOp](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiqueAmpliOp.png)
![SchematiquePinArduino](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiquePinArduino.png)
![SchematiqueCircuitKicad](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/SchematiqueCircuit.png)
![EmpreinteOLED](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/EmpreinteOLED.png)
![EmpreinteBluetooth](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/EmpreinteBluetooth.png)
![EmpreinteEncodeur](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/EmpreinteEncodeur.png)
![EmpreinteAmpliOp](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/EmpreinteAmpliOp.png)
![EmpreintePCB](https://github.com/lm-hotton/ProjetCapteurGraphite2022-LM-TL/blob/LM/PhotoREADME/EmpreintePCB.png)


### 3.4. Fabrication du SHIELD
#### 3.4.1. Fabrication du PCB
Le PCB est fabriqué sur une plaquette d'epoxy recouverte d'une fine couche de cuivre.
Nous avons tout d'abord imprimé notre modélisation du circuit sur unun film plastique qui nous sertde calque. Une insolation sous UV pendant 2 minutes permet de transposer ce calque sur notre couche d'epoxy, permettant ainsi de déposer une résine protectrice, qui grâce à un révélateur, a pu être retirée aux endroits désirés.
Ensuite, la plaquette a été déposée dans un bain de perchlorure de fer afin d'être gravée (environ 8 minutes). A la suite de cela, le cuivre non protégé par la résine a alors été retiré, permettant d'obtenir le PCB modélisé en amont.
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
mettre le code arduino
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