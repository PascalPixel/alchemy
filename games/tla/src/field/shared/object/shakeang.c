#include "types.h"
#include "actor.h"
#include "system.h"

void Actor_ShakeAngle(u32 objectId) {
  struct Actor *object;
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
    WaitFrames(1);
  }
  object->angle = angle;
}
