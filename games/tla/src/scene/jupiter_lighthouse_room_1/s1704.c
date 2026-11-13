/// @file
/// Scene code of overlay 0x69a, in ROM order.

#include "../shared/tables.h"
#include "../shared/teleporter.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "../shared/jupiter_lighthouse_aerie.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_room_1.h"
#include "module/scene/scene.h"

void Func_02000f3c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_TELEPORTER_DROP_IN_LOWER(Teleporter_DropInLower, Func_02000f3c)

extern struct CellTarget gTeleporters;
void Teleporter_Uncover();
void Teleporter_Sink();

DEFINE_TELEPORTERS_STEP(Teleporters_Step, gTeleporters, Teleporter_Uncover,
                        Teleporter_Sink)

#include "cell_target.h"

DEFINE_TELEPORTER_PLACE_ARRIVAL(Teleporter_PlaceArrival, gTeleporters,
                                Teleporter_DropInLower)

#include "scene_tables.h"
#include "objcmd.h"

void Func_02000054(void);
void Func_020001f4(void);

// clang-format off
DEFINE_G_DATA_020012A4(gData_02002050, 22)

DEFINE_G_DATA_020012A4(gData_0200208c, 44)

DEFINE_G_DATA_020012A4(gData_020020c8, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(254),
    SCENE_EXIT(1, 255, 1),
    SCENE_EXIT(2, 251, 2),
    SCENE_EXIT(3, 251, 3),
    SCENE_EXIT(4, 254, 5),
    SCENE_EXIT(5, 254, 4),
    SCENE_EXIT(6, 252, 6),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(744), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(744), FACING_SOUTHEAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(536), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 20, 0xa01, EVENT(Scene_RunOnce)},
    {TRIGGER_STEP, 21, 0xa01, EVENT(Scene_RunIfFlagCleared)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0xa01, EVENT(Func_020001f4)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame[] = {
    -1,
};

u32 gData_02002230[] = {
    1,
};

u32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_02002248[] = {
    (s32)gData_02002050,
    (s32)gData_0200208c,
    (s32)gData_020020c8,
};
