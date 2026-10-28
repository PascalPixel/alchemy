#include "kibombo.h"

void Func_02002fec(struct KibomboObject *object) {
  s32 step;

  object->x += object->field64 << 8;
  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ stores x before it
   * loads y (6 differ without the two blocks). */
  do {
  } while (0);
  object->y += 0x8000;
  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ stores y before it
   * builds the scale step. */
  do {
  } while (0);
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
      /* FAKEMATCH: the loop note ends the scheduling block: the native stores
       * the counter before it takes the flags' address (one reordered pair). */
      do {
      } while (0);
      object->priorityFlags &= ~1;
      mask = 13;
      /* FAKEMATCH: ⚓️ builds the priority mask before loading the sprite, as in
       * Actor_SpawnGroup; the bit-field store loads the sprite first (differ
       * 4). */
      asm volatile("" : "+l"(mask) : : "memory");
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
