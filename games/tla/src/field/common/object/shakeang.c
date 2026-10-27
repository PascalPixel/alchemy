#include "types.h"
#include "object_runtime.h"
#include "system.h"

void Object_ShakeAngle(u32 objectId) {
  struct ObjectRuntime *object;
  s32 angle;
  s32 i;

  object = ObjectTable_Get(objectId);
  if (object == 0)
    return;
  angle = object->angle;
  for (i = 15; i >= 0; i--) {
    u32 a = Random16();
    u32 b = Random16();
    u32 cur;

    /* FAKEMATCH: the one-pass block reads the angle before either random value
     * is shifted, the second first; added in place, 2 instructions differ in
     * tla-en. */
    do {
      cur = object->angle;
    } while (0);
    b >>= 4;
    a >>= 4;
    object->angle = cur + (a - b);
    WaitTicks(1);
  }
  object->angle = angle;
}
