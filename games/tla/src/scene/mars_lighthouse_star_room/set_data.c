/// @file
/// Scene code of overlay 0x6ab, in ROM order.

#include "../shared/mars_lighthouse.h"
#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"
#include "../mars_lighthouse_star_room.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "module/scene/scene.h"
#include "module/tla.h"

extern u16 gData_02004cf0[];
s32 Import_FieldCheckSpheresOverlap(s32 *a, s32 radiusA, struct Position *b, s32 radiusB);

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void MarsLighthouseStarRoom_SlideOnIce(void);

void Func_02003384(u16 value) { gData_02004cf0[0] = value; }

#include "fixed_math.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};


DEFINE_LEADER_SLIDE_ON_ICE_AHEAD(Func_02003390, Func_02002d90, Import_ActorSelectAnimation, Func_02002d04,
    Import_FieldCheckSpheresOverlap)

#include "../../field/object/set_sprite_priority.c"
