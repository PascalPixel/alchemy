/// @file
/// Map ground: the height curve of each cell kind, the world map's terrain
/// behaviour and the small tables after them, then the battle formation's
/// placement scale; then the field object scripts the field starts by itself,
/// the script command table and the work value handlers.

#include "types.h"
#include "module/field/field.h"
#include "curve.h"
#include "script.h"

//--------------------------------------------------------------------------
// Map terrain: the height curve of each cell kind, the world map's
// terrain behaviour, and the small tables after them.
//--------------------------------------------------------------------------

/// Each height function reads only the arguments it needs, so the type
/// leaves them unspecified.
typedef s32 (*TerrainHeightFn)();

s32 Terrain_GetStepDifferenceDiagonalHeight();
s32 Terrain_GetBendSumDiagonalHeight();
s32 Terrain_GetBendDifferenceDiagonalHeight();
s32 Terrain_GetRoundedSquareHeight();
s32 Terrain_GetStepXHeight();
s32 Terrain_GetStepYHeight();
s32 Terrain_GetSampleTableTransposedHeight();
s32 Terrain_GetSampleTableTransposedMirroredHeight();
s32 Terrain_GetSampleTableMirroredHeight();
s32 Terrain_GetSplitTopStepHeight();
s32 Terrain_GetSplitBottomStepHeight();

/// Ground height functions, by enum TerrainKind, called with (the remaining
/// samples, x within the cell, y within the cell). Each reads only the
/// arguments it needs.
const TerrainHeightFn gMapGrid_TerrainHeightFunctions[TERRAIN_KIND_COUNT] = {
    [TERRAIN_FLAT] = Terrain_GetFlatHeight,
    [TERRAIN_SLOPE_X] = Terrain_GetSlopeXHeight,
    [TERRAIN_SLOPE_Y] = Terrain_GetSlopeYHeight,
    [TERRAIN_STEP_SUM_DIAGONAL] = Terrain_GetStepSumDiagonalHeight,
    [TERRAIN_STEP_DIFFERENCE_DIAGONAL] =
        Terrain_GetStepDifferenceDiagonalHeight,
    [TERRAIN_BEND_SUM_DIAGONAL] = Terrain_GetBendSumDiagonalHeight,
    [TERRAIN_BEND_DIFFERENCE_DIAGONAL] =
        Terrain_GetBendDifferenceDiagonalHeight,
    [TERRAIN_ROUNDED_SQUARE] = Terrain_GetRoundedSquareHeight,
    [TERRAIN_STEP_X] = Terrain_GetStepXHeight,
    [TERRAIN_STEP_Y] = Terrain_GetStepYHeight,
    [TERRAIN_SAMPLE_TABLE_TRANSPOSED] = Terrain_GetSampleTableTransposedHeight,
    [TERRAIN_SAMPLE_TABLE_TRANSPOSED_MIRRORED] =
        Terrain_GetSampleTableTransposedMirroredHeight,
    [TERRAIN_SAMPLE_TABLE] = Terrain_GetSampleTableHeight,
    [TERRAIN_SAMPLE_TABLE_MIRRORED] = Terrain_GetSampleTableMirroredHeight,
    [TERRAIN_SPLIT_TOP_STEP] = Terrain_GetSplitTopStepHeight,
    [TERRAIN_SPLIT_BOTTOM_STEP] = Terrain_GetSplitBottomStepHeight,
};

/// The world map's terrain behaviour, read at a selector plus a row: 0x10
/// when the block word's top bit is set, 0x20 for terrain kinds above 40,
/// 0x30 for kinds 12 and 15 and 0x40 for kind 36.
// clang-format off
const u8 gWorldMap_TerrainBehaviorTable[80] = {
    0, 8, 8, 8, 10, 1, 7, 1, 1, 2, 1, 1, 1, 1, 0, 0,
    0, 8, 8, 8, 10, 4, 7, 1, 1, 2, 1, 1, 1, 1, 0, 0, // 0x10
    0, 9, 9, 9, 9, 5, 5, 5, 5, 6, 5, 5, 5, 5, 0, 0, // 0x20
    0, 8, 8, 8, 10, 3, 7, 1, 1, 2, 1, 1, 1, 1, 0, 0, // 0x30
    0, 9, 9, 9, 9, 11, 11, 11, 11, 12, 11, 11, 11, 11, 0, 0, // 0x40
};

