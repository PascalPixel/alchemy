/// @file
/// Scene code of overlay 0x6a2, in ROM order.

#include "../shared/tables.h"
#include "../shared/teleporter.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "cell_target.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_room_9.h"
#include "module/scene/scene.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_TELEPORTER_DROP_IN(Teleporter_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);
void Teleporter_Sink(struct CellTarget *work, struct CellTargetEntry *entry);

DEFINE_TELEPORTERS_STEP(Teleporters_Step, gTeleporters, Teleporter_Uncover,
                        Teleporter_Sink)

DEFINE_TELEPORTER_PLACE_ARRIVAL(Teleporter_PlaceArrival, gTeleporters,
                                Teleporter_DropIn)

#include "scene_tables.h"
#include "objcmd.h"

void JupiterLighthouse_StartHovering(void);
void JupiterLighthouse_RaiseWhirlwind(void);
void JupiterLighthouse_RaiseUpperWhirlwind(void);
void JupiterLighthouse_StepOnLowerTeleporter(void);
void JupiterLighthouse_StepOnUpperTeleporter(void);
void JupiterLighthouse_StepOnTeleporterOrWhirlwind(void);

extern u32 gFieldEffect_LongLifeScript[];

// clang-format off
DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_ShortLifeScript, 22)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_MediumLifeScript, 44)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_LongLifeScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(262),
    SCENE_EXIT(1, 260, 1),
    SCENE_EXIT(2, 262, 3),
    SCENE_EXIT(3, 262, 2),
    SCENE_EXIT(4, 264, 4),
    SCENE_EXIT(5, 262, 6),
    SCENE_EXIT(6, 262, 5),
    SCENE_EXIT(7, 262, 8),
    SCENE_EXIT(8, 262, 7),
    SCENE_EXIT(13, 262, 13),
    SCENE_EXIT(14, 262, 14),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {489, CONDITION_ALWAYS, 11, -0x80000, 0, -0x80000, FACING_WEST, 2, 0},
    {317, CONDITION_ALWAYS, 11, PIXELS(424), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {317, CONDITION_ALWAYS, 11, PIXELS(440), 0, PIXELS(168), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(664), 0, PIXELS(200), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(680), 0, PIXELS(200), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(440), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(408), 0, PIXELS(344), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(424), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(440), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(456), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(472), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(488), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(504), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(520), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(536), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(552), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(568), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(584), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(600), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(616), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(632), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(648), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(664), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(680), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(696), 0, PIXELS(344), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {49, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {33, 5, CONDITION_ALWAYS, 5},
    {49, 6, CONDITION_ALWAYS, 6},
    {33, 7, CONDITION_ALWAYS, 7},
    {49, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseWhirlwind)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseWhirlwind)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 52, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 53, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Whirlwind_CarryLeader)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerTeleporter)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnTeleporterOrWhirlwind)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerTeleporter)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperTeleporter)},
    {TRIGGER_STEP, 44, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperTeleporter)},
    {TRIGGER_STEP, 45, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperTeleporter)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartHovering)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StopHovering)},
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
