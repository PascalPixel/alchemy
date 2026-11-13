#include "types.h"
#include "scene_tables.h"
#include "version.h"
#include "module/scene/lemurian_ship.h"
#include "module/scene/taopo_swamp.h"

void TaopoSwamp_FightFlower(void);
void TaopoSwamp_BlowAwayBush(void);
void TaopoSwamp_SinkPillar(void);
void TaopoSwamp_ScoopHiddenItem(void);
void TaopoSwamp_ScoopOtherHiddenItem(void);
void TaopoSwamp_DigHole(void);
void TaopoSwamp_FallThroughHole(void);
void TaopoSwamp_PushPillarOverVent(void);
void TaopoSwamp_StepOnVent(void);
void TaopoSwamp_BlowFlowerOffLedge(void);
void TaopoSwamp_DropPillarIntoGap(void);
void TaopoSwamp_DouseVentSpot(void);
void TaopoSwamp_FreezeVentSpot(void);
void TaopoSwamp_FillDripSpot(void);
void TaopoSwamp_FreezeDripSpot(void);
void TaopoSwamp_CoolLavaRock(void);
void TaopoSwamp_PushLavaRockIn(void);
void TaopoSwamp_CoolPillarRock(void);
void TaopoSwamp_PushPillarRockIn(void);
void TaopoSwamp_CoolChestRock(void);
void TaopoSwamp_PushChestRockIn(void);
void TaopoSwamp_CoolExitRock(void);
void TaopoSwamp_PushExitRockIn(void);
void TaopoSwamp_CoolDarkRock(void);
void TaopoSwamp_CoolLastRock(void);
void TaopoSwamp_ScoopStarDust(void);
void TaopoSwamp_ShowSparkleMessage(void);
void TaopoSwamp_LandDrop(void);
void TaopoSwamp_LandDrip(void);
void TaopoSwamp_IsDropFalling(void);

u32 gTaopoSwamp_Room5HoleVents[] = {
    0x6090200,
    0x1050604,
};

u32 gTaopoSwamp_Room7HoleVents[] = {
    0x10003,
    1,
};

// clang-format off
u32 gTaopoSwamp_PuffSteamScript[] = {
    22, 26, 0, 39, 2, 0x80010000,
    23, 6, 0x10000, 0, 1, 13,
    30, 0xc0010000, 38,
};