/// Eight colours (BGR555).
const u16 gData_0802f054[8] = {
    0x0000, 0x7c00, 0x03e0, 0x001f, 0x1ce7, 0x3def, 0x7fff, 0x000f,
};

/// Stops for a number from 0 to 715 that steps by one or skips: a skip
/// forward goes to the next stop above it, back to the last stop below.
/// 0xffff ends the list.
const u16 gData_0802f064[10] = {
    0, 16, 243, 247, 250, 490, 526, 642, 671, 0xffff,
};
// clang-format on

/// Eight directions at a radius of 40 (x, y).
const s8 gWorldMap_DirectionOffsets[8][2] = {
    {0, -40}, {28, -28}, {40, 0},  {28, 28},
    {0, 40},  {-28, 28}, {-40, 0}, {-28, -28},
};

/// Two 1.0s (Q16).
const u32 gWorldMap_UnitScale[2] = {
    0x10000,
    0x10000,
};

/// The battle formation's placement scale (Q16).
const s32 gBattle_FormationPlacementScale[2] = {
    0x10000,
    0x10000,
};

#include "objcmd.h"

//--------------------------------------------------------------------------
// Field object scripts: the ones the field starts by itself, the command
// table and the work value handlers SC_SET, SC_ADD and SC_CMP call.
//--------------------------------------------------------------------------

typedef void (*OperandFunc)(struct ScriptOperands *state, s32 operation,
                            s32 value);

s32 ActorScript_Wait();
s32 Script_WaitForArrival();
s32 ActorScript_SetPositionAndResetMotion();
s32 ActorScript_ResetToOrigin();
s32 ActorScript_ApplyAbsolutePosition();
s32 ActorScript_ApplyRelativePosition();
s32 ActorScript_StoreAngleToWatchedActor();
s32 ActorScript_ApplyWatchedActorPosition();
s32 ActorScript_ApplyLocalOffsetPosition();
s32 Actor_Wander();
s32 ActorScript_WanderNearHome();
s32 Actor_ApproachWatchedActor();
s32 FieldCamera_FollowLeaderInBounds();
s32 ActorScript_RepeatOrJump();
s32 ActorScript_JumpToLabel();
u32 ActorScript_JumpIfTrue();
u32 ActorScript_JumpIfFalse();
s32 ActorScript_End();
s32 ActorScript_TestFlag();
s32 ActorScript_SetFlagAndTest();
s32 ActorScript_ClearFlagAndTest();
s32 ActorScript_ToggleFlagAndTest();
s32 ActorScript_ApplyOperandSet();
s32 ActorScript_ApplyOperandAdd();
s32 ActorScript_ApplyOperandCompare();
s32 Actor_UpdatePlayerControl();
s32 Actor_MoveOnWorldMap();
s32 Actor_MoveByKeys();
s32 Actor_CrossRope();
s32 Actor_CrossTightrope();
s32 Actor_MoveAsSand();
s32 Actor_SailOnMap();
s32 Actor_SailOnWorldMap();
s32 Actor_FlyOnWorldMap();
s32 Actor_MoveAsSandOnWorldMap();
s32 Actor_UpdateHoverControl();
s32 Actor_UpdateWalkingControl();
s32 Field_CheckConfiguredKeysAndCount();
s32 Actor_ReleaseAndClear();
s32 ActorScript_SetAnimation();
s32 ActorScript_Nop();
s32 Script_PlaySound();
s32 Script_CallWithOperand();
s32 ActorScript_InvokeCallback();
s32 FieldCamera_FollowWatchedInBounds();
s32 ActorScript_SetMoveTargetXZ();

