/// @file
/// Scene code of overlay 0x69c, in ROM order.

#include "../shared/teleporter.h"
#include "../shared/tables.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "../shared/jupiter_lighthouse_aerie.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_room_3.h"
#include "module/scene/scene.h"

void Func_02001788(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

DEFINE_TELEPORTER_DROP_IN(Func_02001f50, Func_02001788)

void Func_02001d48();
void Func_02001db8();

DEFINE_TELEPORTERS_STEP(Func_020020e8, gData_02002e34, Func_02001d48,
    Func_02001db8)

#include "cell_target.h"

DEFINE_TELEPORTER_PLACE_ARRIVAL(Func_02002198, gData_02002e34, Func_02001f50)

#include "scene_tables.h"

// clang-format off
u16 gData_02002478[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gData_02002484[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

DEFINE_G_DATA_0200198C(gData_0200249c)
// clang-format on
