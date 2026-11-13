/// @file
/// The tables of Jupiter Lighthouse's left-tower room that the scene's getters
/// return.

#include "../shared/tables.h"
#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"
#include "../jupiter_lighthouse_room_8.h"

#include "field/cell_target.h"
void JupiterLighthouse_PushBlock(void);
extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];
extern u32 gFieldEffect_LongLifeScript[];

void JupiterLighthouse_StepOnTeleporter(void);
void JupiterLighthouse_DropBlockIntoHole(void);

void JupiterLighthouse_StartMovingBlock(void);

// clang-format off
DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_ShortLifeScript, 22)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_MediumLifeScript, 44)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_LongLifeScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(261),
    SCENE_EXIT(1, 261, 2),
    SCENE_EXIT(2, 261, 1),
    SCENE_EXIT(3, 261, 4),
    SCENE_EXIT(4, 261, 3),
    SCENE_EXIT(5, 261, 6),
    SCENE_EXIT(6, 261, 5),
    SCENE_EXIT(7, 263, 7),
    SCENE_EXIT(8, 259, 8),
    SCENE_EXIT(9, 261, 10),
    SCENE_EXIT(10, 261, 9),
    SCENE_EXIT(11, 261, 12),
    SCENE_EXIT(12, 261, 11),
    SCENE_EXIT(13, 261, 13),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {340, CONDITION_ALWAYS, 1, PIXELS(203), 0, PIXELS(904), FACING_SOUTHEAST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(296), FACING_SOUTHWEST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {331, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(160), FACING_SOUTH, 2, 0},
    {331, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(160), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 1, PIXELS(400), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 1, PIXELS(480), 0, PIXELS(200), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 1, PIXELS(560), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {33, 3, CONDITION_ALWAYS, 3},
    {49, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {33, 8, CONDITION_ALWAYS, 8},
    {33, 9, CONDITION_ALWAYS, 9},
    {49, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {514, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushBlock)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnTeleporter)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnTeleporter)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnTeleporter)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(JupiterLighthouse_DropBlockIntoHole)},
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

s32 gFieldEffect_LifeScripts[] = {
    (s32)gFieldEffect_ShortLifeScript,
    (s32)gFieldEffect_MediumLifeScript,
    (s32)gFieldEffect_LongLifeScript,
};
