# STM32 Distributed Motor Control

Projet personnel en binôme — Michael & Nawel.

## Objectif
Concevoir progressivement une plateforme distribuée de contrôle moteur temps réel sur STM32 intégrant :
- moteur DC + encodeur ;
- régulation PID ;
- PWM ;
- FreeRTOS ;
- communication CAN entre nœuds ;
- machine à états et mécanismes de sûreté ;
- supervision / diagnostic ;
- tests logiciels et CI.

## État actuel
Le projet est en phase de spécification / architecture initiale.
Les fonctionnalités matérielles et logicielles ne doivent être présentées comme réalisées qu'après validation expérimentale.

## MVP V1
Faire fonctionner un moteur DC avec retour encodeur et régulation PID périodique sous FreeRTOS.

## Organisation du dépôt
- `docs/` : recherche, spécifications, architecture, matériel et validation
- `firmware/` : firmwares STM32
- `common/` : éléments partagés, notamment le DBC CAN
- `supervisor/` : outils PC / Python
- `tests/` : tests unitaires et mocks
- `.github/` : workflows CI et configuration GitHub

## Contributions
Les contributions sont tracées via Issues, branches, commits, Pull Requests et reviews croisées.
