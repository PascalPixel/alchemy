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

// ---- field/map/maps.c ----

/// Each map's resources, as offsets from ResourceId_Map000, by map: its BG
/// map, palette and four tile sets.

#include "types.h"

// clang-format off
const u16 gMaps[][6] = {
    {0, 1, 2, 3, 4, 5},
    {0, 1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10, 11},
    {12, 13, 14, 15, 16, 17},
    {18, 19, 20, 21, 22, 23},
    {24, 25, 26, 27, 28, 29},
    {30, 25, 26, 27, 28, 29},
    {31, 32, 33, 34, 35, 36},
    {37, 38, 39, 40, 41, 42},
    {43, 44, 45, 46, 47, 48},
    {49, 44, 45, 46, 47, 48},
    {50, 51, 52, 53, 54, 55},
    {56, 57, 58, 59, 60, 61},
    {62, 63, 64, 65, 66, 67},
    {68, 63, 64, 65, 66, 67},
    {69, 63, 64, 65, 66, 67},
    {70, 63, 64, 65, 66, 67},
    {71, 63, 64, 65, 66, 67},
    {72, 63, 64, 65, 66, 67},
    {73, 63, 64, 65, 66, 67},
    {74, 75, 45, 76, 47, 48},
    {77, 78, 79, 80, 81, 82},
    {83, 84, 85, 59, 60, 61},
    {86, 84, 85, 59, 60, 61},
    {87, 84, 85, 59, 60, 61},
    {88, 84, 85, 59, 60, 61},
    {89, 84, 85, 59, 60, 61},
    {90, 84, 85, 59, 60, 61},
    {91, 92, 85, 59, 60, 61},
    {93, 84, 85, 59, 60, 61},
    {94, 84, 85, 59, 60, 61},
    {95, 96, 97, 98, 99, 100},
    {101, 102, 103, 104, 105, 106},
    {107, 108, 109, 110, 111, 112},
    {113, 114, 115, 116, 117, 118},
    {119, 120, 121, 122, 123, 124},
    {125, 126, 127, 128, 129, 130},
    {131, 132, 133, 134, 135, 136},
    {137, 138, 139, 140, 141, 142},
    {143, 144, 145, 146, 147, 148},
    {149, 150, 151, 152, 153, 154},
    {155, 156, 157, 158, 159, 160},
    {161, 162, 163, 164, 165, 166},
    {167, 168, 169, 170, 171, 172},
    {173, 174, 175, 176, 177, 178},
    {179, 180, 181, 182, 183, 184},
    {185, 186, 187, 188, 189, 190},
    {191, 192, 187, 188, 189, 190},
    {193, 194, 187, 188, 189, 190},
    {195, 196, 197, 198, 199, 200},
    {201, 202, 203, 204, 205, 206},
    {207, 208, 197, 209, 210, 200},
    {211, 212, 213, 214, 215, 216},
    {217, 218, 219, 220, 221, 222},
    {223, 224, 225, 226, 227, 228},
    {229, 230, 197, 209, 210, 200},
    {231, 232, 197, 209, 210, 200},
    {233, 232, 197, 209, 210, 200},
    {234, 235, 79, 80, 81, 82},
    {236, 237, 79, 80, 81, 82},
    {223, 238, 225, 226, 227, 228},
    {239, 240, 219, 220, 221, 222},
    {161, 241, 163, 164, 165, 166},
    {242, 243, 244, 245, 246, 247},
    {248, 243, 244, 245, 246, 247},
    {249, 243, 244, 245, 246, 247},
    {250, 243, 244, 245, 246, 247},
    {251, 243, 244, 245, 246, 247},
    {252, 243, 244, 245, 246, 247},
    {253, 243, 244, 245, 246, 247},
    {254, 243, 244, 245, 246, 247},
    {255, 243, 244, 245, 246, 247},
    {256, 243, 244, 245, 246, 247},
    {257, 243, 244, 245, 246, 247},
    {258, 243, 244, 245, 246, 247},
    {259, 260, 261, 245, 246, 247},
    {262, 263, 261, 245, 246, 264},
    {265, 266, 261, 245, 246, 267},
    {268, 269, 270, 271, 272, 273},
    {274, 269, 270, 271, 272, 273},
    {275, 269, 270, 271, 272, 273},
    {276, 277, 278, 279, 280, 281},
    {282, 277, 278, 279, 280, 281},
    {283, 277, 278, 279, 280, 281},
    {284, 277, 278, 279, 280, 281},
    {285, 277, 278, 279, 280, 281},
    {286, 277, 278, 279, 280, 281},
    {287, 277, 278, 279, 280, 281},
    {288, 277, 278, 279, 280, 281},
    {289, 277, 278, 279, 280, 281},
    {290, 291, 292, 293, 294, 295},
    {296, 297, 151, 152, 153, 154},
    {298, 299, 157, 158, 159, 300},
    {301, 299, 157, 158, 159, 300},
    {302, 303, 304, 305, 306, 307},
    {308, 309, 310, 311, 312, 313},
    {314, 315, 316, 317, 318, 319},
    {320, 321, 316, 317, 318, 319},
    {322, 323, 324, 325, 326, 327},
    {328, 329, 330, 331, 332, 333},
    {334, 335, 336, 337, 338, 339},
    {340, 335, 336, 337, 338, 339},
    {341, 342, 343, 344, 345, 346},
    {347, 348, 343, 344, 345, 346},
    {349, 350, 351, 352, 353, 354},
    {355, 356, 357, 358, 359, 360},
    {361, 362, 363, 364, 365, 366},
    {367, 368, 369, 370, 371, 372},
    {373, 374, 375, 376, 377, 378},
    {379, 380, 330, 331, 332, 333},
    {381, 382, 383, 384, 385, 386},
    {387, 388, 383, 384, 385, 386},
    {389, 390, 383, 384, 385, 386},
    {391, 392, 383, 384, 385, 386},
    {393, 394, 383, 384, 385, 386},
    {395, 396, 383, 384, 385, 386},
    {397, 398, 399, 400, 401, 402},
    {403, 404, 399, 400, 401, 402},
    {405, 406, 407, 408, 409, 410},
    {411, 412, 399, 400, 401, 402},
    {413, 414, 415, 416, 417, 418},
    {419, 420, 421, 422, 423, 424},
    {425, 420, 421, 422, 423, 424},
    {426, 420, 421, 422, 423, 424},
    {427, 420, 421, 422, 423, 424},
    {428, 420, 421, 422, 423, 424},
    {429, 430, 431, 432, 433, 434},
    {435, 436, 310, 311, 312, 313},
    {437, 436, 310, 311, 312, 313},
    {438, 439, 440, 441, 442, 443},
    {444, 445, 197, 198, 210, 200},
    {446, 447, 45, 46, 47, 48},
    {448, 449, 450, 451, 452, 453},
    {454, 455, 197, 198, 199, 200},
    {456, 457, 458, 459, 460, 461},
    {462, 463, 163, 164, 165, 166},
    {211, 464, 213, 214, 215, 216},
    {465, 466, 467, 468, 469, 470},
    {471, 472, 473, 474, 475, 476},
    {477, 478, 479, 480, 481, 482},
    {483, 478, 479, 480, 481, 482},
    {484, 478, 479, 480, 481, 482},
    {485, 478, 479, 480, 481, 482},
    {486, 478, 479, 480, 481, 482},
    {487, 478, 479, 480, 481, 482},
    {488, 489, 479, 480, 481, 482},
    {490, 489, 479, 480, 481, 482},
    {491, 489, 479, 480, 481, 482},
    {492, 493, 494, 495, 496, 497},
    {498, 499, 500, 501, 502, 503},
    {504, 505, 506, 507, 508, 509},
    {510, 511, 512, 513, 514, 515},
    {516, 517, 518, 519, 520, 521},
    {522, 517, 518, 519, 520, 521},
    {523, 524, 525, 526, 527, 528},
    {529, 524, 525, 526, 527, 528},
    {530, 524, 525, 526, 527, 528},
    {531, 524, 525, 526, 527, 528},
    {532, 524, 525, 526, 527, 528},
    {533, 524, 525, 526, 527, 528},
    {534, 291, 292, 293, 294, 295},
    {535, 524, 525, 526, 527, 528},
    {536, 524, 525, 526, 527, 528},
    {537, 538, 278, 279, 280, 281},
    {539, 540, 541, 542, 543, 544},
    {545, 546, 547, 548, 549, 550},
    {551, 552, 79, 80, 81, 553},
    {554, 555, 79, 80, 81, 556},
    {557, 558, 270, 271, 272, 273},
    {559, 558, 270, 271, 272, 273},
    {560, 558, 270, 271, 272, 273},
    {561, 562, 563, 564, 565, 566},
    {567, 568, 569, 570, 571, 572},
    {573, 568, 569, 570, 571, 572},
    {574, 568, 569, 570, 571, 572},
    {575, 568, 569, 570, 571, 572},
    {576, 577, 541, 542, 543, 544},
    {578, 579, 547, 548, 549, 550},
    {580, 581, 582, 583, 584, 585},
    {586, 581, 582, 583, 584, 585},
    {587, 581, 582, 583, 584, 585},
    {588, 581, 582, 583, 584, 585},
    {589, 590, 225, 226, 227, 228},
    {591, 592, 593, 594, 595, 596},
    {597, 598, 599, 600, 601, 602},
    {603, 430, 604, 605, 606, 607},
    {608, 609, 97, 98, 99, 100},
    {610, 609, 97, 98, 99, 100},
    {611, 609, 97, 98, 99, 100},
    {612, 609, 97, 98, 99, 100},
    {95, 609, 97, 98, 99, 100},
    {613, 609, 97, 98, 99, 100},
    {614, 615, 103, 104, 105, 106},
    {101, 616, 103, 104, 105, 106},
    {603, 617, 604, 605, 606, 607},
    {618, 619, 213, 214, 215, 216},
    {620, 621, 622, 623, 624, 625},
    {626, 627, 622, 623, 624, 625},
    {628, 629, 630, 631, 632, 633},
    {634, 635, 636, 637, 638, 639},
    {640, 641, 642, 643, 644, 645},
    {646, 647, 97, 98, 99, 100},
    {648, 649, 97, 98, 99, 100},
    {650, 651, 97, 98, 99, 100},
    {652, 653, 97, 98, 99, 100},
    {654, 655, 97, 98, 99, 100},
    {656, 657, 103, 104, 658, 106},
    {659, 660, 103, 104, 658, 106},
    {661, 662, 663, 664, 665, 666},
    {667, 668, 669, 670, 671, 672},
    {673, 674, 324, 325, 326, 327},
    {675, 676, 677, 678, 679, 680},
    {681, 682, 683, 684, 685, 686},
    {687, 688, 689, 690, 691, 692},
    {693, 694, 270, 271, 272, 273},
    {695, 694, 270, 271, 272, 273},
    {696, 694, 270, 271, 272, 273},
    {697, 698, 525, 526, 527, 528},
    {699, 698, 525, 526, 527, 528},
    {700, 698, 525, 526, 527, 528},
    {701, 698, 525, 526, 527, 528},
    {702, 698, 525, 526, 527, 528},
    {703, 698, 525, 526, 527, 528},
    {704, 698, 525, 526, 527, 528},
    {705, 706, 278, 279, 280, 281},
    {707, 708, 604, 605, 606, 709},
    {710, 711, 712, 713, 714, 715},
    {716, 717, 712, 713, 714, 718},
    {719, 720, 721, 722, 723, 724},
    {719, 725, 721, 722, 723, 724},
    {726, 727, 663, 664, 665, 666},
    {728, 729, 730, 731, 732, 733},
    {734, 735, 736, 737, 738, 739},
    {740, 741, 736, 737, 738, 739},
    {742, 743, 744, 745, 746, 747},
    {742, 748, 744, 745, 746, 747},
    {223, 749, 225, 226, 227, 228},
    {750, 751, 97, 98, 752, 100},
    {753, 751, 97, 98, 752, 100},
    {754, 751, 97, 98, 752, 100},
    {755, 751, 97, 98, 752, 100},
    {756, 751, 97, 98, 752, 100},
    {757, 751, 97, 98, 752, 100},
    {758, 759, 760, 761, 762, 763},
    {764, 759, 760, 761, 762, 763},
    {765, 766, 767, 768, 769, 770},
    {771, 772, 773, 774, 775, 776},
    {777, 778, 779, 780, 781, 782},
    {783, 784, 79, 80, 81, 82},
    {785, 291, 292, 293, 294, 295},
    {786, 787, 788, 789, 790, 791},
    {792, 787, 788, 789, 790, 791},
    {793, 787, 788, 789, 790, 791},
    {794, 787, 788, 789, 790, 791},
    {795, 787, 788, 789, 790, 791},
    {796, 787, 788, 789, 790, 791},
    {797, 787, 788, 789, 790, 791},
    {798, 787, 788, 789, 790, 791},
    {799, 787, 788, 789, 790, 791},
    {800, 801, 779, 780, 781, 782},
    {802, 803, 779, 780, 781, 782},
    {804, 805, 806, 807, 808, 809},
    {810, 811, 812, 813, 814, 815},
    {816, 817, 599, 600, 601, 602},
    {818, 819, 820, 821, 822, 823},
    {824, 825, 820, 821, 822, 823},
    {826, 827, 820, 821, 822, 823},
    {828, 688, 689, 690, 691, 692},
    {161, 829, 163, 164, 165, 166},
    {223, 830, 225, 226, 227, 228},
    {831, 832, 779, 780, 781, 833},
    {834, 835, 836, 837, 838, 839},
    {840, 835, 836, 837, 838, 839},
    {841, 835, 836, 837, 838, 839},
    {842, 835, 836, 837, 838, 839},
    {843, 844, 779, 845, 781, 846},
    {847, 844, 779, 780, 781, 848},
    {849, 850, 836, 851, 838, 852},
    {853, 835, 836, 837, 838, 839},
    {854, 835, 836, 837, 838, 839},
    {855, 835, 836, 837, 838, 839},
    {856, 835, 836, 837, 838, 839},
    {857, 835, 836, 837, 838, 839},
    {858, 835, 836, 837, 838, 839},
    {859, 835, 836, 837, 838, 839},
    {860, 844, 779, 780, 781, 861},
    {862, 844, 779, 780, 781, 861},
    {863, 844, 779, 780, 781, 861},
    {864, 844, 779, 780, 781, 861},
    {831, 865, 779, 780, 781, 833},
    {834, 866, 836, 837, 838, 839},
    {840, 866, 836, 837, 838, 839},
    {841, 866, 836, 837, 838, 839},
    {842, 866, 836, 837, 838, 839},
    {843, 867, 779, 780, 781, 846},
    {847, 868, 779, 780, 781, 848},
    {849, 869, 836, 837, 838, 852},
    {853, 866, 836, 837, 838, 839},
    {854, 866, 836, 837, 838, 839},
    {855, 866, 836, 837, 838, 839},
    {856, 866, 836, 837, 838, 839},
    {857, 866, 836, 837, 838, 839},
    {858, 866, 836, 837, 838, 839},
    {859, 866, 836, 837, 838, 839},
    {860, 867, 779, 780, 781, 861},
    {862, 867, 779, 780, 781, 861},
    {863, 867, 779, 780, 781, 861},
    {864, 867, 779, 780, 781, 861},
    {870, 871, 872, 873, 874, 875},
    {876, 877, 878, 879, 880, 881},
    {882, 883, 884, 885, 886, 887},
    {888, 889, 890, 891, 892, 893},
    {894, 895, 878, 879, 880, 881},
    {896, 895, 878, 879, 880, 881},
    {897, 898, 899, 900, 901, 902},
    {903, 904, 187, 188, 189, 190},
    {905, 906, 187, 188, 189, 190},
    {907, 908, 139, 140, 141, 142},
    {909, 910, 911, 912, 913, 914},
    {915, 916, 878, 879, 880, 881},
    {917, 918, 919, 920, 921, 922},
    {923, 924, 925, 926, 927, 928},
    {929, 930, 931, 932, 933, 934},
    {935, 936, 937, 938, 939, 940},
    {941, 942, 943, 944, 945, 946},
    {947, 948, 949, 950, 951, 952},
    {953, 954, 955, 956, 957, 958},
    {959, 960, 961, 962, 963, 964},
    {965, 966, 967, 968, 969, 970},
    {971, 972, 973, 974, 975, 976},
    {977, 978, 979, 980, 981, 982},
    {983, 984, 985, 986, 987, 988},
};
// clang-format on

/// Unused: nothing reads these four words after the map table, which The
/// Broken Seal keeps too.
const u32 gMapsTail[] = {0, 3, 7, 9};
