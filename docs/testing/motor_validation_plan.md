# Plan de validation moteur V0

## Test 1 — PWM

Vérifier :
- fréquence ;
- duty cycle ;
- niveau logique ;
- variation de commande.

## Test 2 — Moteur sans PID

Tester progressivement :
0 %, 10 %, 20 %, etc.

Observer :
- démarrage ;
- sens ;
- courant ;
- vitesse.

## Test 3 — Encodeur

Vérifier :
- comptage ;
- sens ;
- cohérence A/B ;
- calcul RPM.

## Test 4 — Conversion RPM

Tester plusieurs vitesses et comparer la mesure calculée avec une référence
si disponible.

## Test 5 — PID

Consignes prévues :
- 0 RPM
- 50 RPM
- 100 RPM
- 150 RPM
- 200 RPM
- 250 RPM
- 300 RPM

Variables à enregistrer :
- timestamp ;
- consigne ;
- vitesse mesurée ;
- erreur ;
- sortie PID ;
- PWM ;
- direction ;
- compteur encodeur ;
- état ;
- défaut.

## Test 6 — Saturation

Vérifier :
- saturation à ±100 % ;
- anti-windup ;
- récupération après saturation.

## Test 7 — Arrêt

Tester :
consigne > 0 → consigne = 0

Vérifier :
- PWM → 0 ;
- arrêt moteur ;
- intégrateur correctement géré ;
- absence de redémarrage intempestif.
