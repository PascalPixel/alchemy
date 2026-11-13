#include "../shared/tables.h"
#include "types.h"
#include "scene_tables.h"
#include "module/scene/yampi_desert_cave.h"

void YampiDesertCave_BurstRoom1Pillar(void);
void YampiDesertCave_BurstRoom3Pillar(void);
void YampiDesertCave_StartMovingPillar(void);
void YampiDesertCave_FinishMovingPillar(void);
void YampiDesertCave_PoundStake(void);
void YampiDesertCave_ScoopDjinni(void);
void YampiDesertCave_MeetGuardianOfFire(void);
void YampiDesertCave_ScoopTreasure(void);

// clang-format off
u16 gYampiDesertCave_LogSprites[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gYampiDesertCave_LogFootprints[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

DEFINE_PUSH_LEADER_STEPS(gPushLeaderSteps)

u32 gYampiDesertCave_RipplePath[] = {
    0x270065, 0, 0x230061, 0, 0x250063, 0,
    0x21005f, 0,
};

s16 gYampiDesertCave_MarkPaletteSlots[] = {
    9, 10, 11, 12, 13, 14, 15, -1,
};

s16 gYampiDesertCave_MarkColorsLit[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gYampiDesertCave_MarkColorsDim[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

DEFINE_G_DATA_020013B4(gYampiDesertCave_MarkCycle, 14, 9, 10, 11, 12, 13)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(79),
    SCENE_EXIT(1, 74, 4),
    SCENE_EXIT(2, 81, 1),
    SCENE_EXIT(3, 79, 6),
    SCENE_EXIT(4, 79, 5),
    SCENE_EXIT(5, 79, 4),
    SCENE_EXIT(6, 79, 3),
    SCENE_EXIT(7, 81, 8),
    SCENE_EXIT(8, 80, 1),
    SCENE_EXITS(80),
    SCENE_EXIT(1, 79, 8),
    SCENE_EXIT(2, 80, 17),
    SCENE_EXIT(14, 81, 13),
    SCENE_EXIT(15, 81, 12),
    SCENE_EXIT(16, 81, 11),
    SCENE_EXIT(17, 80, 2),
    SCENE_EXITS(81),
    SCENE_EXIT(1, 79, 2),
    SCENE_EXIT(2, 81, 3),
    SCENE_EXIT(3, 81, 2),
    SCENE_EXIT(4, 81, 7),
    SCENE_EXIT(5, 81, 6),
    SCENE_EXIT(6, 81, 5),
    SCENE_EXIT(7, 81, 4),
    SCENE_EXIT(8, 79, 7),
    SCENE_EXIT(9, 81, 10),
    SCENE_EXIT(10, 81, 9),
    SCENE_EXIT(11, 80, 16),
    SCENE_EXIT(12, 80, 15),
    SCENE_EXIT(13, 80, 14),
    SCENE_EXITS_END,
};

u32 gYampiDesertCave_Settle[] = {
    22, 26, 0, 17,
};

struct ActorPlacement gYampiDesertCave_Placements[] = {
    {331, CONDITION_ALWAYS, 7, PIXELS(888), 0, PIXELS(336), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(832), 0, PIXELS(312), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(912), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {404, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(712), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gYampiDesertCave_Room2Placements[] = {
    {272, CONDITION_ALWAYS, (s32)gYampiDesertCave_Settle, PIXELS(552), 0,
     PIXELS(200), FACING_SOUTH, 2, 0},
    {177, 0x9cd, 1, PIXELS(200), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gYampiDesertCave_Room3Placements[] = {
    {331, CONDITION_ALWAYS, 7, PIXELS(712), 0, PIXELS(752), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(752), 0, PIXELS(744), FACING_SOUTH, 2, 0},
    {331, CONDITION_ALWAYS, 7, PIXELS(728), 0, PIXELS(784), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(800), 0, PIXELS(760), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, (s32)gYampiDesertCave_Settle, PIXELS(632), 0,
     PIXELS(264), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, (s32)gYampiDesertCave_Settle, PIXELS(136), 0,
     PIXELS(168), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gYampiDesertCave_Settle, PIXELS(744), 0,
     PIXELS(200), FACING_SOUTH, 2, 0},
    {404, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(632), FACING_SOUTH, 2, 0},
    {251, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {243, 0x41, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

u32 gYampiDesertCave_Room3Stakes[] = {
    14,
    0xffff0206,
};

struct Trigger gYampiDesertCave_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {49, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x202, 42, CONDITION_ALWAYS, EVENT(YampiDesertCave_TryPushLog)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 33, 0x300,
     EVENT(YampiDesertCave_BurstRoom1Pillar)},
    {0x9c05, 30, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_TeleportFromFirstMark)},
    {0x9c05, 31, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_TeleportFromSecondMark)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gYampiDesertCave_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {33, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 52, 0x9cd, EVENT(YampiDesertCave_MeetGuardianOfFire)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x303, EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x303,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 100, 0x302,
     EVENT(YampiDesertCave_ScoopTreasure)},
    {TRIGGER_TABLE_END},
};

struct Trigger gYampiDesertCave_Room3Triggers[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {33, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(YampiDesertCave_WonderAtRipple)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(YampiDesertCave_FightCrystal)},
    {0x202, 42, CONDITION_ALWAYS, EVENT(YampiDesertCave_TryPushLog)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 33, 0x301,
     EVENT(YampiDesertCave_BurstRoom3Pillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     0x305, EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x305,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     0x304, EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x304,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {8, 0, CONDITION_ALWAYS, EVENT(YampiDesertCave_StartMovingPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x206,
     EVENT(YampiDesertCave_PoundStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 33,
     IF_FLAG_SET(0x270), EVENT(YampiDesertCave_ScoopDjinni)},
    {TRIGGER_TABLE_END},
};
