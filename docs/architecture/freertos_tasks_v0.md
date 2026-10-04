# Architecture FreeRTOS V0

## MotorControlTask

Rôle :
- lecture compteur encodeur ;
- calcul RPM ;
- calcul de l'erreur ;
- PID ;
- saturation ;
- PWM ;
- direction.

Période initiale :
10 ms — hypothèse à valider.

Priorité :
élevée.

Entrées :
- target_rpm ;
- encoder_count ;
- motor_state.

Sorties :
- measured_rpm ;
- pid_output ;
- PWM ;
- direction.

La périodicité sera réalisée avec `vTaskDelayUntil()`.

---

## SafetyTask

Rôle :
- surveillance du driver ;
- plausibilité encodeur ;
- perte de feedback ;
- timeout ;
- demande d'arrêt sur défaut.

Période initiale :
100 ms — hypothèse à valider.

Priorité :
élevée mais inférieure à MotorControlTask.

---

## TelemetryTask

Rôle :
- consigne ;
- vitesse mesurée ;
- sortie PID ;
- état moteur ;
- diagnostic.

Période :
100 à 200 ms — hypothèse à valider.

Priorité :
basse.

---

## Données partagées

- target_rpm
- measured_rpm
- pid_output
- motor_state
- fault_state
- encoder_count

Le design final déterminera les mécanismes de synchronisation nécessaires.
