#include "fsm.h"

#include <stddef.h>


void FSM_Init(FSM_t *fsm)
{
    if (fsm == NULL)
    {
        return;
    }

    fsm->state = FSM_STATE_STOP;
    fsm->health_ok = true;
}


void FSM_SetHealth(FSM_t *fsm, bool health_ok)
{
    if (fsm == NULL)
    {
        return;
    }

    fsm->health_ok = health_ok;
}


FSM_Result_t FSM_ProcessEvent(
    FSM_t *fsm,
    FSM_Event_t event
)
{
    if (fsm == NULL)
    {
        return FSM_RESULT_INVALID_ARGUMENT;
    }

    /*
     * Les défauts critiques sont prioritaires
     * et provoquent une transition vers FAULT
     * depuis n'importe quel état.
     */
    if ((event == FSM_EVENT_CAN_TIMEOUT) ||
        (event == FSM_EVENT_MOTOR_FAULT) ||
        (event == FSM_EVENT_EMERGENCY))
    {
        fsm->state = FSM_STATE_FAULT;
        fsm->health_ok = false;

        return FSM_RESULT_OK;
    }

    switch (fsm->state)
    {
        case FSM_STATE_STOP:

            if (event == FSM_EVENT_PREPARE)
            {
                fsm->state = FSM_STATE_READY;
                return FSM_RESULT_OK;
            }

            return FSM_RESULT_REJECTED;


        case FSM_STATE_READY:

            if (event == FSM_EVENT_START)
            {
                if (!fsm->health_ok)
                {
                    return FSM_RESULT_REJECTED;
                }

                fsm->state = FSM_STATE_RUN;
                return FSM_RESULT_OK;
            }

            if (event == FSM_EVENT_STOP)
            {
                fsm->state = FSM_STATE_STOP;
                return FSM_RESULT_OK;
            }

            return FSM_RESULT_REJECTED;


        case FSM_STATE_RUN:

            if (event == FSM_EVENT_STOP)
            {
                fsm->state = FSM_STATE_STOP;
                return FSM_RESULT_OK;
            }

            return FSM_RESULT_REJECTED;


        case FSM_STATE_FAULT:

            if (event == FSM_EVENT_RESET)
            {
                if (!fsm->health_ok)
                {
                    return FSM_RESULT_REJECTED;
                }

                fsm->state = FSM_STATE_STOP;
                return FSM_RESULT_OK;
            }

            return FSM_RESULT_REJECTED;


        default:

            return FSM_RESULT_REJECTED;
    }
}


FSM_State_t FSM_GetState(const FSM_t *fsm)
{
    if (fsm == NULL)
    {
        return FSM_STATE_FAULT;
    }

    return fsm->state;
}


const char *FSM_StateToString(FSM_State_t state)
{
    switch (state)
    {
        case FSM_STATE_STOP:
            return "STOP";

        case FSM_STATE_READY:
            return "READY";

        case FSM_STATE_RUN:
            return "RUN";

        case FSM_STATE_FAULT:
            return "FAULT";

        default:
            return "UNKNOWN";
    }
}