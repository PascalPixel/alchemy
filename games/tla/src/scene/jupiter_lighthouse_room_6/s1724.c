/// @file
/// Scene code of overlay 0x69f, in ROM order, from s1724's offset up to the next object the overlay links, and the overlay's data.

#include "../shared/tables.h"
#include "../shared/routines.h"
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

void Func_02000f5c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_FUNC_02001724(Func_02001724, Func_02000f5c)

extern struct CellTarget gData_02002530;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_0200151c(struct CellTarget *work, struct CellTargetEntry *entry);
void Func_0200158c(struct CellTarget *work, struct CellTargetEntry *entry);

DEFINE_FUNC_0200180C(Func_020018bc, gData_02002530, Func_0200151c,
    Func_0200158c)

DEFINE_FUNC_0200196C(Func_0200196c, gData_02002530, Func_02001724)

#include "scene_tables.h"
#include "objcmd.h"

void Func_02000054(void);
void Func_02000084(void);
void Func_02000190(void);

// clang-format off
DEFINE_G_DATA_020012A4(gData_02002068, 22)

DEFINE_G_DATA_020012A4(gData_020020a4, 44)

DEFINE_G_DATA_020012A4(gData_020020e0, 126)

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
    {TRIGGER_EXIT, 1, 0x1a23, 21},
    {TRIGGER_EXIT, 2, 0x1a23, 22},
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
    {TRIGGER_STEP, 20, 0xa19, EVENT(Func_02000190)},
    {TRIGGER_STEP, 21, 0xa19, EVENT(Func_020001b0)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(Func_02000084)},
    {TRIGGER_STEP, 44, CONDITION_ALWAYS, EVENT(Func_02000084)},
    {TRIGGER_STEP, 45, CONDITION_ALWAYS, EVENT(Func_02000084)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0xa19, EVENT(Func_020001d0)},
    {TRIGGERS_END},
};

s32 gData_020023bc[] = {
    -1,
};

u32 gData_020023c0[] = {
    1,
};

u32 gData_0200a3c4[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_020023d8[] = {
    (s32)gData_02002068,
    (s32)gData_020020a4,
    (s32)gData_020020e0,
};
