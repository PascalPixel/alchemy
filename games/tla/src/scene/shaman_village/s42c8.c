#include "shaman_village.h"

void Func_020042c8(struct ShamanVillageObject *object) {
  s32 step;

  object->x += object->field64 << 8;
  /* FAKEMATCH: the loop note ends the scheduling block: the native loads y
   * before it builds 0x8000 (2 differ without it). */
  do {
  } while (0);
  object->y += 0x8000;
  /* FAKEMATCH: the loop note ends the scheduling block, so y is stored before
   * the step starts and scaleX loads inside it; without it, 4 instructions
   * differ in tla-en. */
  do {
  } while (0);
  step = 0x51e;
  object->scaleX += step;
  object->scaleY += step;
  object->field64 += 2;
  if (--object->field68 == 0)
    Func_0200481c(object);
}
