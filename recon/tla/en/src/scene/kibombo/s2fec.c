#include "../../../../../../games/tla/src/scene/kibombo/kibombo.h"

void Func_02002fec(struct KibomboObject *object) {
  s32 step;

  object->x += object->field64 << 8;

  object->y += 0x8000;

  /* FAKEMATCH: the temporary builds the scale step before scaleX is loaded (one
   * reordered row). */
  step = 0x51e;
  object->scaleX += step;
  object->scaleY += step;
  object->field64 += 2;
  if (--object->field68 == 0)
    Func_02005a3c(object);
}

void Func_02003030(void) {
  if ((gFrameCount & 63) == 0) {
    struct KibomboObject *object =
        Func_02005a34(0x11d, 0x580000, 0x100000, 0x13a0000);
    u8 *part;
    s32 mask;

    if (object != 0) {
      object->field5c = 2;
      object->motionFlags = 0;
      object->field64 = 0;
      object->priorityFlags &= ~1;
      mask = 13;
      part = (u8 *)object->sprite;
      part[9] = (part[9] & -mask) | 8;
      Actor_SetSingleChildFlags(object, 0);
      object->scaleX = 0x8000;
      object->scaleY = 0x8000;
      object->field68 = 60;
      object->update = Func_02002fec;
      Func_02005a24(object, 5);
      Animation_SetStateFlags(object, 1);
    }
  }
}
