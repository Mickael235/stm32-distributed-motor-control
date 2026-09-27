# Architecture commande moteur V0

```text
Encodeur
  ↓
Acquisition vitesse
  ↓
Filtrage éventuel
  ↓
Erreur = consigne - mesure
  ↓
PID
  ↓
Saturation
  ↓
PWM
  ↓
Driver
  ↓
Moteur
```

## Paramètres à figer
- moteur DC avec encodeur ;
- encodeur incrémental ;
- driver ;
- alimentation ;
- grandeur contrôlée : vitesse ;
- période de régulation.
