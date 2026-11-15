/// @file
/// Map ground: the height curve of each cell kind, the world map's terrain
/// behaviour and the small tables after them, then the battle formation's
/// placement scale; then the field object scripts the field starts by itself,
/// the script command table and the work value handlers.

#include "types.h"
#include "field/field_bank.h"
#include "field/curve.h"
#include "field/script_operands.h"
#include "field/actor_script.h"
#include "field/map_grid.h"

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
const u16 gSpriteViewer_BackdropColors[8] = {
    0x0000, 0x7c00, 0x03e0, 0x001f, 0x1ce7, 0x3def, 0x7fff, 0x000f,
};

/// Stops for a number from 0 to 715 that steps by one or skips: a skip
/// forward goes to the next stop above it, back to the last stop below.
/// 0xffff ends the list.
const u16 gSpriteViewer_SkipStops[10] = {
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

//--------------------------------------------------------------------------
// Field object scripts: the ones the field starts by itself, the command
// table and the work value handlers SC_SET, SC_ADD and SC_CMP call.
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
    ActorScript_SetOrCompareTimer,             // 0x23 SW_TIMER
    ActorScript_SetOrCompareUserValue,         // 0x24 SW_USER_VALUE
    ActorScript_SetOrCompareWatchedActor,      // 0x25
    ActorScript_SetOrCompareUpdateAddress,     // 0x26 SW_HOOK
    ActorScript_SetOrCompareRiseCounter,       // 0x27 SW_RISE_COUNTER
    ActorScript_SetOrCompareRiseEnabled,       // 0x28 SW_RISE_ENABLED
};

// Each layout's resources, as offsets from ResourceId_Map000, by map number;
// the comment names its folder in src/map.

