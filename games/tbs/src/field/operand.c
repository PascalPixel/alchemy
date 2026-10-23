/// @file
/// Script work values: the handler SC_SET, SC_ADD and SC_CMP call for each.

#include "unit/field/operand.h"
#include "unit/field/field.h"
#include "types.h"

/// The handler for each work value number (enum ObjectScriptValue).
const OperandFunc gObjectScript_OperandHandlerTable[41] = {
    ObjectScript_SetOrCompareAddress,          // 0x00 SW_SCRIPT
    ObjectScript_SetOrCompareCursor,           // 0x01 SW_CURSOR
    ObjectScript_SetOrCompareUnsignedHalfword, // 0x02
    ObjectScript_SetOrCompareUnsignedHalfword, // 0x03 SW_FACING
    ObjectScript_SetOrCompareUnsignedHalfword, // 0x04
    ObjectScript_SetOrCompareX,                // 0x05 SW_X
    ObjectScript_SetOrCompareY,                // 0x06 SW_Y
    ObjectScript_SetOrCompareZ,                // 0x07 SW_Z
    ObjectScript_SetOrCompareRadius,           // 0x08 SW_RADIUS
    ObjectScript_SetOrCompareScaleX,           // 0x09 SW_SCALE_X
    ObjectScript_SetOrCompareScaleY,           // 0x0a SW_SCALE_Y
    ObjectScript_SetOrCompareScaleY,           // 0x0b
    ObjectScript_SetOrCompareVelocityX,        // 0x0c SW_VELOCITY_X
    ObjectScript_SetOrCompareVelocityY,        // 0x0d SW_VELOCITY_Y
    ObjectScript_SetOrCompareVelocityZ,        // 0x0e SW_VELOCITY_Z
    ObjectScript_SetOrCompareSpeedLimit,       // 0x0f SW_SPEED
    ObjectScript_SetOrCompareAcceleration,     // 0x10 SW_ACCELERATION
    ObjectScript_SetOrCompareTargetX,          // 0x11 SW_TARGET_X
    ObjectScript_SetOrCompareTargetY,          // 0x12 SW_TARGET_Y
    ObjectScript_SetOrCompareTargetZ,          // 0x13 SW_TARGET_Z
    ObjectScript_SetOrCompareVerticalMotion,   // 0x14 SW_BOUNCE
    ObjectScript_SetOrCompareGravity,          // 0x15 SW_GRAVITY
    ObjectScript_SetOrCompareTerrainHeight,    // 0x16 SW_GROUND
    ObjectScript_SetOrCompareWord4c,           // 0x17
    ObjectScript_SetOrCompareAddress50,        // 0x18
    ObjectScript_SetOrCompareByte54,           // 0x19
    ObjectScript_SetOrCompareMotionFlags,      // 0x1a SW_MOTION
    ObjectScript_SetOrCompareArrivalAxis,      // 0x1b SW_ARRIVAL
    ObjectScript_SetOrCompareComparisonResult, // 0x1c SW_RESULT
    ObjectScript_SetOrCompareSnapToTarget,     // 0x1d SW_SNAP
    ObjectScript_SetOrCompareCollisionFlags,   // 0x1e SW_COLLISION
    ObjectScript_SetOrCompareSteeringFlags,    // 0x1f SW_STEERING
    ObjectScript_SetOrComparePaused,           // 0x20 SW_PAUSED
    ObjectScript_SetOrCompareRepeatCount,      // 0x21 SW_REPEAT
    ObjectScript_SetOrCompareHalfword5e,       // 0x22 SW_WAIT
    ObjectScript_SetOrCompareSignedHalfword64, // 0x23
    ObjectScript_SetOrCompareSignedHalfword66, // 0x24
    ObjectScript_SetOrCompareWord68,           // 0x25
    ObjectScript_SetOrCompareUpdateAddress,    // 0x26 SW_HOOK
    ObjectScript_SetOrCompareByte62,           // 0x27
    ObjectScript_SetOrCompareByte63,           // 0x28
};
