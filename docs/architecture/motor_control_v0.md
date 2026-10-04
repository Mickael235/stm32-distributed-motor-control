# Architecture commande moteur V0

## Objectif

## Chaîne fonctionnelle
Encodeur
↓
Acquisition vitesse
↓
Calcul RPM
↓
Erreur = consigne - mesure
↓
PID
↓
Saturation
↓
PWM + direction
↓
VNH5019
↓
Moteur

## Matériel retenu
- NUCLEO-G431RB
- Pololu #4863
- Encodeur quadrature intégré
- VNH5019 #1451
- alimentation 12 V

## Grandeur contrôlée
Vitesse en RPM

## Plage V0
Hypothèse : 0 à 300 RPM

## Boucle PID
Hypothèse :
- période : 10 ms
- fréquence : 100 Hz

À valider expérimentalement.

## PWM
Hypothèse :
- fréquence : 20 kHz
- commande : -100 % à +100 %

À valider expérimentalement.

## Encodeur
Environ 979.62 counts/rev sur l'arbre de sortie.

RPM = delta_counts × 60 / (counts_per_rev × delta_t)

## Sécurité
- saturation PID
- PWM = 0 lorsque consigne = 0
- arrêt sur défaut
- gestion anti-windup

## Points restant à valider
- pin mapping
- période PID
- fréquence PWM
- plage RPM
- gains Kp / Ki / Kd
