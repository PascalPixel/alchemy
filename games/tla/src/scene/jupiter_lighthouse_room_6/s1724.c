/// @file
/// Scene code of overlay 0x69f, in ROM order.

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
#include "module/scene/jupiter_lighthouse_room_6.h"
#include "module/scene/scene.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_TELEPORTER_DROP_IN(Teleporter_DropIn, FieldEffect_Spawn)

extern struct CellTarget gTeleporters;

void GameFlag_SetByte(s32 flag, s32 value);
void Teleporter_Uncover(struct CellTarget *work, struct CellTargetEntry *entry);
void Teleporter_Sink(struct CellTarget *work, struct CellTargetEntry *entry);

DEFINE_TELEPORTERS_STEP(Teleporters_Step, gTeleporters, Teleporter_Uncover,
                        Teleporter_Sink)

DEFINE_TELEPORTER_PLACE_ARRIVAL(Teleporter_PlaceArrival, gTeleporters,
                                Teleporter_DropIn)

#include "scene_tables.h"
#include "objcmd.h"

void Func_02000054(void);
void Func_02000084(void);
void Scene_RunOnce(void);

// clang-format off
DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_ShortLifeScript, 22)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_MediumLifeScript, 44)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_LongLifeScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(259),
    SCENE_EXIT(1, 264, 1),
    SCENE_EXIT(2, 264, 2),
    SCENE_EXIT(3, 264, 3),
    SCENE_EXIT(4, 259, 5),
    SCENE_EXIT(5, 259, 4),
    SCENE_EXIT(6, 259, 7),
    SCENE_EXIT(7, 259, 6),
    SCENE_EXIT(8, 261, 8),
    SCENE_EXIT(9, 258, 9),
    SCENE_EXIT(10, 258, 10),
    SCENE_EXIT(11, 259, 12),
    SCENE_EXIT(12, 259, 11),
    SCENE_EXIT(13, 259, 13),
    SCENE_EXIT(14, 261, 14),
    SCENE_EXIT(21, 265, 1),
    SCENE_EXIT(22, 265, 2),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(456), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(456), FACING_SOUTHEAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(632), FACING_SOUTHWEST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(632), FACING_SOUTHEAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(600), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(184), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(184), FACING_SOUTHEAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(264), FACING_SOUTHWEST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(264), FACING_SOUTHEAST, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, IF_FLAG_SET(0xa23), 21},
    {TRIGGER_EXIT, 2, IF_FLAG_SET(0xa23), 22},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {33, 4, CONDITION_ALWAYS, 4},
    {49, 5, CONDITION_ALWAYS, 5},
    {33, 6, CONDITION_ALWAYS, 6},
    {49, 7, CONDITION_ALWAYS, 7},
    {49, 8, CONDITION_ALWAYS, 8},
    {49, 9, CONDITION_ALWAYS, 9},
    {49, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 20, 0xa19, EVENT(Scene_RunOnce)},
    {TRIGGER_STEP, 21, 0xa19, EVENT(Scene_RunIfFlagCleared)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(Func_02000084)},
    {TRIGGER_STEP, 44, CONDITION_ALWAYS, EVENT(Func_02000084)},
    {TRIGGER_STEP, 45, CONDITION_ALWAYS, EVENT(Func_02000084)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0xa19, EVENT(Func_020001d0)},
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
