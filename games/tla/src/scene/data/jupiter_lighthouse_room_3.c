/// @file
/// The tables of Jupiter Lighthouse's room of gaps and logs that the scene's
/// getters return.

#include "../shared/tables.h"
#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

#include "field/cell_target.h"
#include "module/scene/scene.h"
void JupiterLighthouse_StartMovingPillar(void);
void JupiterLighthouse_LeaveByCyclone(void);
void JupiterLighthouse_JumpForward(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void JupiterLighthouse_PushLogOffTheLedge(void);
void JupiterLighthouse_FinishMovingPillar(void);

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(256),
    SCENE_EXIT(1, 263, 1),
    SCENE_EXIT(2, 257, 2),
    SCENE_EXIT(3, 263, 3),
    SCENE_EXIT(4, 263, 4),
    SCENE_EXIT(6, 255, 6),
    SCENE_EXIT(7, 255, 7),
    SCENE_EXIT(8, 255, 8),
    SCENE_EXIT(9, 255, 9),
    SCENE_EXIT(10, 255, 10),
    SCENE_EXIT(11, 255, 11),
    SCENE_EXIT(12, 257, 12),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {332, CONDITION_ALWAYS, 1, PIXELS(144), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(200), FACING_SOUTH, 2, 0},
    {331, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(192), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(568), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(568), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(584), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(584), FACING_SOUTH, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(680), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(680), FACING_SOUTHEAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(840), FACING_SOUTHWEST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(840), FACING_SOUTHEAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(840), FACING_SOUTHEAST, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {49, 6, CONDITION_ALWAYS, 6},
    {49, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {514, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushLogOffTheLedge)},
    {0x8602, 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpForward)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14, CONDITION_ALWAYS, (u32)((u8 *)JupiterLighthouse_StartMovingPillar + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, (u32)((u8 *)JupiterLighthouse_StartMovingPillar + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByCyclone)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame[] = {
    -1,
};

u32 gPaletteCyclePausesInEvents[] = {
    1,
};

u32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
