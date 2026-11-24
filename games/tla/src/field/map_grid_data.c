/// @file
/// Map ground: the height function of each cell kind, the world map's battle
/// environments and the sprite viewer's small tables after them; then the
/// actor scripts the field starts by itself, the actor script command table
/// and the work value handlers. Each map's resources follow in map_resources.s.

#include "types.h"
#include "system/io_reg.h"
#include "field/field.h"
#include "field/curve.h"
#include "field/script_operands.h"
#include "field/actor_script.h"
#include "field/map_grid.h"
#include "sprite_ids.h"

//--------------------------------------------------------------------------
// Map terrain: the height function of each cell kind, the world map's
// battle environments, and the small tables after them.
//--------------------------------------------------------------------------

/// Each height function reads only the arguments it needs, so the type
/// leaves them unspecified.
typedef s32 (*TerrainHeightFn)();

FIXED Terrain_GetStepDifferenceDiagonalHeight();
FIXED Terrain_GetBendSumDiagonalHeight();
FIXED Terrain_GetBendDifferenceDiagonalHeight();
FIXED Terrain_GetRoundedSquareHeight();
FIXED Terrain_GetStepXHeight();
FIXED Terrain_GetStepYHeight();
FIXED Terrain_GetSampleTableTransposedHeight();
FIXED Terrain_GetSampleTableTransposedMirroredHeight();
FIXED Terrain_GetSampleTableMirroredHeight();
FIXED Terrain_GetSplitTopStepHeight();
FIXED Terrain_GetSplitBottomStepHeight();

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

/// The world map's battle environments, read at a collision value plus a set
/// of rows (WorldMap_GetBattleEnvironment): the plain rows; the beach rows for
/// a cell with its beach bit; the snowfield rows for the encounter regions from
/// SNOWFIELD_REGION_FIRST; the desert rows for the regions round Air's Rock and
/// north of Shaman Village; the blizzard rows for the region round Prox.
// clang-format off
// ENV(name) is BATTLE_ENVIRONMENT_name, to keep each row's sixteen short.
#define ENV(name) BATTLE_ENVIRONMENT_##name
/// The World Map's battle environments, by collision value, sixteen to each of
/// its sets of rows.
const u8 gWorldMap_TerrainBehaviorTable[80] = {
    // plain
    ENV(NONE), ENV(SEA), ENV(SEA), ENV(SEA), ENV(SEA_LAST), ENV(GRASSLAND),
    ENV(SHALLOWS), ENV(GRASSLAND), ENV(GRASSLAND), ENV(FOREST), ENV(GRASSLAND),
    ENV(GRASSLAND), ENV(GRASSLAND), ENV(GRASSLAND), ENV(NONE), ENV(NONE),
    // beach
    ENV(NONE), ENV(SEA), ENV(SEA), ENV(SEA), ENV(SEA_LAST), ENV(BEACH),
    ENV(SHALLOWS), ENV(GRASSLAND), ENV(GRASSLAND), ENV(FOREST), ENV(GRASSLAND),
    ENV(GRASSLAND), ENV(GRASSLAND), ENV(GRASSLAND), ENV(NONE), ENV(NONE),
    // snowfield
    ENV(NONE), ENV(SNOWFIELD_SEA), ENV(SNOWFIELD_SEA), ENV(SNOWFIELD_SEA),
    ENV(SNOWFIELD_SEA), ENV(SNOWFIELD), ENV(SNOWFIELD), ENV(SNOWFIELD),
    ENV(SNOWFIELD), ENV(SNOW_FOREST), ENV(SNOWFIELD), ENV(SNOWFIELD),
    ENV(SNOWFIELD), ENV(SNOWFIELD), ENV(NONE), ENV(NONE),
    // desert
    ENV(NONE), ENV(SEA), ENV(SEA), ENV(SEA), ENV(SEA_LAST), ENV(DESERT),
    ENV(SHALLOWS), ENV(GRASSLAND), ENV(GRASSLAND), ENV(FOREST), ENV(GRASSLAND),
    ENV(GRASSLAND), ENV(GRASSLAND), ENV(GRASSLAND), ENV(NONE), ENV(NONE),
    // blizzard
    ENV(NONE), ENV(SNOWFIELD_SEA), ENV(SNOWFIELD_SEA), ENV(SNOWFIELD_SEA),
    ENV(SNOWFIELD_SEA), ENV(BLIZZARD), ENV(BLIZZARD), ENV(BLIZZARD),
    ENV(BLIZZARD), ENV(BLIZZARD_FOREST), ENV(BLIZZARD), ENV(BLIZZARD),
    ENV(BLIZZARD), ENV(BLIZZARD), ENV(NONE), ENV(NONE),
};
#undef ENV