u32 gTaopoSwamp_TremorPillarScript[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gTaopoSwamp_SparkFades[] = {
    0, 30, 38,
};

s32 gTaopoSwamp_DripAtRandomScript[] = {
    46,
    (s32)TaopoSwamp_LandDrop,
    38,
};

s32 gTaopoSwamp_DripFromCeilingScript[] = {
    46,
    (s32)TaopoSwamp_LandDrip,
    38,
};

s32 gTaopoSwamp_DropFalls[] = {
    46,
    (s32)TaopoSwamp_IsDropFalling,
    38,
};

u16 gTaopoSwamp_Room5Vents[] = {
    200, 200, 4, 264, 200, 4, 232, 216,
    4, 280, 216, 4, 568, 200, 4, 632,
    200, 4, 536, 216, 4, 600, 216, 4,
    408, 232, 4, 472, 232, 4, 0,
};

u16 gTaopoSwamp_Room5VentPair[] = {
    9, 10, 0,
};

u16 gTaopoSwamp_Room7Vents[] = {
    424, 104, 4, 536, 136, 4, 0,
};

u16 gTaopoSwamp_Room8Vents[] = {
    408, 408, 4, 472, 360, 4, 0,
};

u16 gTaopoSwamp_RockRoomVents[] = {
    216, 184, 4, 312, 136, 4, 0, 0,
};

u32 gTaopoSwamp_SplashFades[] = {
    0, 32, 38,
};

u32 gTaopoSwamp_PuffRises[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(143),
    SCENE_EXIT(1, 2, 35),
    SCENE_EXIT(2, 144, 1),
    SCENE_EXIT(3, 144, 2),
    SCENE_EXITS(144),
    SCENE_EXIT(1, 143, 2),
    SCENE_EXIT(2, 143, 3),
    SCENE_EXIT(3, 145, 3),
    SCENE_EXIT(4, 148, 4),
    SCENE_EXIT(5, 145, 5),
    SCENE_EXITS(145),
    SCENE_EXIT(3, 144, 3),
    SCENE_EXIT(5, 144, 5),
    SCENE_EXIT(6, 147, 2),
    SCENE_EXIT(7, 147, 3),
    SCENE_EXIT(8, 145, 9),
    SCENE_EXIT(9, 145, 8),
    SCENE_EXITS(146),
    SCENE_EXIT(1, 147, 1),
    SCENE_EXIT(2, 147, 4),
    SCENE_EXITS(147),
    SCENE_EXIT(1, 146, 1),
    SCENE_EXIT(2, 145, 6),
    SCENE_EXIT(3, 145, 7),
    SCENE_EXIT(4, 146, 2),
    SCENE_EXIT(10, 146, 10),
    SCENE_EXIT(11, 146, 11),
    SCENE_EXIT(12, 146, 12),
    SCENE_EXIT(13, 146, 13),
    SCENE_EXIT(14, 146, 14),
    SCENE_EXIT(15, 146, 15),
    SCENE_EXIT(16, 146, 16),
    SCENE_EXITS(148),
    SCENE_EXIT(1, 149, 1),
    SCENE_EXIT(4, 144, 4),
    SCENE_EXITS(149),
    SCENE_EXIT(1, 148, 1),
    SCENE_EXIT(2, 150, 2),
    SCENE_EXIT(3, 150, 3),
    SCENE_EXIT(4, 149, 5),
    SCENE_EXIT(5, 149, 4),
    SCENE_EXIT(10, 150, 9),
    SCENE_EXIT(12, 150, 9),
    SCENE_EXITS(150),
    SCENE_EXIT(1, 152, 1),
    SCENE_EXIT(2, 149, 2),
    SCENE_EXIT(3, 149, 3),
    SCENE_EXIT(4, 150, 5),
    SCENE_EXIT(5, 150, 4),
    SCENE_EXITS(152),
    SCENE_EXIT(1, 150, 1),
    SCENE_EXIT(2, 153, 2),
    SCENE_EXIT(3, 151, 3),
    SCENE_EXIT(4, 151, 4),
    SCENE_EXITS(151),
    SCENE_EXIT(3, 152, 3),
    SCENE_EXIT(4, 152, 4),
    SCENE_EXITS(153),
    SCENE_EXIT(2, 152, 2),
    SCENE_EXIT(3, 153, 4),
    SCENE_EXIT(4, 153, 3),
    SCENE_EXIT(5, 153, 6),
    SCENE_EXIT(6, 153, 5),
    SCENE_EXITS_END,
};

struct ActorPlacement gTaopoSwamp_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room2Placements[] = {
    {307, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(128), FACING_NORTH, 2, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(112), FACING_NORTH, 2, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(112), FACING_NORTH, 2, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(176), FACING_NORTH, 2, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(256), FACING_NORTH, 2, 0},
    {299, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(272), FACING_NORTH, 2, 0},
    {273, 0x8e5, 1, PIXELS(216), 0, PIXELS(264), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room4Placements[] = {
    {243, 0x3b, 1, PIXELS(440), 0, PIXELS(632), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room5Placements[] = {
    {243, 0x3b, 1, PIXELS(408), 0, PIXELS(312), FACING_SOUTH, 0, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room7Placements[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(136), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(424), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room8Placements[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(200), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(200), FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(440), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room10Placements[] = {
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {292, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {292, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(296), FACING_NORTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(312), FACING_NORTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(360), FACING_NORTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(376), FACING_NORTH, 2, 1},
    {328, CONDITION_ALWAYS, 1, PIXELS(384), 0, PIXELS(320), FACING_NORTH, 2, 0},
    {328, CONDITION_ALWAYS, 1, PIXELS(416), 0, PIXELS(336), FACING_NORTH, 2, 1},
    {328, CONDITION_ALWAYS, 1, PIXELS(368), 0, PIXELS(352), FACING_NORTH, 2, 1},
    {328, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room9Placements[] = {
    {292, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(360), FACING_NORTH, 2, 0},
    {292, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(216), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room11Placements[] = {
    {292, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(632), FACING_NORTH, 2, 0},
    {292, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(632), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gTaopoSwamp_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0xce01, 3, IF_FLAG_SET(0x8e0), 3},
    {0xce01, 4, IF_FLAG_SET(0x8e3), 4},
    {0xce01, 5, IF_FLAG_SET(0x8e4), 5},
    {TRIGGER_STEP, 60, CONDITION_ALWAYS, EVENT(TaopoSwamp_SinkInMud)},
    {TRIGGER_STEP, 61, CONDITION_ALWAYS, EVENT(TaopoSwamp_RiseOnBubble)},
    {TRIGGER_STEP, 62, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnFirmGround)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, EVENT(TaopoSwamp_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, EVENT(TaopoSwamp_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, EVENT(TaopoSwamp_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, EVENT(TaopoSwamp_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 13, CONDITION_ALWAYS, EVENT(TaopoSwamp_GrowVine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(TaopoSwamp_SinkPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room3Triggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {49, 6, CONDITION_ALWAYS, 6},
    {33, 7, CONDITION_ALWAYS, 7},
    {49, 8, CONDITION_ALWAYS, 8},
    {33, 9, CONDITION_ALWAYS, 9},
    {0x50008805, 51, 0x301, EVENT(TaopoSwamp_ScoopHiddenItem)},
    {0x50008805, 50, 0x300, EVENT(TaopoSwamp_ScoopOtherHiddenItem)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room4Triggers[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(TaopoSwamp_FightFlower)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(TaopoSwamp_DigHole)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(TaopoSwamp_DigHoleUnderfoot)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(TaopoSwamp_StartMadPlantBattle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room5Triggers[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {49, 3, CONDITION_ALWAYS, 3},
    {49, 4, CONDITION_ALWAYS, 4},
    {0x4602, 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {0x4602, 11, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {0x4602, 12, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {0x4602, 13, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {0x4602, 14, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {0x4602, 15, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {0x4602, 16, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 17, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 18, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0xa6d, EVENT(TaopoSwamp_PushPillarOverVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0xa6e, EVENT(TaopoSwamp_PushPillarOverVent)},
    {9, 0, CONDITION_ALWAYS, EVENT(TaopoSwamp_PushPillarOverVent)},
    {TRIGGER_RAISED, 100, 0x310, EVENT(TaopoSwamp_BlowFlowerOffLedge)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room6Triggers[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gTaopoSwamp_Room7Triggers[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {49, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x4602, 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0xa6f,
     EVENT(TaopoSwamp_DropPillarIntoGap)},
    {9, 0, CONDITION_ALWAYS, EVENT(TaopoSwamp_DropPillarIntoGap)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_ACTS), 8, 0x220,
     EVENT(TaopoSwamp_DouseVentSpot)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, IF_FLAG_SET(0x222),
     EVENT(TaopoSwamp_FreezeVentSpot)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room8Triggers[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {33, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x308, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_ACTS), 10, 0x220,
     EVENT(TaopoSwamp_FillDripSpot)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, IF_FLAG_SET(0x222),
     EVENT(TaopoSwamp_FreezeDripSpot)},
    {TRIGGER_TABLE_END},
};

u32 gTaopoSwamp_SteamPuffs[] = {
    112,
};

u32 gTaopoSwamp_PassageRows[] = {
    120,
};

u32 gTaopoSwamp_PoolFrames[] = {
    24,
};

struct Trigger gTaopoSwamp_Room10Triggers[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, 0x331,
     EVENT(TaopoSwamp_CoolLavaRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 10, 0x334,
     EVENT(TaopoSwamp_CoolPillarRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x332,
     EVENT(TaopoSwamp_PushLavaRockIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x335,
     EVENT(TaopoSwamp_PushPillarRockIn)},
    {9, 0, 0x335, EVENT(TaopoSwamp_PushPillarRockIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 11, 0x330,
     EVENT(TaopoSwamp_TremorPillar)},
    {TRIGGER_TABLE_END},
};

s32 gTaopoSwamp_Room9Triggers[] = {
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    2,
    (s32)0xffff000a,
    (s32)TaopoSwamp_StepOnVent,
    2,
    (s32)0xffff000b,
    (s32)TaopoSwamp_StepOnVent,
#if TLA_PATCH_1 /* Absent from the Japanese edition. */
    0x602,
    (s32)0xffff0040,
    (s32)TaopoSwamp_JumpForward,
#endif
    0x2115,
    0x3380008,
    (s32)TaopoSwamp_CoolChestRock,
    0x2115,
    0x33b0009,
    (s32)TaopoSwamp_CoolExitRock,
    0x8c15,
    0x3390008,
    (s32)TaopoSwamp_PushChestRockIn,
    0x8c15,
    0x33c0009,
    (s32)TaopoSwamp_PushExitRockIn,
    -1,
    0,
    0,
};

struct Trigger gTaopoSwamp_Room11Triggers[] = {
    {33, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {83, 50, 0x348, EVENT(TaopoSwamp_ShowSparkleMessage)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, 0x340,
     EVENT(TaopoSwamp_CoolDarkRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, 0x342,
     EVENT(TaopoSwamp_CoolLastRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x341, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x343, 0},
    {0x50008805, 50, 0x348, EVENT(TaopoSwamp_ScoopStarDust)},
    {TRIGGER_TABLE_END},
};

u32 gTaopoSwamp_SparkleFrame[] = {
    0,
};
