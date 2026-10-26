#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"

s32 Math_Sine(s32 angle);

s32 Func_020039a8(struct ObjectRuntime *object) {
  struct ObjectRuntime *parent = object->linkedObject;
  s32 step = ++object->action;
  s32 scale;

  if (step > 31)
    return 0;
  scale = Iwram_MulQ16(Math_Sine(step << 10), 0x14000);
  object->scaleX = scale;
  object->scaleY = scale;
  object->x = parent->x;
  object->y += 0x10000;
  object->z = parent->z;
  return 1;
}
