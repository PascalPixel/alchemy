/// @file
/// Script work values: the handler SC_SET, SC_ADD and SC_CMP call for each.

#include "unit/field/operand.h"
#include "unit/field/field.h"
#include "types.h"

/// The handler for each work value number (enum ScriptWork).
const OperandFunc gScript_OperandHandlerTable[41] = {
    ActionScript_SetOrCompareAddress,          // 0x00 SW_SCRIPT
    ActionScript_SetOrCompareCursor,           // 0x01 SW_CURSOR
    ActionScript_SetOrCompareUnsignedHalfword, // 0x02
    ActionScript_SetOrCompareUnsignedHalfword, // 0x03 SW_FACING
    ActionScript_SetOrCompareUnsignedHalfword, // 0x04
    ActionScript_SetOrCompareX,                // 0x05 SW_X
    ActionScript_SetOrCompareY,                // 0x06 SW_Y
    ActionScript_SetOrCompareZ,                // 0x07 SW_Z
    ActionScript_SetOrCompareRadius,           // 0x08 SW_RADIUS
    ActionScript_SetOrCompareScaleX,           // 0x09 SW_SCALE_X
    ActionScript_SetOrCompareScaleY,           // 0x0a SW_SCALE_Y
    ActionScript_SetOrCompareScaleY,           // 0x0b
    ActionScript_SetOrCompareVelocityX,        // 0x0c SW_VELOCITY_X
    ActionScript_SetOrCompareVelocityY,        // 0x0d SW_VELOCITY_Y
    ActionScript_SetOrCompareVelocityZ,        // 0x0e SW_VELOCITY_Z
    ActionScript_SetOrCompareSpeedLimit,       // 0x0f SW_SPEED
    ActionScript_SetOrCompareAcceleration,     // 0x10 SW_ACCELERATION
    ActionScript_SetOrCompareTargetX,          // 0x11 SW_TARGET_X
    ActionScript_SetOrCompareTargetY,          // 0x12 SW_TARGET_Y
    ActionScript_SetOrCompareTargetZ,          // 0x13 SW_TARGET_Z
    ActionScript_SetOrCompareVerticalMotion,   // 0x14 SW_BOUNCE
    ActionScript_SetOrCompareGravity,          // 0x15 SW_GRAVITY
    ActionScript_SetOrCompareTerrainHeight,    // 0x16 SW_GROUND
    ActionScript_SetOrCompareWord4c,           // 0x17
    ActionScript_SetOrCompareAddress50,        // 0x18
    ActionScript_SetOrCompareByte54,           // 0x19
    ActionScript_SetOrCompareMotionFlags,      // 0x1a SW_MOTION
    ActionScript_SetOrCompareArrivalAxis,      // 0x1b SW_ARRIVAL
    ActionScript_SetOrCompareComparisonResult, // 0x1c SW_RESULT
    ActionScript_SetOrCompareSnapToTarget,     // 0x1d SW_SNAP
    ActionScript_SetOrCompareCollisionFlags,   // 0x1e SW_COLLISION
    ActionScript_SetOrCompareSteeringFlags,    // 0x1f SW_STEERING
    ActionScript_SetOrComparePaused,           // 0x20 SW_PAUSED
    ActionScript_SetOrCompareRepeatCount,      // 0x21 SW_REPEAT
    ActionScript_SetOrCompareHalfword5e,       // 0x22 SW_WAIT
    ActionScript_SetOrCompareSignedHalfword64, // 0x23
    ActionScript_SetOrCompareSignedHalfword66, // 0x24
    ActionScript_SetOrCompareWord68,           // 0x25
    ActionScript_SetOrCompareUpdateAddress,    // 0x26 SW_HOOK
    ActionScript_SetOrCompareByte62,           // 0x27
    ActionScript_SetOrCompareByte63,           // 0x28
};
