#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "can_protocol.h"


#define FLOAT_TOLERANCE 0.05f


static void assert_float_close(
    float actual,
    float expected
)
{
    assert(
        fabsf(actual - expected)
        < FLOAT_TOLERANCE
    );
}


/* ============================================================
 * Speed command
 * ============================================================ */

static void test_speed_command_round_trip(void)
{
    CAN_Frame_t frame;
    float decoded_rpm = 0.0f;

    CAN_ProtocolResult_t result;

    result =
        CAN_EncodeMotorSpeedCommand(
            150.0f,
            &frame
        );

    assert(result == CAN_PROTOCOL_OK);

    assert(frame.id == CAN_ID_MOTOR_SPEED_CMD);
    assert(frame.dlc == 2U);

    result =
        CAN_DecodeMotorSpeedCommand(
            &frame,
            &decoded_rpm
        );

    assert(result == CAN_PROTOCOL_OK);

    assert_float_close(
        decoded_rpm,
        150.0f
    );

    printf(
        "[PASS] speed command encode/decode\n"
    );
}


static void test_negative_speed_command(void)
{
    CAN_Frame_t frame;
    float decoded_rpm;

    assert(
        CAN_EncodeMotorSpeedCommand(
            -125.5f,
            &frame
        )
        == CAN_PROTOCOL_OK
    );

    assert(
        CAN_DecodeMotorSpeedCommand(
            &frame,
            &decoded_rpm
        )
        == CAN_PROTOCOL_OK
    );

    assert_float_close(
        decoded_rpm,
        -125.5f
    );

    printf(
        "[PASS] negative speed encode/decode\n"
    );
}


static void test_speed_out_of_range(void)
{
    CAN_Frame_t frame;

    assert(
        CAN_EncodeMotorSpeedCommand(
            400.0f,
            &frame
        )
        == CAN_PROTOCOL_OUT_OF_RANGE
    );

    printf(
        "[PASS] speed range protection\n"
    );
}


/* ============================================================
 * Speed feedback
 * ============================================================ */

static void test_motor_speed_feedback(void)
{
    CAN_Frame_t frame;
    float speed;

    assert(
        CAN_EncodeMotorSpeed(
            217.3f,
            &frame
        )
        == CAN_PROTOCOL_OK
    );

    assert(frame.id == CAN_ID_MOTOR_SPEED);

    assert(
        CAN_DecodeMotorSpeed(
            &frame,
            &speed
        )
        == CAN_PROTOCOL_OK
    );

    assert_float_close(
        speed,
        217.3f
    );

    printf(
        "[PASS] motor speed feedback\n"
    );
}


/* ============================================================
 * System command
 * ============================================================ */

static void test_system_start_command(void)
{
    CAN_Frame_t frame;
    SystemCommand_t command;

    assert(
        CAN_EncodeSystemCommand(
            SYSTEM_COMMAND_START,
            &frame
        )
        == CAN_PROTOCOL_OK
    );

    assert(frame.id == CAN_ID_SYSTEM_COMMAND);
    assert(frame.data[0] == 2U);

    assert(
        CAN_DecodeSystemCommand(
            &frame,
            &command
        )
        == CAN_PROTOCOL_OK
    );

    assert(
        command ==
        SYSTEM_COMMAND_START
    );

    printf(
        "[PASS] system START command\n"
    );
}


/* ============================================================
 * Motor state
 * ============================================================ */

static void test_motor_state_run(void)
{
    CAN_Frame_t frame;
    MotorState_t state;

    assert(
        CAN_EncodeMotorStatus(
            MOTOR_STATE_RUN,
            &frame
        )
        == CAN_PROTOCOL_OK
    );

    assert(
        CAN_DecodeMotorStatus(
            &frame,
            &state
        )
        == CAN_PROTOCOL_OK
    );

    assert(
        state == MOTOR_STATE_RUN
    );

    printf(
        "[PASS] motor state RUN\n"
    );
}


/* ============================================================
 * Heartbeat
 * ============================================================ */

static void test_motor_heartbeat(void)
{
    CAN_Frame_t frame;
    uint8_t counter;

    assert(
        CAN_EncodeMotorHeartbeat(
            42U,
            &frame
        )
        == CAN_PROTOCOL_OK
    );

    assert(
        CAN_DecodeHeartbeat(
            &frame,
            &counter
        )
        == CAN_PROTOCOL_OK
    );

    assert(counter == 42U);

    printf(
        "[PASS] motor heartbeat\n"
    );
}


