#include "../../../../../../games/tla/src/scene/shaman_village/shaman_village.h"
#include "call.h"

void Func_020042c8(struct ShamanVillageObject *object) {
  s32 step;

  object->x += object->field64 << 8;

  object->y += 0x8000;

  step = SPLIT_CONSTANT(0x51e);
  object->scaleX += step;
  object->scaleY += step;
  object->field64 += 2;
  if (--object->field68 == 0)
    Func_0200481c(object);
}
