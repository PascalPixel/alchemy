/// @file
/// Script state values: the handler SC_SET, SC_ADD and SC_CMP call for each.

#include "field/actor_script_operands.h"
#include "types.h"
#include "field/field.h"

/// The handler for each state value number (enum ActorScriptValue).
const OperandFunc gActorScript_OperandHandlerTable[41] = {
    ActorScript_SetOrCompareScriptAddress,     // 0 SW_SCRIPT
    ActorScript_SetOrCompareCursor,            // 1 SW_CURSOR
    ActorScript_SetOrCompareFacing,            // 2, as SW_FACING
    ActorScript_SetOrCompareFacing,            // 3 SW_FACING
    ActorScript_SetOrCompareFacing,            // 4, as SW_FACING
    ActorScript_SetOrCompareX,                 // 5 SW_X
    ActorScript_SetOrCompareY,                 // 6 SW_Y
    ActorScript_SetOrCompareZ,                 // 7 SW_Z
    ActorScript_SetOrCompareRadius,            // 8 SW_RADIUS
    ActorScript_SetOrCompareScaleX,            // 9 SW_SCALE_X
    ActorScript_SetOrCompareScaleY,            // 10 SW_SCALE_Y
    ActorScript_SetOrCompareScaleY,            // 11, as SW_SCALE_Y
    ActorScript_SetOrCompareVelocityX,         // 12 SW_VELOCITY_X
    ActorScript_SetOrCompareVelocityY,         // 13 SW_VELOCITY_Y
    ActorScript_SetOrCompareVelocityZ,         // 14 SW_VELOCITY_Z
    ActorScript_SetOrCompareSpeedLimit,        // 15 SW_SPEED
    ActorScript_SetOrCompareAcceleration,      // 16 SW_ACCELERATION
    ActorScript_SetOrCompareTargetX,           // 17 SW_TARGET_X
    ActorScript_SetOrCompareTargetY,           // 18 SW_TARGET_Y
    ActorScript_SetOrCompareTargetZ,           // 19 SW_TARGET_Z
    ActorScript_SetOrCompareBounce,            // 20 SW_BOUNCE
    ActorScript_SetOrCompareGravity,           // 21 SW_GRAVITY
    ActorScript_SetOrCompareTerrainHeight,     // 22 SW_GROUND
    ActorScript_SetOrCompareSceneWord,         // 23 SW_SCENE_WORD
    ActorScript_SetOrCompareSprite,            // 24 SW_SPRITE
    ActorScript_SetOrCompareSpriteStorageKind, // 25 SW_SPRITE_KIND
    ActorScript_SetOrCompareMotionFlags,       // 26 SW_MOTION
    ActorScript_SetOrCompareArrivalAxis,       // 27 SW_ARRIVAL
    ActorScript_SetOrCompareComparisonResult,  // 28 SW_RESULT
    ActorScript_SetOrCompareSnapToTarget,      // 29 SW_SNAP
    ActorScript_SetOrCompareCollisionFlags,    // 30 SW_COLLISION
    ActorScript_SetOrCompareSteeringFlags,     // 31 SW_STEERING
    ActorScript_SetOrComparePaused,            // 32 SW_PAUSED
    ActorScript_SetOrCompareRepeatCount,       // 33 SW_REPEAT
    ActorScript_SetOrCompareWait,              // 34 SW_WAIT
    ActorScript_SetOrCompareTimer,             // 35 SW_TIMER
    ActorScript_SetOrCompareUserValue,         // 36 SW_USER_VALUE
    ActorScript_SetOrCompareWatchedActor,      // 37 SW_WATCHED_ACTOR
    ActorScript_SetOrCompareUpdateAddress,     // 38 SW_HOOK
    ActorScript_SetOrCompareSceneCounter,      // 39 SW_SCENE_COUNTER
    ActorScript_SetOrCompareSceneByte,         // 40 SW_SCENE_BYTE
};
