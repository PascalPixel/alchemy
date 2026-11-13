#include "types.h"
#include "scene_tables.h"
#include "module/scene/trial_road.h"

void TrialRoad_FightGasp(void);
void TrialRoad_TakeStairs(void);
void TrialRoad_OfferToGiveUp(void);
void TrialRoad_OpenChests(void);
void TrialRoad_PutItemInChest(void);
void TrialRoad_ExamineChest(void);
void TrialRoad_UnblockMovedPillar(void);
void TrialRoad_BlockMovedPillar(void);
void TrialRoad_JumpForward(void);
void TrialRoad_UnblockTorch(void);
void TrialRoad_MeltIceBesideTorch(void);
void TrialRoad_FreezePuddle(void);
void TrialRoad_BlowBushOffPillar(void);
void TrialRoad_BlowBushOffPoundPillar(void);
void TrialRoad_PoundPillar18(void);
void TrialRoad_PoundPillar19(void);
void TrialRoad_PoundPillar20(void);
void TrialRoad_PoundPillar21(void);
void TrialRoad_CrumblePillar15(void);
void TrialRoad_CrumblePillar16(void);
void TrialRoad_UncoverSpout16(void);
void TrialRoad_UncoverSpout17(void);
void TrialRoad_UncoverSpout18(void);
void TrialRoad_StandOnBlock(void);
void TrialRoad_OpenSummitGate(s32 sound);
void TrialRoad_SetSummitPillar(void);
void TrialRoad_StepPastChests(void);

extern u8 gTrialRoad_SettleScript[];
extern u8 gFieldEffect_ShortScript[];
extern u8 gFieldEffect_MediumScript[];
extern u8 gFieldEffect_LongScript[];

// clang-format off
u32 gTrialRoad_HexDigits[] = {
    0x33323130, 0x37363534, 0x42413938, 0x46454443, 0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 184, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 344, 0, 264, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(247),
    SCENE_EXIT(1, 247, 3),
    SCENE_EXIT(2, 247, 4),
    SCENE_EXIT(3, 247, 1),
    SCENE_EXIT(4, 247, 2),
    SCENE_EXIT(5, 241, 1),
    SCENE_EXIT(6, 241, 2),
    SCENE_EXIT(7, 248, 1),
    SCENE_EXIT(8, 248, 2),
    SCENE_EXIT(9, 241, 9),
    SCENE_EXIT(10, 241, 11),
    SCENE_EXITS(249),
    SCENE_EXIT(1, 249, 2),
    SCENE_EXIT(2, 249, 1),
    SCENE_EXIT(3, 248, 3),
    SCENE_EXIT(4, 250, 1),
    SCENE_EXITS(250),
    SCENE_EXIT(1, 249, 4),
    SCENE_EXITS(248),
    SCENE_EXIT(1, 247, 33),
    SCENE_EXIT(2, 247, 34),
    SCENE_EXIT(3, 249, 3),
    SCENE_EXIT(4, 244, 30),
    SCENE_EXIT(5, 244, 31),
    SCENE_EXITS_END,
};