static void test_supervisor_heartbeat(void)
{
    CAN_Frame_t frame;
    uint8_t counter;

    assert(
        CAN_EncodeSupervisorHeartbeat(
            99U,
            &frame
        )
        == CAN_PROTOCOL_OK
    );

    assert(
        CAN_DecodeHeartbeat(
            &frame,
            &counter
        )
        == CAN_PROTOCOL_OK
    );

    assert(counter == 99U);

    printf(
        "[PASS] supervisor heartbeat\n"
    );
}


/* ============================================================
 * Emergency fault
 * ============================================================ */

static void test_emergency_fault(void)
{
    CAN_Frame_t frame;

    NodeSource_t source;
    uint16_t fault_code;

    assert(
        CAN_EncodeEmergencyFault(
            NODE_SOURCE_MOTOR,
            0x1234U,
            &frame
        )
        == CAN_PROTOCOL_OK
    );

    assert(frame.id == CAN_ID_EMERGENCY_FAULT);
    assert(frame.dlc == 3U);

    assert(
        CAN_DecodeEmergencyFault(
            &frame,
            &source,
            &fault_code
        )
        == CAN_PROTOCOL_OK
    );

    assert(
        source ==
        NODE_SOURCE_MOTOR
    );

    assert(
        fault_code ==
        0x1234U
    );

    printf(
        "[PASS] emergency fault\n"
    );
}


/* ============================================================
 * Diagnostic
 * ============================================================ */

static void test_motor_diagnostic(void)
{
    CAN_Frame_t frame;

    uint16_t fault_code;
    DiagnosticSeverity_t severity;
    uint8_t detail;

    assert(
        CAN_EncodeMotorDiagnostic(
            0x0102U,
            DIAG_SEVERITY_ERROR,
            55U,
            &frame
        )
        == CAN_PROTOCOL_OK
    );

    assert(
        CAN_DecodeMotorDiagnostic(
            &frame,
            &fault_code,
            &severity,
            &detail
        )
        == CAN_PROTOCOL_OK
    );

    assert(
        fault_code == 0x0102U
    );

    assert(
        severity ==
        DIAG_SEVERITY_ERROR
    );

    assert(detail == 55U);

    printf(
        "[PASS] motor diagnostic\n"
    );
}


/* ============================================================
 * Robustesse
 * ============================================================ */

static void test_invalid_can_id(void)
{
    CAN_Frame_t frame;

    float speed;

    CAN_EncodeMotorSpeedCommand(
        100.0f,
        &frame
    );

    frame.id = 0x555U;

    assert(
        CAN_DecodeMotorSpeedCommand(
            &frame,
            &speed
        )
        == CAN_PROTOCOL_INVALID_ID
    );

    printf(
        "[PASS] invalid CAN ID protection\n"
    );
}


static void test_invalid_dlc(void)
{
    CAN_Frame_t frame;

    float speed;

    CAN_EncodeMotorSpeedCommand(
        100.0f,
        &frame
    );

    frame.dlc = 1U;

    assert(
        CAN_DecodeMotorSpeedCommand(
            &frame,
            &speed
        )
        == CAN_PROTOCOL_INVALID_DLC
    );

    printf(
        "[PASS] invalid DLC protection\n"
    );
}


static void test_null_pointer(void)
{
    assert(
        CAN_EncodeMotorSpeedCommand(
            100.0f,
            NULL
        )
        ==
        CAN_PROTOCOL_INVALID_ARGUMENT
    );

    printf(
        "[PASS] null pointer protection\n"
    );
}


/* ============================================================
 * Main
 * ============================================================ */

int main(void)
{
    printf(
        "=== CAN PROTOCOL UNIT TESTS ===\n\n"
    );

    test_speed_command_round_trip();
    test_negative_speed_command();
    test_speed_out_of_range();

    test_motor_speed_feedback();

    test_system_start_command();

    test_motor_state_run();

    test_motor_heartbeat();
    test_supervisor_heartbeat();

    test_emergency_fault();
    test_motor_diagnostic();

    test_invalid_can_id();
    test_invalid_dlc();
    test_null_pointer();

    printf(
        "\n=== ALL CAN PROTOCOL TESTS PASSED ===\n"
    );

    return 0;
}