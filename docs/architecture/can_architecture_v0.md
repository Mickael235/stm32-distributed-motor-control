Architecture CAN V0
1. Objectif
Le bus CAN assure les échanges entre les différents nœuds STM32 de la plateforme distribuée de contrôle moteur.

Cette première architecture définit :

la topologie du réseau ;
les principales catégories de messages ;
une première organisation des identifiants CAN ;
les responsabilités des différents nœuds ;
les principes de communication à utiliser lors de l'implémentation.
Cette version constitue une spécification initiale.

Elle ne signifie pas encore que la communication CAN physique a été validée.

2. Topologie V0
Architecture cible :

                  PC / Python
                       |
                  USB / UART
                       |
                Supervisor Node
                    STM32
                       |
==================== BUS CAN ====================
          |                         |
          |                         |
     Motor Node               Sensor / Safety Node
        STM32                       STM32
          |
     Motor + Encoder
Motor Node
Responsabilités prévues :

acquisition de l'encodeur ;
mesure de la vitesse moteur ;
exécution de la régulation PID ;
génération du PWM ;
commande du driver moteur ;
réception des consignes ;
transmission de la vitesse et de l'état du moteur.
Supervisor Node
Responsabilités prévues :

envoi des commandes de haut niveau ;
transmission des consignes de vitesse ;
surveillance de l'état des nœuds ;
réception des heartbeats ;
détection future des pertes de communication ;
transmission des données vers le PC ;
centralisation des informations de diagnostic.
Sensor / Safety Node
Ce nœud pourra à terme gérer :

des capteurs supplémentaires ;
la surveillance de sécurité ;
la détection de défauts ;
l'émission d'alarmes critiques.
Ce nœud n'est pas nécessaire au premier MVP moteur.

3. Technologie CAN retenue
Pour l'architecture V0 :

CAN classique ;
identifiants standards sur 11 bits ;
plage d'identifiants : 0x000 à 0x7FF ;
communication multi-nœuds ;
attribution statique des identifiants.
Le débit du bus CAN sera déterminé et validé lors de l'intégration matérielle.

4. Catégories de messages
Les messages sont regroupés selon leur fonction.

Catégorie	Rôle	Exemples
Urgence	Défauts critiques et mise en sécurité	FAULT, arrêt critique
Commande	Ordres transmis aux nœuds	vitesse cible, START, STOP, RESET
Mesure	Valeurs mesurées	vitesse réelle
État	État courant du système	STOP, READY, RUN, FAULT
Santé	Surveillance de présence	heartbeat
Diagnostic	Informations sur les défauts	code erreur
5. Organisation des identifiants CAN
La répartition V0 proposée est la suivante :

Plage CAN	Priorité	Type de messages
0x000 - 0x07F	Très haute	Urgences / alarmes critiques
0x100 - 0x1FF	Haute	Commandes
0x200 - 0x3FF	Moyenne	Mesures / capteurs
0x400 - 0x5FF	Basse	États / heartbeats
0x600 - 0x7FF	Très basse	Diagnostic / télémétrie
Sur un bus CAN, les identifiants numériques les plus faibles obtiennent une priorité d'arbitrage supérieure.

Cette organisation reste une proposition V0 et pourra évoluer pendant l'intégration.

6. Première matrice de communication
La première matrice est volontairement limitée aux échanges essentiels.

ID / plage V0	Message	Émetteur	Récepteur	Déclenchement
0x010	EMERGENCY_FAULT	Nœud en défaut	Tous les nœuds	Événement
0x110	MOTOR_SPEED_CMD	Supervisor Node	Motor Node	Commande
0x120	SYSTEM_COMMAND	Supervisor Node	Motor Node	Commande
0x210	MOTOR_SPEED	Motor Node	Supervisor Node	Périodique
0x410	MOTOR_STATUS	Motor Node	Supervisor Node	Périodique
0x420	MOTOR_HEARTBEAT	Motor Node	Supervisor Node	Périodique
0x430	SUPERVISOR_HEARTBEAT	Supervisor Node	Motor Node	Périodique
0x610	MOTOR_DIAGNOSTIC	Motor Node	Supervisor Node	Événement
Les formats précis des payloads seront centralisés dans :

common/network_v0.dbc
Les périodes d'émission restent à valider.

7. Principes d'échange
Commande du moteur
Supervisor Node
      |
      | MOTOR_SPEED_CMD
      v
   Motor Node
      |
      | PID + PWM
      v
     Moteur
Le Supervisor Node transmet une consigne de vitesse au Motor Node.

Le Motor Node applique ensuite cette consigne à la chaîne de régulation.

Retour de mesure
Encodeur
   |
   v
Motor Node
   |
   | MOTOR_SPEED
   v
Supervisor Node
Le Motor Node mesure la vitesse du moteur et transmet cette information au Supervisor Node.

Surveillance par heartbeat
Chaque nœud critique devra envoyer périodiquement un heartbeat.

Node
 |
 | HEARTBEAT
 v
Supervisor
 |
 +--> nœud présent

Absence de heartbeat
 |
 v
Timeout
 |
 v
Défaut de communication
Les périodes de heartbeat et les valeurs de timeout restent à déterminer expérimentalement.

8. Réception CAN sous FreeRTOS
La réception CAN suivra une architecture avec interruption courte et traitement dans une tâche FreeRTOS.

CAN RX Interrupt
       |
       v
    Queue CAN
       |
       v
CAN Parsing Task
       |
       +--> Commandes
       |
       +--> Événements
       |
       +--> FSM / Control
L'interruption CAN doit rester minimale.

Son rôle sera principalement :

récupérer la trame reçue ;
la placer dans une queue FreeRTOS ;
réveiller si nécessaire la tâche de traitement.
Le décodage et le traitement applicatif seront effectués hors interruption.

9. Sécurité et gestion des défauts
Les messages critiques doivent être prioritaires sur les commandes normales et la télémétrie.

Un défaut critique devra pouvoir provoquer une transition vers un état sécurisé.

Défaut critique
      |
      v
    FAULT
      |
      v
Arrêt commande moteur
      |
      v
PWM désactivé
La machine d'état globale prévue utilise :

STOP
READY
RUN
FAULT
Une perte de communication avec un nœud critique pourra provoquer une transition vers FAULT.

La sortie de l'état FAULT devra nécessiter :

une commande RESET ;
la disparition de la cause du défaut ;
un système considéré comme sain ;
un retour vers STOP avant tout nouveau démarrage.
10. Points restant à valider
Les éléments suivants ne sont pas encore considérés comme définitivement validés :

modèle définitif des STM32 pour chaque nœud ;
modèle des transceivers CAN ;
débit CAN ;
nombre final de nœuds ;
identifiants CAN définitifs ;
format exact des payloads ;
périodes des messages périodiques ;
période des heartbeats ;
seuils de timeout ;
filtres matériels CAN STM32 ;
brochage CAN ;
stratégie Bus-Off ;
fréquence de télémétrie ;
stratégie finale de diagnostic.
Ces choix seront progressivement validés pendant les phases d'implémentation, d'intégration et de test.