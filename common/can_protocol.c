#include "can_protocol.h"

#include <stddef.h>


/* ============================================================
 * Fonctions internes
 * ============================================================ */

static void CAN_ClearFrame(CAN_Frame_t *frame)
{
    uint8_t i;

    frame->id = 0U;
    frame->dlc = 0U;

    for (i = 0U; i < CAN_PROTOCOL_MAX_DATA_LENGTH; i++)
    {
        frame->data[i] = 0U;
    }
}


static void CAN_WriteUint16LE(uint8_t *data, uint16_t value)
{
    data[0] = (uint8_t)(value & 0xFFU);
    data[1] = (uint8_t)((value >> 8) & 0xFFU);
}


static uint16_t CAN_ReadUint16LE(const uint8_t *data)
{
    return
        ((uint16_t)data[0]) |
        ((uint16_t)data[1] << 8);
}


static int16_t CAN_RpmToRaw(float rpm)
{
    float scaled = rpm * CAN_SPEED_SCALE;

    if (scaled >= 0.0f)
    {
        scaled += 0.5f;
    }
    else
    {
        scaled -= 0.5f;
    }

    return (int16_t)scaled;
}


static float CAN_RawToRpm(int16_t raw)
{
    return ((float)raw) / CAN_SPEED_SCALE;
}