// clang-format off
const struct MapResources gMaps[] = {
    {0, 1, {2, 3, 4}, 5}, // map_000
    {0, 1, {2, 3, 4}, 5}, // map_000
    {6, 7, {8, 9, 10}, 11}, // lobby
    {12, 13, {14, 15, 16}, 17}, // venus_lighthouse_entrance
    {18, 19, {20, 21, 22}, 23}, // venus_lighthouse
    {24, 25, {26, 27, 28}, 29}, // suhalla_gate_house
    {30, 25, {26, 27, 28}, 29}, // suhalla_gate_ambush
    {31, 32, {33, 34, 35}, 36}, // suhalla_gate_psynergy_stone
    {37, 38, {39, 40, 41}, 42}, // idejima
    {43, 44, {45, 46, 47}, 48}, // daila_1
    {49, 44, {45, 46, 47}, 48}, // daila_2
    {50, 51, {52, 53, 54}, 55}, // daila_interiors_houses
    {56, 57, {58, 59, 60}, 61}, // daila_interiors_sanctum
    {62, 63, {64, 65, 66}, 67}, // kandorean_temple_mimic
    {68, 63, {64, 65, 66}, 67}, // kandorean_temple_lifts
    {69, 63, {64, 65, 66}, 67}, // kandorean_temple_chestbeaters
    {70, 63, {64, 65, 66}, 67}, // kandorean_temple_pillar
    {71, 63, {64, 65, 66}, 67}, // kandorean_temple_5
    {72, 63, {64, 65, 66}, 67}, // kandorean_temple_steam
    {73, 63, {64, 65, 66}, 67}, // kandorean_temple_fog
    {74, 75, {45, 76, 47}, 48}, // kandorean_temple_gate
    {77, 78, {79, 80, 81}, 82}, // kandorean_temple_poi
    {83, 84, {85, 59, 60}, 61}, // shrine_of_the_sea_god_sea_gods_tear
    {86, 84, {85, 59, 60}, 61}, // shrine_of_the_sea_god_rope
    {87, 84, {85, 59, 60}, 61}, // shrine_of_the_sea_god_2
    {88, 84, {85, 59, 60}, 61}, // shrine_of_the_sea_god_3
    {89, 84, {85, 59, 60}, 61}, // shrine_of_the_sea_god_4
    {90, 84, {85, 59, 60}, 61}, // shrine_of_the_sea_god_breath
    {91, 92, {85, 59, 60}, 61}, // shrine_of_the_sea_god_6
    {93, 84, {85, 59, 60}, 61}, // shrine_of_the_sea_god_pillars
    {94, 84, {85, 59, 60}, 61}, // shrine_of_the_sea_god_9
    {95, 96, {97, 98, 99}, 100}, // shrine_of_the_sea_god_10
    {101, 102, {103, 104, 105}, 106}, // shrine_of_the_sea_god_right_prong
    {107, 108, {109, 110, 111}, 112}, // dehkan_plateau_full_metal_vest_chest
    {113, 114, {115, 116, 117}, 118}, // dehkan_plateau_themis_axe_chest
    {119, 120, {121, 122, 123}, 124}, // dehkan_plateau_3
    {125, 126, {127, 128, 129}, 130}, // dehkan_plateau_4
    {131, 132, {133, 134, 135}, 136}, // dehkan_plateau_pound_cube
    {137, 138, {139, 140, 141}, 142}, // dehkan_plateau_cannon
    {143, 144, {145, 146, 147}, 148}, // gondowan_cliffs_hideout
    {149, 150, {151, 152, 153}, 154}, // madra
    {155, 156, {157, 158, 159}, 160}, // madra_interiors
    {161, 162, {163, 164, 165}, 166}, // madra_interiors_2
    {167, 168, {169, 170, 171}, 172}, // madra_catacombs_entrance
    {173, 174, {175, 176, 177}, 178}, // madra_catacombs_2
    {179, 180, {181, 182, 183}, 184}, // madra_catacombs_3
    {185, 186, {187, 188, 189}, 190}, // madra_catacombs_tablet
    {191, 192, {187, 188, 189}, 190}, // madra_catacombs_bookshelf
    {193, 194, {187, 188, 189}, 190}, // madra_catacombs_stone_door
    {195, 196, {197, 198, 199}, 200}, // madra_drawbridge
    {201, 202, {203, 204, 205}, 206}, // osenia_cliffs
    {207, 208, {197, 209, 210}, 200}, // mikasalla
    {211, 212, {213, 214, 215}, 216}, // mikasalla_2
    {217, 218, {219, 220, 221}, 222}, // mikasalla_interiors
    {223, 224, {225, 226, 227}, 228}, // mikasalla_interiors_sanctum
    {229, 230, {197, 209, 210}, 200}, // garoh_1
    {231, 232, {197, 209, 210}, 200}, // garoh_hypnos_sword_chest
    {233, 232, {197, 209, 210}, 200}, // garoh_blocks
    {234, 235, {79, 80, 81}, 82}, // garoh_4
    {236, 237, {79, 80, 81}, 82}, // garoh_werewolf_cave
    {223, 238, {225, 226, 227}, 228}, // mikasalla_interiors_sanctum
    {239, 240, {219, 220, 221}, 222}, // garoh_interiors
    {161, 241, {163, 164, 165}, 166}, // madra_interiors_2
    {242, 243, {244, 245, 246}, 247}, // yampi_desert_guardian_ring_chest
    {248, 243, {244, 245, 246}, 247}, // yampi_desert_oasis
    {249, 243, {244, 245, 246}, 247}, // yampi_desert_blitz
    {250, 243, {244, 245, 246}, 247}, // yampi_desert_psynergy_stone
    {251, 243, {244, 245, 246}, 247}, // yampi_desert_king_scorpion
    {252, 243, {244, 245, 246}, 247}, // yampi_desert_treasure_pit
    {253, 243, {244, 245, 246}, 247}, // yampi_desert_7
    {254, 243, {244, 245, 246}, 247}, // yampi_desert_trainers_whip_chest
    {255, 243, {244, 245, 246}, 247}, // yampi_desert_medal_spot
    {256, 243, {244, 245, 246}, 247}, // yampi_desert_10
    {257, 243, {244, 245, 246}, 247}, // yampi_desert_11
    {258, 243, {244, 245, 246}, 247}, // yampi_desert_blow_mace_chest
    {259, 260, {261, 245, 246}, 247}, // yampi_desert_cave_water_of_life_chest
    {262, 263, {261, 245, 246}, 264}, // yampi_desert_cave_guardian
    {265, 266, {261, 245, 246}, 267}, // yampi_desert_cave_crystal
    {268, 269, {270, 271, 272}, 273}, // airs_rock_storm_brand_chest
    {274, 269, {270, 271, 272}, 273}, // airs_rock_entrance
    {275, 269, {270, 271, 272}, 273}, // airs_rock_fujin_shield_chest
    {276, 277, {278, 279, 280}, 281}, // airs_rock_inside_1
    {282, 277, {278, 279, 280}, 281}, // airs_rock_inside_2
    {283, 277, {278, 279, 280}, 281}, // airs_rock_inside_clarity_circlet_chest
    {284, 277, {278, 279, 280}, 281}, // airs_rock_inside_4
    {285, 277, {278, 279, 280}, 281}, // airs_rock_inside_reveal_tablet
    {286, 277, {278, 279, 280}, 281}, // airs_rock_inside_elixir_chest
    {287, 277, {278, 279, 280}, 281}, // airs_rock_inside_paired_statues
    {288, 277, {278, 279, 280}, 281}, // airs_rock_inside_face_warp
    {289, 277, {278, 279, 280}, 281}, // airs_rock_inside_coins_chest
    {290, 291, {292, 293, 294}, 295}, // airs_rock_inside_psynergy_stone
    {296, 297, {151, 152, 153}, 154}, // alhafra
    {298, 299, {157, 158, 159}, 300}, // alhafra_interiors
    {301, 299, {157, 158, 159}, 300}, // alhafra_manor
    {302, 303, {304, 305, 306}, 307}, // eastern_alhafra
    {308, 309, {310, 311, 312}, 313}, // eastern_alhafra_ship
    {314, 315, {316, 317, 318}, 319}, // alhafran_cave
    {320, 321, {316, 317, 318}, 319}, // alhafra_jail
    {322, 323, {324, 325, 326}, 327}, // naribwe
    {328, 329, {330, 331, 332}, 333}, // naribwe_interiors
    {334, 335, {336, 337, 338}, 339}, // gondowan_cliffs
    {340, 335, {336, 337, 338}, 339}, // gondowan_cliffs_pass
    {341, 342, {343, 344, 345}, 346}, // kibombo_mountains_waft
    {347, 348, {343, 344, 345}, 346}, // kibombo_mountains_disk_axe_chest
    {349, 350, {351, 352, 353}, 354}, // kibombo_mountains_3
    {355, 356, {357, 358, 359}, 360}, // kibombo_1
    {361, 362, {363, 364, 365}, 366}, // kibombo_2
    {367, 368, {369, 370, 371}, 372}, // kibombo_4
    {373, 374, {375, 376, 377}, 378}, // kibombo_5
    {379, 380, {330, 331, 332}, 333}, // kibombo_3
    {381, 382, {383, 384, 385}, 386}, // gabomba_statue_bone_armlet_chest
    {387, 388, {383, 384, 385}, 386}, // gabomba_statue_elixir_chest
    {389, 390, {383, 384, 385}, 386}, // gabomba_statue_steel
    {391, 392, {383, 384, 385}, 386}, // gabomba_statue_turntable
    {393, 394, {383, 384, 385}, 386}, // gabomba_statue_cogs
    {395, 396, {383, 384, 385}, 386}, // gabomba_statue_chamber_1
    {397, 398, {399, 400, 401}, 402}, // gabomba_statue_arrows
    {403, 404, {399, 400, 401}, 402}, // gabomba_statue_chamber_lift
    {405, 406, {407, 408, 409}, 410}, // gabomba_statue_pinion
    {411, 412, {399, 400, 401}, 402}, // gabomba_statue_chamber_witch_doctor
    {413, 414, {415, 416, 417}, 418}, // kibombo_6
    {419, 420, {421, 422, 423}, 424}, // gabomba_catacombs_teleporters
    {425, 420, {421, 422, 423}, 424}, // gabomba_catacombs_mud
    {426, 420, {421, 422, 423}, 424}, // gabomba_catacombs_3
    {427, 420, {421, 422, 423}, 424}, // gabomba_catacombs_tomegathericon
    {428, 420, {421, 422, 423}, 424}, // gabomba_catacombs_5
    {429, 430, {431, 432, 433}, 434}, // lemurian_ship_deck
    {435, 436, {310, 311, 312}, 313}, // lemurian_ship_1
    {437, 436, {310, 311, 312}, 313}, // lemurian_ship_aqua_hydra
    {438, 439, {440, 441, 442}, 443}, // islets_1
    {444, 445, {197, 198, 210}, 200}, // islets_2
    {446, 447, {45, 46, 47}, 48}, // islets_3
    {448, 449, {450, 451, 452}, 453}, // islets_turtle
    {454, 455, {197, 198, 199}, 200}, // yallam
    {456, 457, {458, 459, 460}, 461}, // yallam_interiors
    {462, 463, {163, 164, 165}, 166}, // yallam_interiors_shops
    {211, 464, {213, 214, 215}, 216}, // mikasalla_2
    {465, 466, {467, 468, 469}, 470}, // taopo_swamp_1
    {471, 472, {473, 474, 475}, 476}, // taopo_swamp_2
    {477, 478, {479, 480, 481}, 482}, // taopo_swamp_6
    {483, 478, {479, 480, 481}, 482}, // taopo_swamp_tear_stones
    {484, 478, {479, 480, 481}, 482}, // taopo_swamp_flower
    {485, 478, {479, 480, 481}, 482}, // taopo_swamp_5
    {486, 478, {479, 480, 481}, 482}, // taopo_swamp_7
    {487, 478, {479, 480, 481}, 482}, // taopo_swamp_8
    {488, 489, {479, 480, 481}, 482}, // taopo_swamp_9
    {490, 489, {479, 480, 481}, 482}, // taopo_swamp_10
    {491, 489, {479, 480, 481}, 482}, // taopo_swamp_star_dust
    {492, 493, {494, 495, 496}, 497}, // apojii_islands_1
    {498, 499, {500, 501, 502}, 503}, // apojii_islands_haze
    {504, 505, {506, 507, 508}, 509}, // apojii_islands_3
    {510, 511, {512, 513, 514}, 515}, // apojii_islands_cave
    {516, 517, {518, 519, 520}, 521}, // aqua_rock_1
    {522, 517, {518, 519, 520}, 521}, // aqua_rock_mist_sabre_chest
    {523, 524, {525, 526, 527}, 528}, // aqua_rock_inside_water_of_life_chest
    {529, 524, {525, 526, 527}, 528}, // aqua_rock_inside_steam
    {530, 524, {525, 526, 527}, 528}, // aqua_rock_inside_aquarius_stone
    {531, 524, {525, 526, 527}, 528}, // aqua_rock_inside_4
    {532, 524, {525, 526, 527}, 528}, // aqua_rock_inside_5
    {533, 524, {525, 526, 527}, 528}, // aqua_rock_inside_6
    {534, 291, {292, 293, 294}, 295}, // aqua_rock_inside_psynergy_stone
    {535, 524, {525, 526, 527}, 528}, // aqua_rock_inside_8
    {536, 524, {525, 526, 527}, 528}, // aqua_rock_inside_tear_stone
    {537, 538, {278, 279, 280}, 281}, // aqua_rock_inside_parch_tablet
    {539, 540, {541, 542, 543}, 544}, // izumo
    {545, 546, {547, 548, 549}, 550}, // izumo_interiors
    {551, 552, {79, 80, 81}, 553}, // izumo_ruins_first_room
    {554, 555, {79, 80, 81}, 556}, // izumo_ruins_second_room
    {557, 558, {270, 271, 272}, 273}, // gaia_rock_susa
    {559, 558, {270, 271, 272}, 273}, // gaia_rock_statue_spots
    {560, 558, {270, 271, 272}, 273}, // gaia_rock_dancing_idol
    {561, 562, {563, 564, 565}, 566}, // gaia_rock_inside_maze
    {567, 568, {569, 570, 571}, 572}, // gaia_rock_inside_first_altar
    {573, 568, {569, 570, 571}, 572}, // gaia_rock_inside_twin_altars
    {574, 568, {569, 570, 571}, 572}, // gaia_rock_inside_4
    {575, 568, {569, 570, 571}, 572}, // gaia_rock_inside_5
    {576, 577, {541, 542, 543}, 544}, // champa
    {578, 579, {547, 548, 549}, 550}, // champa_interiors
    {580, 581, {582, 583, 584}, 585}, // champa_cliff_caves_briggs
    {586, 581, {582, 583, 584}, 585}, // champa_cliff_caves_2
    {587, 581, {582, 583, 584}, 585}, // champa_cliff_caves_obaba
    {588, 581, {582, 583, 584}, 585}, // champa_cliff_caves_avimander
    {589, 590, {225, 226, 227}, 228}, // champa_sanctum
    {591, 592, {593, 594, 595}, 596}, // sea_of_time_whirlpool
    {597, 598, {599, 600, 601}, 602}, // sea_of_time_to_lemuria
    {603, 430, {604, 605, 606}, 607}, // lemurian_ship_deck_1
    {608, 609, {97, 98, 99}, 100}, // ankohl_ruins_1
    {610, 609, {97, 98, 99}, 100}, // ankohl_ruins_thanatos_mace_chest
    {611, 609, {97, 98, 99}, 100}, // ankohl_ruins_3
    {612, 609, {97, 98, 99}, 100}, // ankohl_ruins_4
    {95, 609, {97, 98, 99}, 100}, // shrine_of_the_sea_god_10
    {613, 609, {97, 98, 99}, 100}, // ankohl_ruins_sand_doors
    {614, 615, {103, 104, 105}, 106}, // ankohl_ruins_7
    {101, 616, {103, 104, 105}, 106}, // shrine_of_the_sea_god_right_prong
    {603, 617, {604, 605, 606}, 607}, // lemurian_ship_deck_1
    {618, 619, {213, 214, 215}, 216}, // lemuria_entrance
    {620, 621, {622, 623, 624}, 625}, // lemuria
    {626, 627, {622, 623, 624}, 625}, // ancient_lemuria
    {628, 629, {630, 631, 632}, 633}, // lemuria_palace
    {634, 635, {636, 637, 638}, 639}, // lemuria_interiors
    {640, 641, {642, 643, 644}, 645}, // lemuria_spring
    {646, 647, {97, 98, 99}, 100}, // tundaria_tower_center_prong
    {648, 649, {97, 98, 99}, 100}, // tundaria_tower_3
    {650, 651, {97, 98, 99}, 100}, // tundaria_tower_reflux
    {652, 653, {97, 98, 99}, 100}, // tundaria_tower_5
    {654, 655, {97, 98, 99}, 100}, // tundaria_tower_6
    {656, 657, {103, 104, 658}, 106}, // tundaria_tower_1
    {659, 660, {103, 104, 658}, 106}, // tundaria_tower_burst_brooch
    {661, 662, {663, 664, 665}, 666}, // settlements_3
    {667, 668, {669, 670, 671}, 672}, // settlements_9
    {673, 674, {324, 325, 326}, 327}, // settlements_1
    {675, 676, {677, 678, 679}, 680}, // settlements_4
    {681, 682, {683, 684, 685}, 686}, // settlements_7
    {687, 688, {689, 690, 691}, 692}, // settlements_8
    {693, 694, {270, 271, 272}, 273}, // magma_rock_1
    {695, 694, {270, 271, 272}, 273}, // magma_rock_salamander_tail_chest
    {696, 694, {270, 271, 272}, 273}, // magma_rock_statue
    {697, 698, {525, 526, 527}, 528}, // magma_rock_inside_lucky_medal_chest
    {699, 698, {525, 526, 527}, 528}, // magma_rock_inside_2
    {700, 698, {525, 526, 527}, 528}, // magma_rock_inside_fury
    {701, 698, {525, 526, 527}, 528}, // magma_rock_inside_4
    {702, 698, {525, 526, 527}, 528}, // magma_rock_inside_golem_core_chest
    {703, 698, {525, 526, 527}, 528}, // magma_rock_inside_6
    {704, 698, {525, 526, 527}, 528}, // magma_rock_inside_ember_stone
    {705, 706, {278, 279, 280}, 281}, // magma_rock_inside_blaze_tablet
    {707, 708, {604, 605, 606}, 709}, // atteka_inlet_1
    {710, 711, {712, 713, 714}, 715}, // atteka_inlet_geode
    {716, 717, {712, 713, 714}, 718}, // contigo_1
    {719, 720, {721, 722, 723}, 724}, // contigo_shops
    {719, 725, {721, 722, 723}, 724}, // contigo_shops
    {726, 727, {663, 664, 665}, 666}, // shaman_village_cave
    {728, 729, {730, 731, 732}, 733}, // shaman_village_1
    {734, 735, {736, 737, 738}, 739}, // shaman_village_2
    {740, 741, {736, 737, 738}, 739}, // shaman_village_aroma
    {742, 743, {744, 745, 746}, 747}, // shaman_village_inn
    {742, 748, {744, 745, 746}, 747}, // shaman_village_inn
    {223, 749, {225, 226, 227}, 228}, // mikasalla_interiors_sanctum
    {750, 751, {97, 98, 752}, 100}, // anemos_inner_sanctum_dragon_skin_chest
    {753, 751, {97, 98, 752}, 100}, // anemos_inner_sanctum_2
    {754, 751, {97, 98, 752}, 100}, // anemos_inner_sanctum_switches
    {755, 751, {97, 98, 752}, 100}, // anemos_inner_sanctum_orihalcon_chest
    {756, 751, {97, 98, 752}, 100}, // anemos_inner_sanctum_guardian
    {757, 751, {97, 98, 752}, 100}, // anemos_inner_sanctum_entrance
    {758, 759, {760, 761, 762}, 763}, // trial_road_1
    {764, 759, {760, 761, 762}, 763}, // trial_road_3
    {765, 766, {767, 768, 769}, 770}, // trial_road_gasp
    {771, 772, {773, 774, 775}, 776}, // trial_road_summit
    {777, 778, {779, 780, 781}, 782}, // jupiter_lighthouse_entrance
    {783, 784, {79, 80, 81}, 82}, // jupiter_lighthouse_basement_1
    {785, 291, {292, 293, 294}, 295}, // jupiter_lighthouse_basement_2
    {786, 787, {788, 789, 790}, 791}, // jupiter_lighthouse_room_1
    {792, 787, {788, 789, 790}, 791}, // jupiter_lighthouse_room_2
    {793, 787, {788, 789, 790}, 791}, // jupiter_lighthouse_room_erinyes_tunic_chest
    {794, 787, {788, 789, 790}, 791}, // jupiter_lighthouse_room_locked_doors
    {795, 787, {788, 789, 790}, 791}, // jupiter_lighthouse_room_red_key
    {796, 787, {788, 789, 790}, 791}, // jupiter_lighthouse_room_6
    {797, 787, {788, 789, 790}, 791}, // jupiter_lighthouse_room_7
    {798, 787, {788, 789, 790}, 791}, // jupiter_lighthouse_room_blue_key
    {799, 787, {788, 789, 790}, 791}, // jupiter_lighthouse_room_phaetons_blade_chest
    {800, 801, {779, 780, 781}, 782}, // jupiter_lighthouse_room_m
    {802, 803, {779, 780, 781}, 782}, // jupiter_lighthouse_aerie
    {804, 805, {806, 807, 808}, 809}, // loho_1
    {810, 811, {812, 813, 814}, 815}, // loho_2
    {816, 817, {599, 600, 601}, 602}, // northern_reaches
    {818, 819, {820, 821, 822}, 823}, // prox_1
    {824, 825, {820, 821, 822}, 823}, // prox_2
    {826, 827, {820, 821, 822}, 823}, // prox_mold
    {828, 688, {689, 690, 691}, 692}, // prox_4
    {161, 829, {163, 164, 165}, 166}, // madra_interiors_2
    {223, 830, {225, 226, 227}, 228}, // mikasalla_interiors_sanctum
    {831, 832, {779, 780, 781}, 833}, // mars_lighthouse_entrance_1
    {834, 835, {836, 837, 838}, 839}, // mars_lighthouse_room_f_1
    {840, 835, {836, 837, 838}, 839}, // mars_lighthouse_room_f_2
    {841, 835, {836, 837, 838}, 839}, // mars_lighthouse_room_f_3
    {842, 835, {836, 837, 838}, 839}, // mars_lighthouse_room_f_4
    {843, 844, {779, 845, 781}, 846}, // mars_lighthouse_aerie
    {847, 844, {779, 780, 781}, 848}, // mars_lighthouse_room_m_6
    {849, 850, {836, 851, 838}, 852}, // mars_lighthouse_room_m_1
    {853, 835, {836, 837, 838}, 839}, // mars_lighthouse_entrance_fugue
    {854, 835, {836, 837, 838}, 839}, // mars_lighthouse_entrance_teleport_lapis
    {855, 835, {836, 837, 838}, 839}, // mars_lighthouse_entrance_balm
    {856, 835, {836, 837, 838}, 839}, // mars_lighthouse_entrance_5
    {857, 835, {836, 837, 838}, 839}, // mars_lighthouse_entrance_valkyrie_mail_chest
    {858, 835, {836, 837, 838}, 839}, // mars_lighthouse_entrance_sol_blade_chest
    {859, 835, {836, 837, 838}, 839}, // mars_lighthouse_star_room
    {860, 844, {779, 780, 781}, 861}, // mars_lighthouse_room_m_2
    {862, 844, {779, 780, 781}, 861}, // mars_lighthouse_room_m_3
    {863, 844, {779, 780, 781}, 861}, // mars_lighthouse_room_m_4
    {864, 844, {779, 780, 781}, 861}, // mars_lighthouse_room_m_5
    {831, 865, {779, 780, 781}, 833}, // mars_lighthouse_entrance_1
    {834, 866, {836, 837, 838}, 839}, // mars_lighthouse_room_f_1
    {840, 866, {836, 837, 838}, 839}, // mars_lighthouse_room_f_2
    {841, 866, {836, 837, 838}, 839}, // mars_lighthouse_room_f_3
    {842, 866, {836, 837, 838}, 839}, // mars_lighthouse_room_f_4
    {843, 867, {779, 780, 781}, 846}, // mars_lighthouse_aerie
    {847, 868, {779, 780, 781}, 848}, // mars_lighthouse_room_m_6
    {849, 869, {836, 837, 838}, 852}, // mars_lighthouse_room_m_1
    {853, 866, {836, 837, 838}, 839}, // mars_lighthouse_entrance_fugue
    {854, 866, {836, 837, 838}, 839}, // mars_lighthouse_entrance_teleport_lapis
    {855, 866, {836, 837, 838}, 839}, // mars_lighthouse_entrance_balm
    {856, 866, {836, 837, 838}, 839}, // mars_lighthouse_entrance_5
    {857, 866, {836, 837, 838}, 839}, // mars_lighthouse_entrance_valkyrie_mail_chest
    {858, 866, {836, 837, 838}, 839}, // mars_lighthouse_entrance_sol_blade_chest
    {859, 866, {836, 837, 838}, 839}, // mars_lighthouse_star_room
    {860, 867, {779, 780, 781}, 861}, // mars_lighthouse_room_m_2
    {862, 867, {779, 780, 781}, 861}, // mars_lighthouse_room_m_3
    {863, 867, {779, 780, 781}, 861}, // mars_lighthouse_room_m_4
    {864, 867, {779, 780, 781}, 861}, // mars_lighthouse_room_m_5
    {870, 871, {872, 873, 874}, 875}, // mercury_lighthouse
    {876, 877, {878, 879, 880}, 881}, // treasure_isle_1
    {882, 883, {884, 885, 886}, 887}, // treasure_isle_2
    {888, 889, {890, 891, 892}, 893}, // treasure_isle_jesters_armlet_chest
    {894, 895, {878, 879, 880}, 881}, // treasure_isle_gale
    {896, 895, {878, 879, 880}, 881}, // treasure_isle_guardian
    {897, 898, {899, 900, 901}, 902}, // islet_cave_meld
    {903, 904, {187, 188, 189}, 190}, // islet_cave_serac
    {905, 906, {187, 188, 189}, 190}, // islet_cave_sentinel
    {907, 908, {139, 140, 141}, 142}, // caverns_indra
    {909, 910, {911, 912, 913}, 914}, // caverns_osenia
    {915, 916, {878, 879, 880}, 881}, // caverns_angara
    {917, 918, {919, 920, 921}, 922}, // mt_aleph_summit
    {923, 924, {925, 926, 927}, 928}, // map_290
    {929, 930, {931, 932, 933}, 934}, // r60d
    {935, 936, {937, 938, 939}, 940}, // shop_test
    {941, 942, {943, 944, 945}, 946}, // test_0
    {947, 948, {949, 950, 951}, 952}, // test_1
    {953, 954, {955, 956, 957}, 958}, // test_2
    {959, 960, {961, 962, 963}, 964}, // test_3
    {965, 966, {967, 968, 969}, 970}, // test_4
    {971, 972, {973, 974, 975}, 976}, // test_5
    {977, 978, {979, 980, 981}, 982}, // test_6
    {983, 984, {985, 986, 987}, 988}, // test_7
};
// clang-format on

/// Unused: nothing reads these four words after the map table, which The
/// Broken Seal keeps too.
const u32 gMapsTail[] = {0, 3, 7, 9};