struct ActorPlacement gTrialRoad_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_PillarStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(344), FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(456), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(392), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(456), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(392), FACING_EAST, 2, 1},
    {307, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, PIXELS(248), 0, PIXELS(400),
     FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_TorchStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(424), FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(424), FACING_EAST, 2, 1},
    {290, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(440), FACING_EAST, 2, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(440), FACING_EAST, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(456), FACING_EAST, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(456), FACING_EAST, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_BushPillarStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(424), FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(424), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(440), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(456), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(456), FACING_EAST, 2, 1},
    {307, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, PIXELS(232), 0, PIXELS(432),
     FACING_EAST, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, PIXELS(296), 0, PIXELS(432),
     FACING_EAST, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_PoundStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(456), FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(456), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(456), FACING_EAST, 2, 1},
    {406, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(424), FACING_EAST, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(424), FACING_EAST, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(424), FACING_EAST, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(424), FACING_EAST, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, PIXELS(264), 0, PIXELS(464),
     FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_CrumblingStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {404, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(440), FACING_EAST, 2, 0},
    {404, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(440), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_LogStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(392), FACING_EAST, 2, 0},
    {261, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(392), FACING_EAST, 2, 0},
    {261, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(440), FACING_EAST, 2, 0},
    {260, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(456), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_FountainStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(472), FACING_EAST, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(376), FACING_EAST, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(376), FACING_EAST, 2, 1},
    {489, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(376), FACING_EAST, 2, 1},
    {489, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(440), FACING_EAST, 2, 1},
    {489, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(440), FACING_EAST, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_SandStagePlacements[] = {
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(152), FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(408), FACING_EAST, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(408), FACING_EAST, 2, 1},
    {307, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, PIXELS(312), 0, PIXELS(336),
     FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_Room3Placements[] = {
    {329, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(280), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_Room4Placements[] = {
    {246, 0x7b, 1, PIXELS(344), 0, PIXELS(264), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTrialRoad_Room2Placements[] = {
    {4, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(328), FACING_EAST, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

s32 gTrialRoad_Room1Placements[] = {
    (s32)gTrialRoad_PillarStagePlacements,
    (s32)gTrialRoad_TorchStagePlacements,
    (s32)gTrialRoad_BushPillarStagePlacements,
    (s32)gTrialRoad_PoundStagePlacements,
    (s32)gTrialRoad_CrumblingStagePlacements,
    (s32)gTrialRoad_LogStagePlacements,
    (s32)gTrialRoad_FountainStagePlacements,
    (s32)gTrialRoad_SandStagePlacements,
};

struct Trigger gTrialRoad_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gTrialRoad_PillarStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0x4602, 51, IF_FLAG_SET(0x208), EVENT(TrialRoad_JumpAcrossGap)},
    {0x4602, 52, IF_FLAG_SET(0x208), EVENT(TrialRoad_JumpForward)},
    {8, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, IF_FLAG_SET(0x307), EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, IF_FLAG_SET(0x307), EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 20, 0x307, EVENT(TrialRoad_BlowBushOffLonePillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTrialRoad_TorchStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0xc602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpMeltedPuddle)},
    {0x4602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpMeltedPuddle)},
    {8, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 21, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 22, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {9, 0, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 21, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 22, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, IF_FLAG_SET(0x214), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, IF_FLAG_SET(0x215), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, IF_FLAG_SET(0x216), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, IF_FLAG_SET(0x217), EVENT(TrialRoad_FreezePuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTrialRoad_BushPillarStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0x4602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {8, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, IF_FLAG_SET(0x327), EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, IF_FLAG_SET(0x328), EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, IF_FLAG_SET(0x327), EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, IF_FLAG_SET(0x328), EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 20, 0x327, EVENT(TrialRoad_BlowBushOffPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 21, 0x328, EVENT(TrialRoad_BlowBushOffPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTrialRoad_PoundStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0x202, 50, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {0x202, 51, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {0x202, 52, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {0x202, 53, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {TRIGGER_STEP, 50, 0x335, EVENT(TrialRoad_StepOnPillar18)},
    {TRIGGER_STEP, 51, IF_FLAG_SET(0x335), EVENT(TrialRoad_StepOnPillar19)},
    {TRIGGER_STEP, 52, 0x337, EVENT(TrialRoad_StepOnPillar20)},
    {TRIGGER_STEP, 53, IF_FLAG_SET(0x337), EVENT(TrialRoad_StepOnPillar21)},
    {0x202, 60, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(TrialRoad_CheckPoundedPillarsCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, IF_FLAG_SET(0x339), EVENT(TrialRoad_CheckPoundedPillarsCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(TrialRoad_CheckPoundedPillarsCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 22, 0x339, EVENT(TrialRoad_BlowBushOffPoundPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 18, 0x335, EVENT(TrialRoad_PoundPillar18)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 19, IF_FLAG_SET(0x335), EVENT(TrialRoad_PoundPillar19)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 20, 0x337, EVENT(TrialRoad_PoundPillar20)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 21, IF_FLAG_SET(0x337), EVENT(TrialRoad_PoundPillar21)},
    {TRIGGER_TABLE_END},
};

s16 gTrialRoad_CrumblingPillars[] = {
    15, 834, 16, 836, -1, 0,
};
// clang-format on

struct Trigger gTrialRoad_CrumblingStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0x4602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {0xc602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {0xc602, 50, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {0xc602, 51, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {0x602, 34, CONDITION_ALWAYS, EVENT(TrialRoad_CheckCrumblingPillars)},
    {0x8602, 34, CONDITION_ALWAYS, EVENT(TrialRoad_CheckCrumblingPillars)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50, 0x343,
     EVENT(TrialRoad_CrumblePillar15)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51, 0x345,
     EVENT(TrialRoad_CrumblePillar16)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTrialRoad_LogStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {0x602, 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {0x8602, 50, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog16West)},
    {0x602, 50, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog16East)},
    {0x8602, 51, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog17West)},
    {0x602, 51, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog17East)},
    {0xc602, 50, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18North)},
    {0x4602, 50, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18South)},
    {0xc602, 51, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18North)},
    {0x4602, 51, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18South)},
    {0xc602, 52, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18North)},
    {0x4602, 52, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18SouthFromRow52)},
    {0xc602, 53, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18North)},
    {0x4602, 53, CONDITION_ALWAYS, EVENT(TrialRoad_PushLog18South)},
    {8, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS,
     EVENT(TrialRoad_BlockMovedPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTrialRoad_FountainStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(TrialRoad_StandOnBlock)},
    {8, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS,
     EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 50, 0x363,
     EVENT(TrialRoad_UncoverSpout16)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 51, 0x364,
     EVENT(TrialRoad_UncoverSpout17)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 52, 0x365,
     EVENT(TrialRoad_UncoverSpout18)},
    {TRIGGER_TABLE_END},
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct Trigger gTrialRoad_SandStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(0x281), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, 0x281, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, 0x284, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x284), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, 0x284, EVENT(TrialRoad_OpenChests)},
    {0x1fe14, 0x409, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40a, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40b, IF_FLAG_SET(0x285), EVENT(TrialRoad_PutItemInChest)},
    {0x1fe14, 0x40c, IF_FLAG_SET(0x286), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(TrialRoad_DriftWithStream)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_StartSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(TrialRoad_EndSand)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, 0x374,
     EVENT(TrialRoad_RevealVine)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTrialRoad_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 8, 0x289, EVENT(TrialRoad_LiftCaveRock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTrialRoad_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33,
     CONDITION_ALWAYS, EVENT(TrialRoad_CutBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34,
     CONDITION_ALWAYS, EVENT(TrialRoad_CutMadPlantBushes)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(TrialRoad_FightGasp)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_RevealPoolTiles)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_HidePoolTiles)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTrialRoad_Room2Triggers[] = {
    {TRIGGER_STEP, 33, 0x283, EVENT(TrialRoad_OpenSummitGate)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(0x9ff), EVENT(TrialRoad_ChallengeMoapa)},
    {TRIGGER_STEP, 41, IF_FLAG_SET(0x9ff), EVENT(TrialRoad_ChallengeMoapa)},
    {49, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x9fc,
     EVENT(TrialRoad_SetSummitPillar)},
    {9, 0, 0x9fc, EVENT(TrialRoad_SetSummitPillar)},
    {TRIGGER_TABLE_END},
};

s32 gTrialRoad_Room1Triggers[] = {
    (s32)gTrialRoad_PillarStageTriggers,     (s32)gTrialRoad_TorchStageTriggers,
    (s32)gTrialRoad_BushPillarStageTriggers, (s32)gTrialRoad_PoundStageTriggers,
    (s32)gTrialRoad_CrumblingStageTriggers,  (s32)gTrialRoad_LogStageTriggers,
    (s32)gTrialRoad_FountainStageTriggers,   (s32)gTrialRoad_SandStageTriggers,
};

s32 gTrialRoad_PaletteCycleFrame[] = {
    -1,
};

u32 gTrialRoad_PaletteCycleRunning[] = {
    1,
};
