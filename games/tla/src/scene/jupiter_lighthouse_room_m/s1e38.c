#include "jupiter_lighthouse_room_m.h"

extern u32 gData_02005548[];

void Func_0200484c(s32 id, s32 x, s32 z, s32 angle);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);

static __inline__ void ObjPlace(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200484c(id, x, z, angle);
}

/* FAKEMATCH: the angle passes through a block temporary: the native shares the
 * first two angles in r5 and builds the later ones before z (a plain argument:
 * differ 24, one function-wide angle local: differ 10-16). */
#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    s32 angle_ = (angle);                                                      \
    ObjPlace(id, (x) << 16, (z) << 16, angle_);                                \
  }

void Func_02001e38(void) {
  PLACE(0, 761, 219, 0xa000);
  PLACE(2, 741, 232, 0xa000);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(0), 0);
  Object_SetModeById(0, 39);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(2), 0);
  Object_SetModeById(2, 32);
  PLACE(5, 761, 203, 0x4000);
  PLACE(6, 725, 232, 0);
  PLACE(28, 729, 203, 0x2000);
  ObjectMotion_EnableActionAndSetCallback(3, gData_02005548);
}