/// The sprite viewer's backdrops, which B steps through: black, blue, green,
/// red, dark grey, grey, white and dark red.
const u16 gSpriteViewer_BackdropColors[8] = {
    RGB(0, 0, 0),    RGB(0, 0, 31),   RGB(0, 31, 0),
    RGB(31, 0, 0),   RGB(7, 7, 7),    RGB(15, 15, 15),
    RGB(31, 31, 31), RGB(15, 0, 0),
};

/// The sprite ids the sprite viewer's fast step (Start held with L or R)
/// stops at, the first of each group of sprites: a skip forward goes to the
/// next stop above the sprite shown, back to the last stop below it. (u16)-1
/// ends the list.
const u16 gSpriteViewer_SkipStops[10] = {
    SPRITE_ISAAC,              // the party
    SPRITE_BRIGGS,             // the people
    SPRITE_VENUS_DJINNI,       // the Djinn
    SPRITE_PUNCH_ANT,          // monsters on the field
    SPRITE_VINE_PILLAR,        // things on the field
    SPRITE_BATTLE_ISAAC,       // the party in battle, and their weapons
    SPRITE_BATTLE_BLUE_DRAGON, // monsters in battle
    SPRITE_THOR,               // the summons
    SPRITE_STAR_BURST,         // effects
    (u16)-1,
};
// clang-format on

/// Unused: nothing refers to it. Eight directions at a radius of 40 (x, y).
const s8 gWorldMap_DirectionOffsets[8][2] = {
    {0, -40}, {28, -28}, {40, 0},  {28, 28},
    {0, 40},  {-28, 28}, {-40, 0}, {-28, -28},
};

/// Unused: nothing refers to it. A scale of 1 on both axes (16.16).
const u32 gWorldMap_UnitScale[2] = {
    FIXED_ONE,
    FIXED_ONE,
};

/// The sprite viewer's scale for its ring of copies: full size.
const struct SpriteScale gSpriteViewer_Scale = {{{FIXED_ONE, FIXED_ONE}}};

//--------------------------------------------------------------------------
// Actor scripts: the ones the field starts by itself, the command table and
// the work value handlers SC_SET, SC_ADD and SC_CMP call.
//--------------------------------------------------------------------------

s32 ActorScript_Wait();
s32 ActorScript_WaitForArrival();
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
s32 ActorScript_Release();
s32 ActorScript_SetAnimation();
s32 ActorScript_Nop();
s32 ActorScript_PlaySound();
s32 ActorScript_PlaySoundInView();
s32 ActorScript_InvokeCallback();
s32 FieldCamera_FollowWatchedInBounds();
s32 ActorScript_SetMoveTargetXZ();

