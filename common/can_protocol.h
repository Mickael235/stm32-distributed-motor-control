#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include <stdint.h>

/* ============================================================
 * Identifiants CAN V0
 * ============================================================ */

#define CAN_ID_EMERGENCY_FAULT        0x010U
#define CAN_ID_MOTOR_SPEED_CMD        0x110U
#define CAN_ID_SYSTEM_COMMAND         0x120U
#define CAN_ID_MOTOR_SPEED            0x210U
#define CAN_ID_MOTOR_STATUS           0x410U
#define CAN_ID_MOTOR_HEARTBEAT        0x420U
#define CAN_ID_SUPERVISOR_HEARTBEAT   0x430U
#define CAN_ID_MOTOR_DIAGNOSTIC       0x610U


/* ============================================================
 * Constantes protocole
 * ============================================================ */

#define CAN_PROTOCOL_MAX_DATA_LENGTH  8U

#define CAN_SPEED_MIN_RPM            (-300.0f)
#define CAN_SPEED_MAX_RPM             (300.0f)
#define CAN_SPEED_SCALE               (10.0f)


/* ============================================================
 * Types
 * ============================================================ */

typedef struct
{
    uint16_t id;
    uint8_t dlc;
    uint8_t data[CAN_PROTOCOL_MAX_DATA_LENGTH];

} CAN_Frame_t;


typedef enum
{
    CAN_PROTOCOL_OK = 0,
    CAN_PROTOCOL_INVALID_ARGUMENT,
    CAN_PROTOCOL_INVALID_ID,
    CAN_PROTOCOL_INVALID_DLC,
    CAN_PROTOCOL_OUT_OF_RANGE

} CAN_ProtocolResult_t;


typedef enum
{
    SYSTEM_COMMAND_STOP = 0,
    SYSTEM_COMMAND_PREPARE,
    SYSTEM_COMMAND_START,
    SYSTEM_COMMAND_RESET

} SystemCommand_t;


typedef enum
{
    MOTOR_STATE_STOP = 0,
    MOTOR_STATE_READY,
    MOTOR_STATE_RUN,
    MOTOR_STATE_FAULT

} MotorState_t;


typedef enum
{
    NODE_SOURCE_UNKNOWN = 0,
    NODE_SOURCE_MOTOR = 1,
    NODE_SOURCE_SUPERVISOR = 2,
    NODE_SOURCE_SAFETY = 3

} NodeSource_t;


typedef enum
{
    DIAG_SEVERITY_INFO = 0,
    DIAG_SEVERITY_WARNING,
    DIAG_SEVERITY_ERROR,
    DIAG_SEVERITY_CRITICAL

} DiagnosticSeverity_t;


/* ============================================================
 * Motor Speed Command
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeMotorSpeedCommand(
    float target_rpm,
    CAN_Frame_t *frame
);

CAN_ProtocolResult_t CAN_DecodeMotorSpeedCommand(
    const CAN_Frame_t *frame,
    float *target_rpm
);


/* ============================================================
 * Motor Speed Feedback
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeMotorSpeed(
    float measured_rpm,
    CAN_Frame_t *frame
);

CAN_ProtocolResult_t CAN_DecodeMotorSpeed(
    const CAN_Frame_t *frame,
    float *measured_rpm
);


/* ============================================================
 * System Command
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeSystemCommand(
    SystemCommand_t command,
    CAN_Frame_t *frame
);

CAN_ProtocolResult_t CAN_DecodeSystemCommand(
    const CAN_Frame_t *frame,
    SystemCommand_t *command
);


/* ============================================================
 * Motor Status
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeMotorStatus(
    MotorState_t state,
    CAN_Frame_t *frame
);

CAN_ProtocolResult_t CAN_DecodeMotorStatus(
    const CAN_Frame_t *frame,
    MotorState_t *state
);


/* ============================================================
 * Heartbeats
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeMotorHeartbeat(
    uint8_t alive_counter,
    CAN_Frame_t *frame
);

CAN_ProtocolResult_t CAN_EncodeSupervisorHeartbeat(
    uint8_t alive_counter,
    CAN_Frame_t *frame
);

CAN_ProtocolResult_t CAN_DecodeHeartbeat(
    const CAN_Frame_t *frame,
    uint8_t *alive_counter
);


/* ============================================================
 * Emergency Fault
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeEmergencyFault(
    NodeSource_t source,
    uint16_t fault_code,
    CAN_Frame_t *frame
);

CAN_ProtocolResult_t CAN_DecodeEmergencyFault(
    const CAN_Frame_t *frame,
    NodeSource_t *source,
    uint16_t *fault_code
);


/* ============================================================
 * Diagnostic
 * ============================================================ */

CAN_ProtocolResult_t CAN_EncodeMotorDiagnostic(
    uint16_t fault_code,
    DiagnosticSeverity_t severity,
    uint8_t detail,
    CAN_Frame_t *frame
);

CAN_ProtocolResult_t CAN_DecodeMotorDiagnostic(
    const CAN_Frame_t *frame,
    uint16_t *fault_code,
    DiagnosticSeverity_t *severity,
    uint8_t *detail
);

#endif /* CAN_PROTOCOL_H */