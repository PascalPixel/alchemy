/// @file
/// Script work values: the handler SC_SET, SC_ADD and SC_CMP call for each.

#include "module/field/actor_script_operands.h"
#include "module/field/field.h"
#include "types.h"

/// The handler for each work value number (enum ActorScriptValue).
const OperandFunc gActorScript_OperandHandlerTable[41] = {
    ActorScript_SetOrCompareScriptAddress,     // 0 SW_SCRIPT
    ActorScript_SetOrCompareCursor,            // 1 SW_CURSOR
    ActorScript_SetOrCompareFacing,            // 2
    ActorScript_SetOrCompareFacing,            // 3 SW_FACING
    ActorScript_SetOrCompareFacing,            // 4
    ActorScript_SetOrCompareX,                 // 5 SW_X
    ActorScript_SetOrCompareY,                 // 6 SW_Y
    ActorScript_SetOrCompareZ,                 // 7 SW_Z
    ActorScript_SetOrCompareRadius,            // 8 SW_RADIUS
    ActorScript_SetOrCompareScaleX,            // 9 SW_SCALE_X
    ActorScript_SetOrCompareScaleY,            // 10 SW_SCALE_Y
    ActorScript_SetOrCompareScaleY,            // 11
    ActorScript_SetOrCompareVelocityX,         // 12 SW_VELOCITY_X
    ActorScript_SetOrCompareVelocityY,         // 13 SW_VELOCITY_Y
    ActorScript_SetOrCompareVelocityZ,         // 14 SW_VELOCITY_Z
    ActorScript_SetOrCompareSpeedLimit,        // 15 SW_SPEED
    ActorScript_SetOrCompareAcceleration,      // 16 SW_ACCELERATION
    ActorScript_SetOrCompareTargetX,           // 17 SW_TARGET_X
    ActorScript_SetOrCompareTargetY,           // 18 SW_TARGET_Y
    ActorScript_SetOrCompareTargetZ,           // 19 SW_TARGET_Z
    ActorScript_SetOrCompareVerticalMotion,    // 20 SW_BOUNCE
    ActorScript_SetOrCompareGravity,           // 21 SW_GRAVITY
    ActorScript_SetOrCompareTerrainHeight,     // 22 SW_GROUND
    ActorScript_SetOrCompareRiseBaseY,         // 23
    ActorScript_SetOrCompareSprite,            // 24
    ActorScript_SetOrCompareSpriteStorageKind, // 25
    ActorScript_SetOrCompareMotionFlags,       // 26 SW_MOTION
    ActorScript_SetOrCompareArrivalAxis,       // 27 SW_ARRIVAL
    ActorScript_SetOrCompareComparisonResult,  // 28 SW_RESULT
    ActorScript_SetOrCompareSnapToTarget,      // 29 SW_SNAP
    ActorScript_SetOrCompareCollisionFlags,    // 30 SW_COLLISION
    ActorScript_SetOrCompareSteeringFlags,     // 31 SW_STEERING
    ActorScript_SetOrComparePaused,            // 32 SW_PAUSED
    ActorScript_SetOrCompareRepeatCount,       // 33 SW_REPEAT
    ActorScript_SetOrCompareWait,              // 34 SW_WAIT
    ActorScript_SetOrCompareTimer,             // 35
    ActorScript_SetOrCompareUserValue,         // 36
    ActorScript_SetOrCompareWatchedActor,      // 37
    ActorScript_SetOrCompareUpdateAddress,     // 38 SW_HOOK
    ActorScript_SetOrCompareRiseCounter,       // 39 SW_RISE_COUNTER
    ActorScript_SetOrCompareRiseEnabled,       // 40 SW_RISE_ENABLED
};
