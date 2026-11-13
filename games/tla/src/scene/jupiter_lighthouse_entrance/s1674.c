/// @file
/// Scene code of overlay 0x698, in ROM order.

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
#include "module/scene/jupiter_lighthouse_entrance.h"
#include "module/scene/scene.h"


static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

DEFINE_TELEPORTER_DROP_IN_LOWER(Teleporter_DropInLower, Func_02000eac)

void Teleporter_Uncover();
void Teleporter_Sink();

DEFINE_TELEPORTERS_STEP(Teleporters_Step, gTeleporters, Teleporter_Uncover,
    Teleporter_Sink)

#include "cell_target.h"

DEFINE_TELEPORTER_PLACE_ARRIVAL(Teleporter_PlaceArrival, gTeleporters, Teleporter_DropInLower)

#include "scene_tables.h"
#include "objcmd.h"

void Func_02000054(void);
void Func_02000094(void);

// clang-format off
DEFINE_G_DATA_020012A4(gData_02001fc0, 22)

DEFINE_G_DATA_020012A4(gData_02001ffc, 44)

DEFINE_G_DATA_020012A4(gData_02002038, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS_2(251, 2, 55, 254, 2, 254, 3, 252, 4)

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(360), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(360), FACING_SOUTHEAST, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_02000074)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(Func_02000094)},
    {0x50008905, 33, CONDITION_ALWAYS, EVENT(Func_020000b4)},
    {0x50008905, 34, CONDITION_ALWAYS, EVENT(Func_020000d4)},
    {0x50008905, 35, CONDITION_ALWAYS, EVENT(Func_02000184)},
    {0x50008905, 36, CONDITION_ALWAYS, EVENT(Func_020000f4)},
    {TRIGGERS_END},
};

s32 gPaletteCycleFrame[] = {
    -1,
};

u32 gData_02002198[] = {
    1,
};

u32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_020021b0[] = {
    (s32)gData_02001fc0,
    (s32)gData_02001ffc,
    (s32)gData_02002038,
};
