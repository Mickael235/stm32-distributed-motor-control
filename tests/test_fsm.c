#include <assert.h>
#include <stdio.h>

#include "fsm.h"


static void test_initial_state(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    assert(FSM_GetState(&fsm) == FSM_STATE_STOP);

    printf("[PASS] initial state = STOP\n");
}


static void test_stop_prepare_ready(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    FSM_Result_t result =
        FSM_ProcessEvent(&fsm, FSM_EVENT_PREPARE);

    assert(result == FSM_RESULT_OK);
    assert(FSM_GetState(&fsm) == FSM_STATE_READY);

    printf("[PASS] STOP + PREPARE -> READY\n");
}


static void test_ready_start_run(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    FSM_ProcessEvent(&fsm, FSM_EVENT_PREPARE);

    FSM_Result_t result =
        FSM_ProcessEvent(&fsm, FSM_EVENT_START);

    assert(result == FSM_RESULT_OK);
    assert(FSM_GetState(&fsm) == FSM_STATE_RUN);

    printf("[PASS] READY + START -> RUN\n");
}


static void test_run_stop_stop(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    FSM_ProcessEvent(&fsm, FSM_EVENT_PREPARE);
    FSM_ProcessEvent(&fsm, FSM_EVENT_START);

    FSM_Result_t result =
        FSM_ProcessEvent(&fsm, FSM_EVENT_STOP);

    assert(result == FSM_RESULT_OK);
    assert(FSM_GetState(&fsm) == FSM_STATE_STOP);

    printf("[PASS] RUN + STOP -> STOP\n");
}


static void test_run_motor_fault_fault(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    FSM_ProcessEvent(&fsm, FSM_EVENT_PREPARE);
    FSM_ProcessEvent(&fsm, FSM_EVENT_START);

    FSM_Result_t result =
        FSM_ProcessEvent(
            &fsm,
            FSM_EVENT_MOTOR_FAULT
        );

    assert(result == FSM_RESULT_OK);
    assert(FSM_GetState(&fsm) == FSM_STATE_FAULT);

    printf("[PASS] RUN + MOTOR_FAULT -> FAULT\n");
}


static void test_can_timeout_fault(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    FSM_ProcessEvent(&fsm, FSM_EVENT_PREPARE);
    FSM_ProcessEvent(&fsm, FSM_EVENT_START);

    FSM_Result_t result =
        FSM_ProcessEvent(
            &fsm,
            FSM_EVENT_CAN_TIMEOUT
        );

    assert(result == FSM_RESULT_OK);
    assert(FSM_GetState(&fsm) == FSM_STATE_FAULT);

    printf("[PASS] CAN_TIMEOUT -> FAULT\n");
}


static void test_emergency_fault(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    FSM_Result_t result =
        FSM_ProcessEvent(
            &fsm,
            FSM_EVENT_EMERGENCY
        );

    assert(result == FSM_RESULT_OK);
    assert(FSM_GetState(&fsm) == FSM_STATE_FAULT);

    printf("[PASS] EMERGENCY -> FAULT\n");
}


static void test_fault_start_rejected(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    FSM_ProcessEvent(
        &fsm,
        FSM_EVENT_MOTOR_FAULT
    );

    FSM_Result_t result =
        FSM_ProcessEvent(
            &fsm,
            FSM_EVENT_START
        );

    assert(result == FSM_RESULT_REJECTED);
    assert(FSM_GetState(&fsm) == FSM_STATE_FAULT);

    printf("[PASS] FAULT + START rejected\n");
}


static void test_fault_reset_without_health_rejected(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    FSM_ProcessEvent(
        &fsm,
        FSM_EVENT_MOTOR_FAULT
    );

    FSM_Result_t result =
        FSM_ProcessEvent(
            &fsm,
            FSM_EVENT_RESET
        );

    assert(result == FSM_RESULT_REJECTED);
    assert(FSM_GetState(&fsm) == FSM_STATE_FAULT);

    printf("[PASS] FAULT + RESET without health rejected\n");
}


static void test_fault_reset_health_ok_stop(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    FSM_ProcessEvent(
        &fsm,
        FSM_EVENT_MOTOR_FAULT
    );

    FSM_SetHealth(&fsm, true);

    FSM_Result_t result =
        FSM_ProcessEvent(
            &fsm,
            FSM_EVENT_RESET
        );

    assert(result == FSM_RESULT_OK);
    assert(FSM_GetState(&fsm) == FSM_STATE_STOP);

    printf("[PASS] FAULT + RESET + health_OK -> STOP\n");
}


static void test_start_from_stop_rejected(void)
{
    FSM_t fsm;

    FSM_Init(&fsm);

    FSM_Result_t result =
        FSM_ProcessEvent(
            &fsm,
            FSM_EVENT_START
        );

    assert(result == FSM_RESULT_REJECTED);
    assert(FSM_GetState(&fsm) == FSM_STATE_STOP);

    printf("[PASS] STOP + START rejected\n");
}


static void test_null_pointer(void)
{
    FSM_Result_t result =
        FSM_ProcessEvent(
            NULL,
            FSM_EVENT_START
        );

    assert(result == FSM_RESULT_INVALID_ARGUMENT);

    printf("[PASS] null pointer protection\n");
}


int main(void)
{
    printf("=== FSM UNIT TESTS ===\n\n");

    test_initial_state();

    test_stop_prepare_ready();
    test_ready_start_run();
    test_run_stop_stop();

    test_run_motor_fault_fault();
    test_can_timeout_fault();
    test_emergency_fault();

    test_fault_start_rejected();
    test_fault_reset_without_health_rejected();
    test_fault_reset_health_ok_stop();

    test_start_from_stop_rejected();
    test_null_pointer();

    printf("\n=== ALL FSM TESTS PASSED ===\n");

    return 0;
}