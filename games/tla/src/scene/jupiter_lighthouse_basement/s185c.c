/// @file
/// Scene code of overlay 0x699, in ROM order.

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
#include "module/scene/jupiter_lighthouse_basement.h"
#include "module/scene/scene.h"

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_TELEPORTER_DROP_IN_LOWER(Teleporter_DropInLower, Func_02001094)

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
void Func_02000094(void);
void Func_02000124(void);

// clang-format off
DEFINE_G_DATA_020012A4(gData_020021c8, 22)

DEFINE_G_DATA_020012A4(gData_02002204, 44)

DEFINE_G_DATA_020012A4(gData_02002240, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(252),
    SCENE_EXIT(1, 253, 1),
    SCENE_EXIT(2, 252, 3),
    SCENE_EXIT(3, 252, 2),
    SCENE_EXIT(4, 251, 4),
    SCENE_EXIT(6, 254, 6),
    SCENE_EXITS(253),
    SCENE_EXIT(1, 252, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gData_020022e8[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_02000074)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(Func_020000f8)},
    {0x50008905, 33, CONDITION_ALWAYS, EVENT(Func_02000118)},
    {0x50008905, 34, CONDITION_ALWAYS, EVENT(Func_02000124)},
    {0x50008905, 38, CONDITION_ALWAYS, EVENT(Func_02000094)},
    {TRIGGERS_END},
};

struct Trigger gData_0200236c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGERS_END},
};

s32 gPaletteCycleFrame[] = {
    -1,
};

u32 gData_02002388[] = {
    1,
};

u32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_020023a0[] = {
    (s32)gData_020021c8,
    (s32)gData_02002204,
    (s32)gData_02002240,
};
