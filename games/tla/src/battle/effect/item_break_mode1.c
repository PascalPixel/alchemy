#include "types.h"
#include "system.h"
#include "actor.h"

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);

/// Spawns object 278 and grows it from a quarter to full size, an eighth of a
/// turn a frame, then faces it to angle.
struct Actor *BattleEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z,
                                                   s32 angle) {
  struct Actor *object;
  s32 scale;
  /* FAKEMATCH: the limit's first byte is pinned to the r2 the native builds it
   * in; as a free temporary it takes r3 and the scale r2 (differ 30). */
  register s32 limit asm("r2");

  Sound_PlayFar(138);
  object = Actor_Spawn(278, x, y, z);
  if (object != NULL) {
    object->scaleY = 0x4000;
    object->scaleX = 0x4000;
    object->speedLimit = 0x30000;
    object->acceleration = 0x30000;
    object->actionFlags = 0;
    Actor_SetMode(object, 1);
    scale = object->scaleX;
    /* FAKEMATCH: the barrier loads the scale before the 0x10000 limit is
     * started; without it the limit is started first (one reordered pair,
     * differ 2). */
    asm volatile("");
    if (scale < 0x10000) {
      do {
        scale += 0x800;
        object->scaleY = scale;
        object->scaleX = scale;
        object->angle += 0x2000;
        wait(1);
        limit = 255;
        scale = object->scaleX;
        /* FAKEMATCH: the native starts the 0xffff limit, loads the scale, then
         * finishes the limit; as a constant the load comes after its shift
         * (differ 2). */
        asm volatile("" : "+l"(limit));
      } while (scale <= (limit << 8) + 255);
    }
    object->angle = angle;
  }
  return object;
}
