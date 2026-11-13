/// @file
/// Scene code of overlay 0x6aa, in ROM order.

#include "../shared/mars_lighthouse.h"
#include "types.h"
#include "actor.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "event_inline.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"
#include "module/tla.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Func_020031f8(void);

DEFINE_LEADER_SLIDE_ON_ICE_AHEAD(Func_02003884, Func_02003284, Func_02004bc0, Func_020031f8,
    Func_02004c78)

#include "../../field/object/set_sprite_priority_b.c"
