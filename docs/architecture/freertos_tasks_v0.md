# Architecture FreeRTOS V0

## Tâches prévues

### Task_Control_PID
- rôle : régulation moteur
- périodicité : à confirmer expérimentalement
- entrée : consigne + encodeur
- sortie : PWM

### Task_Monitoring
- rôle : états / mesures

### Task_Safety
- rôle : watchdog / limites / arrêt sûr

> Toute valeur temporelle reste une hypothèse tant qu'elle n'a pas été validée expérimentalement.
