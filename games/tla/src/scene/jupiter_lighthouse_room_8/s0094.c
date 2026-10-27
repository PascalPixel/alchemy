#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void WaitTicks(s32 ticks);
void Func_020021e0(s32 x, s32 z, s32 a, s32 b);
void Func_020022c0(s32 x, s32 z, s32 a, s32 b);
void Func_020022c8(s32 cue);

/// On cell (6, 14), sets flag 0xa18, waits until the object finishes falling
/// and stops (at most 30 frames), then releases it.
void Func_02000094(s32 unused, s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);
  s32 i;
  s32 y;
  s32 floor;

  if (object->x >> 20 == 6 && object->z >> 20 == 14) {
    GameFlag_SetBit(0xa18);
    WaitTicks(10);
    Func_020022c8(134);
    y = object->y;
    floor = object->terrainHeight;
    /* FAKEMATCH(barrier): the height and the floor are read before the counter
     * is cleared and again at the end of each pass; as plain C, 8 of 50
     * instructions differ in tla-en. */
    __asm__ volatile("");
    i = 0;
    goto test;
    do {
      WaitTicks(1);
      if (++i > 29)
        break;
      y = object->y;
      floor = object->terrainHeight;
    test:;
    } while (y > floor || object->velocityY != 0);
    {
      s32 zero;
      /* FAKEMATCH(temporary): the motion flags cleared from a zero of their
       * own; as a plain 0 store, 2 of 50 instructions differ in tla-en. */
      s32 cleared = 0;

      object->motionFlags = cleared;
      zero = 0;
      /* FAKEMATCH(barrier): the call's zero is built before the position
       * loads; as a plain 0 argument, 2 of 50 instructions differ in tla-en. */
      __asm__("" : : "r"(zero));
      Func_020021e0(object->x, object->z, zero, 0);
    }
    Func_020022c0(object->x, object->z, 0, 128);
  }
}
