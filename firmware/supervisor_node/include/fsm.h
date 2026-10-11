#ifndef FSM_H
#define FSM_H

#include <stdbool.h>

typedef enum
{
    FSM_STATE_STOP = 0,
    FSM_STATE_READY,
    FSM_STATE_RUN,
    FSM_STATE_FAULT
} FSM_State_t;

typedef enum
{
    FSM_EVENT_PREPARE = 0,
    FSM_EVENT_START,
    FSM_EVENT_STOP,
    FSM_EVENT_RESET,
    FSM_EVENT_CAN_TIMEOUT,
    FSM_EVENT_MOTOR_FAULT,
    FSM_EVENT_EMERGENCY
} FSM_Event_t;

typedef enum
{
    FSM_RESULT_OK = 0,
    FSM_RESULT_REJECTED,
    FSM_RESULT_INVALID_ARGUMENT
} FSM_Result_t;

typedef struct
{
    FSM_State_t state;
    bool health_ok;
} FSM_t;


/**
 * @brief Initialise la machine à états.
 *
 * État initial : STOP
 * Santé initiale : OK
 */
void FSM_Init(FSM_t *fsm);


/**
 * @brief Met à jour l'état de santé global du système.
 */
void FSM_SetHealth(FSM_t *fsm, bool health_ok);


/**
 * @brief Traite un événement de la machine à états.
 *
 * @return FSM_RESULT_OK si l'événement est accepté,
 *         FSM_RESULT_REJECTED si la transition est interdite,
 *         FSM_RESULT_INVALID_ARGUMENT si fsm == NULL.
 */
FSM_Result_t FSM_ProcessEvent(
    FSM_t *fsm,
    FSM_Event_t event
);


/**
 * @brief Retourne l'état actuel.
 */
FSM_State_t FSM_GetState(const FSM_t *fsm);


/**
 * @brief Retourne une chaîne représentant l'état.
 */
const char *FSM_StateToString(FSM_State_t state);

#endif /* FSM_H */