/// Each runs one control command every frame: the command, SC_WAIT(1),
/// SC_LOOP(SC_FOREVER, SC_TOP).
const s32 gActor_KeyCheckScript[] = {
    SC_KEY_COUNT,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Climbing by the keys (Actor_MoveByKeys).
const s32 gActor_KeysScript[] = {
    SC_KEYS,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Crossing a tied rope (Actor_CrossRope).
const s32 gActor_RopeScript[] = {
    SC_ROPE,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Crossing a tightrope (Actor_CrossTightrope).
const s32 gActor_TightropeScript[] = {
    SC_TIGHTROPE,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Moving as sand (Actor_MoveAsSand).
const s32 gActor_SandScript[] = {
    SC_SAND,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Hovering (Actor_UpdateHoverControl).
const s32 gActor_HoverScript[] = {
    SC_HOVER,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Walking (Actor_UpdateWalkingControl).
const s32 gActor_WalkingScript[] = {
    SC_WALK,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The leader on foot (Actor_UpdatePlayerControl).
const s32 gActor_PlayerScript[] = {
    SC_PLAYER,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The leader on the World Map (Actor_MoveOnWorldMap).
const s32 gActor_WorldMapScript[] = {
    SC_WORLD,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Sailing on a map (Actor_SailOnMap).
const s32 gActor_SailOnMapScript[] = {
    SC_SAIL,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Sailing on the World Map (Actor_SailOnWorldMap).
const s32 gActor_SailScript[] = {
    SC_WORLD_SAIL,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Flying on the World Map (Actor_FlyOnWorldMap).
const s32 gActor_FlyScript[] = {
    SC_WORLD_FLY,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Moving as sand on the World Map (Actor_MoveAsSandOnWorldMap).
const s32 gActor_SandOnWorldMapScript[] = {
    SC_WORLD_SAND,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The camera following the leader within the map's bounds.
const s32 gActor_CameraScript[] = {
    SC_CAMERA,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Following the watched actor (Actor_ApproachWatchedActor).
const s32 gActor_ApproachWatchedScript[] = {
    SC_APPROACH_WATCHED,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A script that ends at once.
const s32 gActorScript_EndScript[] = {
    SC_END,
};

/// The handler of each command. 0x31 to 0x35 repeat SC_END's.
const ScriptCommandFn gActorScript_CommandTable[54] = {
    ActorScript_Wait,                         // 0x00 SC_WAIT
    ActorScript_WaitForArrival,               // 0x01 SC_SYNC
    ActorScript_SetPositionAndResetMotion,    // 0x02 SC_POSITION
    ActorScript_ResetToOrigin,                // 0x03 SC_POSITION_ORIGIN
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
    Actor_MoveOnWorldMap,                     // 0x1a SC_WORLD
    Actor_MoveByKeys,                         // 0x1b SC_KEYS
    Actor_CrossRope,                          // 0x1c SC_ROPE
    Actor_CrossTightrope,                     // 0x1d SC_TIGHTROPE
    Actor_MoveAsSand,                         // 0x1e SC_SAND
    Actor_SailOnMap,                          // 0x1f SC_SAIL
    Actor_SailOnWorldMap,                     // 0x20 SC_WORLD_SAIL
    Actor_FlyOnWorldMap,                      // 0x21 SC_WORLD_FLY
    Actor_MoveAsSandOnWorldMap,               // 0x22 SC_WORLD_SAND
    Actor_UpdateHoverControl,                 // 0x23 SC_HOVER
    Actor_UpdateWalkingControl,               // 0x24 SC_WALK
    Field_CheckConfiguredKeysAndCount,        // 0x25 SC_KEY_COUNT
    ActorScript_Release,                      // 0x26 SC_RELEASE
    ActorScript_SetAnimation,                 // 0x27 SC_ANIMATION
    ActorScript_Nop,                          // 0x28 SC_NOP
    ActorScript_PlaySound,                    // 0x29 SC_SE
    ActorScript_PlaySoundInView,              // 0x2a SC_SE_IN_VIEW
    ActorScript_Goto,                         // 0x2b SC_GOTO
    ActorScript_HideSprites,                  // 0x2c SC_HIDE_SPRITES
    ActorScript_ShowSprite,                   // 0x2d SC_SHOW_SPRITE
    ActorScript_InvokeCallback,               // 0x2e SC_CALL
    FieldCamera_FollowWatchedInBounds,        // 0x2f SC_CAMERA_IN_BOUNDS
    ActorScript_SetMoveTargetXZ,              // 0x30 SC_MOVE_XZ
    ActorScript_End,                          // 0x31
    ActorScript_End,                          // 0x32
    ActorScript_End,                          // 0x33
    ActorScript_End,                          // 0x34
    ActorScript_End,                          // 0x35
};

/// The handler of each work value (enum ActorScriptValue in actor_script.h).
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
    ActorScript_SetOrCompareAcceleration,      // 0x10 SW_ACCELERATION
    ActorScript_SetOrCompareTargetX,           // 0x11 SW_TARGET_X
    ActorScript_SetOrCompareTargetY,           // 0x12 SW_TARGET_Y
    ActorScript_SetOrCompareTargetZ,           // 0x13 SW_TARGET_Z
    ActorScript_SetOrCompareBounce,            // 0x14 SW_BOUNCE
    ActorScript_SetOrCompareGravity,           // 0x15 SW_GRAVITY
    ActorScript_SetOrCompareTerrainHeight,     // 0x16 SW_GROUND
    ActorScript_SetOrCompareSceneWord,         // 0x17
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
    ActorScript_SetOrCompareTimer,             // 0x23 SW_TIMER
    ActorScript_SetOrCompareUserValue,         // 0x24 SW_USER_VALUE
    ActorScript_SetOrCompareWatchedActor,      // 0x25
    ActorScript_SetOrCompareUpdateAddress,     // 0x26 SW_HOOK
    ActorScript_SetOrCompareSceneCounter,      // 0x27 SW_SCENE_COUNTER
    ActorScript_SetOrCompareSceneByte,         // 0x28 SW_SCENE_BYTE
};
