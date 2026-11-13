/// @file
/// Scene code of overlay 0x6a1, in ROM order.

#include "../shared/teleporter.h"
#include "../shared/tables.h"
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
#include "module/scene/jupiter_lighthouse_room_8.h"
#include "module/scene/scene.h"

void Func_02001600(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_TELEPORTER_DROP_IN(Teleporter_DropIn, Func_02001600)

void GameFlag_SetByte(s32 flag, s32 value);
void Teleporter_Sink(struct CellTarget *work, struct CellTargetEntry *entry);

DEFINE_TELEPORTERS_STEP(Teleporters_Step, gData_02002bf8, Teleporter_Uncover,
    Teleporter_Sink)

DEFINE_TELEPORTER_PLACE_ARRIVAL(Teleporter_PlaceArrival, gData_02002bf8, Teleporter_DropIn)

#include "scene_tables.h"

// clang-format off
u16 gData_020022d0[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gData_020022dc[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

DEFINE_G_DATA_0200198C(gData_020022f4)
// clang-format on
