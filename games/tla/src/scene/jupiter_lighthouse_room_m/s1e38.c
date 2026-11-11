#include "jupiter_lighthouse_room_m.h"
#include "scene_tables.h"
#include "event_inline.h"

extern u32 gData_02005548[];

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void ObjPlace(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    /* FAKEMATCH(temporary): the angle passes through a block local, so the    \
     * first two angles are shared in r5 and the later ones built before z,    \
     * as the native does; as a plain argument, 24 of 53 instructions differ   \
     * in tla-en (as one function-wide angle local, 10 to 16). */              \
    s32 angle_ = (angle);                                                      \
    ObjPlace(id, (x) << 16, (z) << 16, angle_);                                \
  }

void Func_02001e38(void) {
  PLACE(0, 761, 219, FACING_NORTHWEST);
  PLACE(2, 741, 232, FACING_NORTHWEST);
  Actor_SetSingleChildFlags(Actor_Get(0), 0);
  Actor_SetAnimation(0, 39);
  Actor_SetSingleChildFlags(Actor_Get(2), 0);
  Actor_SetAnimation(2, 32);
  PLACE(5, 761, 203, FACING_SOUTH);
  PLACE(6, 725, 232, FACING_EAST);
  PLACE(28, 729, 203, FACING_SOUTHEAST);
  Actor_SetScript(3, gData_02005548);
}