/* ============================================================
 * Motor Speed Command
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeMotorSpeedCommand(
    float target_rpm,
    CAN_Frame_t *frame
)
{
    int16_t raw_speed;

    if (frame == NULL)
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if ((target_rpm < CAN_SPEED_MIN_RPM) ||
        (target_rpm > CAN_SPEED_MAX_RPM))
    {
        return CAN_PROTOCOL_OUT_OF_RANGE;
    }

    CAN_ClearFrame(frame);

    frame->id = CAN_ID_MOTOR_SPEED_CMD;
    frame->dlc = 2U;

    raw_speed = CAN_RpmToRaw(target_rpm);

    CAN_WriteUint16LE(
        frame->data,
        (uint16_t)raw_speed
    );

    return CAN_PROTOCOL_OK;
}


CAN_ProtocolResult_t CAN_DecodeMotorSpeedCommand(
    const CAN_Frame_t *frame,
    float *target_rpm
)
{
    int16_t raw_speed;

    if ((frame == NULL) || (target_rpm == NULL))
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if (frame->id != CAN_ID_MOTOR_SPEED_CMD)
    {
        return CAN_PROTOCOL_INVALID_ID;
    }

    if (frame->dlc != 2U)
    {
        return CAN_PROTOCOL_INVALID_DLC;
    }

    raw_speed =
        (int16_t)CAN_ReadUint16LE(frame->data);

    *target_rpm = CAN_RawToRpm(raw_speed);

    return CAN_PROTOCOL_OK;
}


/* ============================================================
 * Motor Speed Feedback
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeMotorSpeed(
    float measured_rpm,
    CAN_Frame_t *frame
)
{
    int16_t raw_speed;

    if (frame == NULL)
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if ((measured_rpm < CAN_SPEED_MIN_RPM) ||
        (measured_rpm > CAN_SPEED_MAX_RPM))
    {
        return CAN_PROTOCOL_OUT_OF_RANGE;
    }

    CAN_ClearFrame(frame);

    frame->id = CAN_ID_MOTOR_SPEED;
    frame->dlc = 2U;

    raw_speed = CAN_RpmToRaw(measured_rpm);

    CAN_WriteUint16LE(
        frame->data,
        (uint16_t)raw_speed
    );

    return CAN_PROTOCOL_OK;
}


CAN_ProtocolResult_t CAN_DecodeMotorSpeed(
    const CAN_Frame_t *frame,
    float *measured_rpm
)
{
    int16_t raw_speed;

    if ((frame == NULL) || (measured_rpm == NULL))
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if (frame->id != CAN_ID_MOTOR_SPEED)
    {
        return CAN_PROTOCOL_INVALID_ID;
    }

    if (frame->dlc != 2U)
    {
        return CAN_PROTOCOL_INVALID_DLC;
    }

    raw_speed =
        (int16_t)CAN_ReadUint16LE(frame->data);

    *measured_rpm = CAN_RawToRpm(raw_speed);

    return CAN_PROTOCOL_OK;
}


/* ============================================================
 * System Command
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeSystemCommand(
    SystemCommand_t command,
    CAN_Frame_t *frame
)
{
    if (frame == NULL)
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if (command > SYSTEM_COMMAND_RESET)
    {
        return CAN_PROTOCOL_OUT_OF_RANGE;
    }

    CAN_ClearFrame(frame);

    frame->id = CAN_ID_SYSTEM_COMMAND;
    frame->dlc = 1U;
    frame->data[0] = (uint8_t)command;

    return CAN_PROTOCOL_OK;
}


CAN_ProtocolResult_t CAN_DecodeSystemCommand(
    const CAN_Frame_t *frame,
    SystemCommand_t *command
)
{
    if ((frame == NULL) || (command == NULL))
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if (frame->id != CAN_ID_SYSTEM_COMMAND)
    {
        return CAN_PROTOCOL_INVALID_ID;
    }

    if (frame->dlc != 1U)
    {
        return CAN_PROTOCOL_INVALID_DLC;
    }

    if (frame->data[0] > SYSTEM_COMMAND_RESET)
    {
        return CAN_PROTOCOL_OUT_OF_RANGE;
    }

    *command = (SystemCommand_t)frame->data[0];

    return CAN_PROTOCOL_OK;
}


/* ============================================================
 * Motor Status
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeMotorStatus(
    MotorState_t state,
    CAN_Frame_t *frame
)
{
    if (frame == NULL)
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if (state > MOTOR_STATE_FAULT)
    {
        return CAN_PROTOCOL_OUT_OF_RANGE;
    }

    CAN_ClearFrame(frame);

    frame->id = CAN_ID_MOTOR_STATUS;
    frame->dlc = 1U;
    frame->data[0] = (uint8_t)state;

    return CAN_PROTOCOL_OK;
}


CAN_ProtocolResult_t CAN_DecodeMotorStatus(
    const CAN_Frame_t *frame,
    MotorState_t *state
)
{
    if ((frame == NULL) || (state == NULL))
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if (frame->id != CAN_ID_MOTOR_STATUS)
    {
        return CAN_PROTOCOL_INVALID_ID;
    }

    if (frame->dlc != 1U)
    {
        return CAN_PROTOCOL_INVALID_DLC;
    }

    if (frame->data[0] > MOTOR_STATE_FAULT)
    {
        return CAN_PROTOCOL_OUT_OF_RANGE;
    }

    *state = (MotorState_t)frame->data[0];

    return CAN_PROTOCOL_OK;
}


/* ============================================================
 * Heartbeats
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeMotorHeartbeat(
    uint8_t alive_counter,
    CAN_Frame_t *frame
)
{
    if (frame == NULL)
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    CAN_ClearFrame(frame);

    frame->id = CAN_ID_MOTOR_HEARTBEAT;
    frame->dlc = 1U;
    frame->data[0] = alive_counter;

    return CAN_PROTOCOL_OK;
}


CAN_ProtocolResult_t CAN_EncodeSupervisorHeartbeat(
    uint8_t alive_counter,
    CAN_Frame_t *frame
)
{
    if (frame == NULL)
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    CAN_ClearFrame(frame);

    frame->id = CAN_ID_SUPERVISOR_HEARTBEAT;
    frame->dlc = 1U;
    frame->data[0] = alive_counter;

    return CAN_PROTOCOL_OK;
}


CAN_ProtocolResult_t CAN_DecodeHeartbeat(
    const CAN_Frame_t *frame,
    uint8_t *alive_counter
)
{
    if ((frame == NULL) || (alive_counter == NULL))
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if ((frame->id != CAN_ID_MOTOR_HEARTBEAT) &&
        (frame->id != CAN_ID_SUPERVISOR_HEARTBEAT))
    {
        return CAN_PROTOCOL_INVALID_ID;
    }

    if (frame->dlc != 1U)
    {
        return CAN_PROTOCOL_INVALID_DLC;
    }

    *alive_counter = frame->data[0];

    return CAN_PROTOCOL_OK;
}


/* ============================================================
 * Emergency Fault
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeEmergencyFault(
    NodeSource_t source,
    uint16_t fault_code,
    CAN_Frame_t *frame
)
{
    if (frame == NULL)
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if ((source < NODE_SOURCE_MOTOR) ||
        (source > NODE_SOURCE_SAFETY))
    {
        return CAN_PROTOCOL_OUT_OF_RANGE;
    }

    CAN_ClearFrame(frame);

    frame->id = CAN_ID_EMERGENCY_FAULT;
    frame->dlc = 3U;

    frame->data[0] = (uint8_t)source;

    CAN_WriteUint16LE(
        &frame->data[1],
        fault_code
    );

    return CAN_PROTOCOL_OK;
}


CAN_ProtocolResult_t CAN_DecodeEmergencyFault(
    const CAN_Frame_t *frame,
    NodeSource_t *source,
    uint16_t *fault_code
)
{
    if ((frame == NULL) ||
        (source == NULL) ||
        (fault_code == NULL))
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if (frame->id != CAN_ID_EMERGENCY_FAULT)
    {
        return CAN_PROTOCOL_INVALID_ID;
    }

    if (frame->dlc != 3U)
    {
        return CAN_PROTOCOL_INVALID_DLC;
    }

    if ((frame->data[0] < NODE_SOURCE_MOTOR) ||
        (frame->data[0] > NODE_SOURCE_SAFETY))
    {
        return CAN_PROTOCOL_OUT_OF_RANGE;
    }

    *source = (NodeSource_t)frame->data[0];

    *fault_code =
        CAN_ReadUint16LE(&frame->data[1]);

    return CAN_PROTOCOL_OK;
}


/* ============================================================
 * Motor Diagnostic
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeMotorDiagnostic(
    uint16_t fault_code,
    DiagnosticSeverity_t severity,
    uint8_t detail,
    CAN_Frame_t *frame
)
{
    if (frame == NULL)
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if (severity > DIAG_SEVERITY_CRITICAL)
    {
        return CAN_PROTOCOL_OUT_OF_RANGE;
    }

    CAN_ClearFrame(frame);

    frame->id = CAN_ID_MOTOR_DIAGNOSTIC;
    frame->dlc = 4U;

    CAN_WriteUint16LE(
        frame->data,
        fault_code
    );

    frame->data[2] = (uint8_t)severity;
    frame->data[3] = detail;

    return CAN_PROTOCOL_OK;
}


CAN_ProtocolResult_t CAN_DecodeMotorDiagnostic(
    const CAN_Frame_t *frame,
    uint16_t *fault_code,
    DiagnosticSeverity_t *severity,
    uint8_t *detail
)
{
    if ((frame == NULL) ||
        (fault_code == NULL) ||
        (severity == NULL) ||
        (detail == NULL))
    {
        return CAN_PROTOCOL_INVALID_ARGUMENT;
    }

    if (frame->id != CAN_ID_MOTOR_DIAGNOSTIC)
    {
        return CAN_PROTOCOL_INVALID_ID;
    }

    if (frame->dlc != 4U)
    {
        return CAN_PROTOCOL_INVALID_DLC;
    }

    if (frame->data[2] > DIAG_SEVERITY_CRITICAL)
    {
        return CAN_PROTOCOL_OUT_OF_RANGE;
    }

    *fault_code =
        CAN_ReadUint16LE(frame->data);

    *severity =
        (DiagnosticSeverity_t)frame->data[2];

    *detail =
        frame->data[3];

    return CAN_PROTOCOL_OK;
}