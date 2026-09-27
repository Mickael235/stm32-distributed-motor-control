# FSM V0

États :
- STOP
- READY
- RUN
- FAULT

Transitions principales :
```text
STOP --PREPARE--> READY
READY --START--> RUN
RUN --STOP--> STOP
ANY --FAULT--> FAULT
FAULT --RESET + system healthy--> STOP
```

Événements prévus :
- EV_PREPARE
- EV_START
- EV_STOP
- EV_RESET
- EV_CAN_TIMEOUT
- EV_MOTOR_FAULT
- EV_EMERGENCY
