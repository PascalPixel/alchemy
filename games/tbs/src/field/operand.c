#include "unit/field/operand.h"
#include "unit/field/field.h"
#include "types.h"

//--------------------------------------------------------------------------
// 作業値の代入、加算と比較
// Script work values: the handler SC_SET, SC_ADD and SC_CMP call for each.
//--------------------------------------------------------------------------

// 作業値の番号 (enum ScriptWork) ごとの処理
const OperandFunc gScript_OperandHandlerTable[41] = {
    Script_SetOrCompareAddress,          // 0x00 SW_SCRIPT
    Script_SetOrCompareCursor,           // 0x01 SW_CURSOR
    Script_SetOrCompareUnsignedHalfword, // 0x02
    Script_SetOrCompareUnsignedHalfword, // 0x03 SW_FACING
    Script_SetOrCompareUnsignedHalfword, // 0x04
    Script_SetOrCompareX,                // 0x05 SW_X
    Script_SetOrCompareY,                // 0x06 SW_Y
    Script_SetOrCompareZ,                // 0x07 SW_Z
    Script_SetOrCompareRadius,           // 0x08 SW_RADIUS
    Script_SetOrCompareScaleX,           // 0x09 SW_SCALE_X
    Script_SetOrCompareScaleY,           // 0x0a SW_SCALE_Y
    Script_SetOrCompareScaleY,           // 0x0b
    Script_SetOrCompareVelocityX,        // 0x0c SW_VELOCITY_X
    Script_SetOrCompareVelocityY,        // 0x0d SW_VELOCITY_Y
    Script_SetOrCompareVelocityZ,        // 0x0e SW_VELOCITY_Z
    Script_SetOrCompareSpeedLimit,       // 0x0f SW_SPEED
    Script_SetOrCompareAcceleration,     // 0x10 SW_ACCEL
    Script_SetOrCompareTargetX,          // 0x11 SW_TARGET_X
    Script_SetOrCompareTargetY,          // 0x12 SW_TARGET_Y
    Script_SetOrCompareTargetZ,          // 0x13 SW_TARGET_Z
    Script_SetOrCompareVerticalMotion,   // 0x14 SW_BOUNCE
    Script_SetOrCompareGravity,          // 0x15 SW_GRAVITY
    Script_SetOrCompareTerrainHeight,    // 0x16 SW_GROUND
    Script_SetOrCompareWord4c,           // 0x17
    Script_SetOrCompareAddress50,        // 0x18
    Script_SetOrCompareByte54,           // 0x19
    Script_SetOrCompareMotionFlags,      // 0x1a SW_MOTION
    Script_SetOrCompareArrivalAxis,      // 0x1b SW_ARRIVAL
    Script_SetOrCompareComparisonResult, // 0x1c SW_RESULT
    Script_SetOrCompareSnapToTarget,     // 0x1d SW_SNAP
    Script_SetOrCompareCollisionFlags,   // 0x1e SW_COLLISION
    Script_SetOrCompareSteeringFlags,    // 0x1f SW_STEERING
    Script_SetOrComparePaused,           // 0x20 SW_PAUSED
    Script_SetOrCompareRepeatCount,      // 0x21 SW_REPEAT
    Script_SetOrCompareHalfword5e,       // 0x22 SW_WAIT
    Script_SetOrCompareSignedHalfword64, // 0x23
    Script_SetOrCompareSignedHalfword66, // 0x24
    Script_SetOrCompareWord68,           // 0x25
    Script_SetOrCompareUpdateAddress,    // 0x26 SW_HOOK
    Script_SetOrCompareByte62,           // 0x27
    Script_SetOrCompareByte63,           // 0x28
};