/// Each runs one control command every frame: the command, SC_WAIT(1),
/// SC_LOOP(SC_FOREVER, SC_TOP).
const s32 gActor_KeyCheckScript[] = {
    SC_CMD_25,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_KeysScript[] = {
    SC_CMD_1B,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_RopeScript[] = {
    SC_CMD_1C,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_TightropeScript[] = {
    SC_CMD_1D,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_SandScript[] = {
    SC_CMD_1E,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_HoverScript[] = {
    SC_CMD_23,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_WalkingScript[] = {
    SC_CMD_24,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_PlayerScript[] = {
    SC_PLAYER,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_WorldMapScript[] = {
    SC_CMD_1A,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_SailOnMapScript[] = {
    SC_CMD_1F,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_SailScript[] = {
    SC_CMD_20,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_FlyScript[] = {
    SC_CMD_21,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_SandOnWorldMapScript[] = {
    SC_CMD_22,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_CameraScript[] = {
    SC_CAMERA,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_ApproachWatchedScript[] = {
    SC_APPROACH_WATCHED,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActorScript_EndScript[] = {
    SC_END,
};

/// The handler of each command. 0x31 to 0x35 repeat SC_END's.
const ScriptCommandFn gData_0802f204[54] = {
    ActorScript_Wait,                         // 0x00 SC_WAIT
    Script_WaitForArrival,                    // 0x01 SC_SYNC
    ActorScript_SetPositionAndResetMotion,    // 0x02 SC_POSITION
    ActorScript_ResetToOrigin,                // 0x03 SC_CMD_03
    ActorScript_ApplyAbsolutePosition,        // 0x04 SC_MOVE
    ActorScript_ApplyRelativePosition,        // 0x05 SC_MOVE_BY
    ActorScript_StoreAngleToWatchedActor,     // 0x06 SC_FACE_WATCHED
    ActorScript_ApplyWatchedActorPosition,    // 0x07 SC_MOVE_TO_WATCHED
    ActorScript_ApplyLocalOffsetPosition,     // 0x08 SC_MOVE_LOCAL
    Actor_Wander,                             // 0x09 SC_WANDER
    ActorScript_WanderNearHome,               // 0x0a SC_WANDER_HOME
    Actor_ApproachWatchedActor,               // 0x0b SC_APPROACH_WATCHED
    FieldCamera_FollowLeaderInBounds,         // 0x0c SC_CAMERA
    ActorScript_RepeatOrJump,                 // 0x0d SC_LOOP
    ActorScript_JumpToLabel,                  // 0x0e SC_JUMP
    (ScriptCommandFn)ActorScript_JumpIfTrue,  // 0x0f SC_IF
    (ScriptCommandFn)ActorScript_JumpIfFalse, // 0x10 SC_IFNOT
    ActorScript_End,                          // 0x11 SC_END
    ActorScript_TestFlag,                     // 0x12 SC_FLAG_CHECK
    ActorScript_SetFlagAndTest,               // 0x13 SC_FLAG_ON
    ActorScript_ClearFlagAndTest,             // 0x14 SC_FLAG_OFF
    ActorScript_ToggleFlagAndTest,            // 0x15 SC_FLAG_REV
    ActorScript_ApplyOperandSet,              // 0x16 SC_SET
    ActorScript_ApplyOperandAdd,              // 0x17 SC_ADD
    ActorScript_ApplyOperandCompare,          // 0x18 SC_CMP
    Actor_UpdatePlayerControl,                // 0x19 SC_PLAYER
    Actor_MoveOnWorldMap,                     // 0x1a SC_CMD_1A
    Actor_MoveByKeys,                         // 0x1b SC_KEYS
    Actor_CrossRope,                          // 0x1c SC_CMD_1C
    Actor_CrossTightrope,                     // 0x1d SC_CMD_1D
    Actor_MoveAsSand,                         // 0x1e SC_CMD_1E
    Actor_SailOnMap,                          // 0x1f SC_CMD_1F
    Actor_SailOnWorldMap,                     // 0x20 SC_CMD_20
    Actor_FlyOnWorldMap,                      // 0x21 SC_CMD_21
    Actor_MoveAsSandOnWorldMap,               // 0x22 SC_CMD_22
    Actor_UpdateHoverControl,                 // 0x23 SC_CMD_23
    Actor_UpdateWalkingControl,               // 0x24 SC_CMD_24
    Field_CheckConfiguredKeysAndCount,        // 0x25 SC_CMD_25
    Actor_ReleaseAndClear,                    // 0x26 SC_FREE
    ActorScript_SetAnimation,                 // 0x27 SC_ARG
    ActorScript_Nop,                          // 0x28 SC_NOP
    Script_PlaySound,                         // 0x29 SC_SE
    Script_CallWithOperand,                   // 0x2a SC_CMD_2A
    Script_JumpToNextOperandPointer,          // 0x2b SC_GOTO
    ActorScript_HideSprites,                  // 0x2c SC_B54_OFF
    ActorScript_ShowSprite,                   // 0x2d SC_B54_ON
    ActorScript_InvokeCallback,               // 0x2e SC_CALL
    FieldCamera_FollowWatchedInBounds,        // 0x2f SC_CMD_2F
    ActorScript_SetMoveTargetXZ,              // 0x30 SC_MOVE_XZ
    ActorScript_End,                          // 0x31
    ActorScript_End,                          // 0x32
    ActorScript_End,                          // 0x33
    ActorScript_End,                          // 0x34
    ActorScript_End,                          // 0x35
};

/// The handler of each work value (enum ScriptWork in objcmd.h).
const OperandFunc gActorScript_OperandHandlerTable[41] = {
    ActorScript_SetOrCompareScriptAddress,     // 0x00 SW_SCRIPT
    ActorScript_SetOrCompareCursor,            // 0x01 SW_CURSOR
    ActorScript_SetOrCompareFacing,            // 0x02
    ActorScript_SetOrCompareFacing,            // 0x03 SW_FACING
    ActorScript_SetOrCompareFacing,            // 0x04
    ActorScript_SetOrCompareX,                 // 0x05 SW_X
    ActorScript_SetOrCompareY,                 // 0x06 SW_Y
    ActorScript_SetOrCompareZ,                 // 0x07 SW_Z
    ActorScript_SetOrCompareRadius,            // 0x08 SW_RADIUS
    ActorScript_SetOrCompareScaleX,            // 0x09 SW_SCALE_X
    ActorScript_SetOrCompareScaleY,            // 0x0a SW_SCALE_Y
    ActorScript_SetOrCompareScaleY,            // 0x0b
    ActorScript_SetOrCompareVelocityX,         // 0x0c SW_VELOCITY_X
    ActorScript_SetOrCompareVelocityY,         // 0x0d SW_VELOCITY_Y
    ActorScript_SetOrCompareVelocityZ,         // 0x0e SW_VELOCITY_Z
    ActorScript_SetOrCompareSpeedLimit,        // 0x0f SW_SPEED
    ActorScript_SetOrCompareAcceleration,      // 0x10 SW_ACCEL
    ActorScript_SetOrCompareTargetX,           // 0x11 SW_TARGET_X
    ActorScript_SetOrCompareTargetY,           // 0x12 SW_TARGET_Y
    ActorScript_SetOrCompareTargetZ,           // 0x13 SW_TARGET_Z
    ActorScript_SetOrCompareVerticalMotion,    // 0x14 SW_BOUNCE
    ActorScript_SetOrCompareGravity,           // 0x15 SW_GRAVITY
    ActorScript_SetOrCompareTerrainHeight,     // 0x16 SW_GROUND
    ActorScript_SetOrCompareRiseBaseY,         // 0x17
    ActorScript_SetOrCompareSprite,            // 0x18
    ActorScript_SetOrCompareSpriteStorageKind, // 0x19
    ActorScript_SetOrCompareMotionFlags,       // 0x1a SW_MOTION
    ActorScript_SetOrCompareArrivalAxis,       // 0x1b SW_ARRIVAL
    ActorScript_SetOrCompareComparisonResult,  // 0x1c SW_RESULT
    ActorScript_SetOrCompareSnapToTarget,      // 0x1d SW_SNAP
    ActorScript_SetOrCompareCollisionFlags,    // 0x1e SW_COLLISION
    ActorScript_SetOrCompareSteeringFlags,     // 0x1f SW_STEERING
    ActorScript_SetOrComparePaused,            // 0x20 SW_PAUSED
    ActorScript_SetOrCompareRepeatCount,       // 0x21 SW_REPEAT
    ActorScript_SetOrCompareWait,              // 0x22 SW_WAIT
    ActorScript_SetOrCompareTimer,             // 0x23
    ActorScript_SetOrCompareUserValue,         // 0x24
    ActorScript_SetOrCompareWatchedActor,      // 0x25
    ActorScript_SetOrCompareUpdateAddress,     // 0x26 SW_HOOK
    ActorScript_SetOrCompareRiseCounter,       // 0x27
    ActorScript_SetOrCompareRiseEnabled,       // 0x28 SW_BYTE_63
};
