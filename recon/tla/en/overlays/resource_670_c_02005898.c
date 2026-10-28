// 26 October 2026, under -mthumb-slow-loads: differ 4 of 105: two calls' first
// argument, which the native builds before the other constants; alchemy permute
// (5 to 6 minutes) and an empty asm or one-pass block after any one statement
// do not move it.
// Draft, differ 4 of 105 (was 30; English: the Japanese listing lacks the block
// under #if). An empty asm after the second placement call keeps the two
// calls' tails apart (cross-jumping shared them, 30 to 12), and the angle
// pinned to r3 in each builds it first, as the native does (12 to 8). Left:
// the native builds the actor (movs r0, #8) between the angle and x halves,
// here last (pinning x, z and the actor too does not move it, nor an asm on
// the angle that clobbers r0, 6). Loading x and z before their steps are
// built, as the native does, takes an asm on each that clobbers r2 (8 to 4).
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

void Func_02003288(void);
void Func_02004cf4(void);
void Func_02005c74(s32 actor, s32 value);

void Func_02005898(void) {
  struct KibomboObject *object;

  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 521;
  Func_02003288();
  if (gAdventure.entrance == 9) {
    if (GameFlag_Test(0x8ef)) {
      Func_02005b0c(10, 0, 0);
      if (GameFlag_Test(0x8ee)) {
        /* FAKEMATCH(pin): the angle in r3, so it is built first, as the native
         * builds it. */
        register s32 angle asm("r3") = 0xb000;

        Func_02005b14(8, 0x1b20000, 0x27a0000, angle);
      } else {
        /* FAKEMATCH(pin): the angle in r3, so it is built first, as the native
         * builds it. */
        register s32 angle asm("r3") = 0xb000;

        Func_02005b14(8, 0x16c0000, 0x2400000, angle);
        /* FAKEMATCH(barrier): keeps the two calls' tails apart, as the native
         * keeps them. */
        __asm__ volatile("");
      }
    } else {
      Func_02004cf4();
    }
  }
#if !defined(TLA_EDITION_JA)
  if (GameFlag_Test(0x109)) {
    object = Object_GetById(16);
    object->x -= 0xe0000;
    object->z -= 0x80000;
  }
#endif
  Func_02005c74(16, 0);
  object = Object_GetById(16);
  object->sprite->rotation = 0;
  object->scaleX = 0x10000;
  object->scaleY = 0x10000;
  {
    s32 x = object->x;

    /* FAKEMATCH(barrier): clobbering r2 loads x before the step is built in
     * r2, as the native does. */
    __asm__("" : "+l"(x) : : "r2");
    object->x = x + 0xe0000;
  }
  {
    s32 z = object->z;

    /* FAKEMATCH(barrier): as for x. */
    __asm__("" : "+l"(z) : : "r2");
    object->z = z + 0x80000;
  }
  Object_SetModeById(16, 6);
  if (gAdventure.entrance == 14)
    GameFlag_SetBit(0x8ee);
}
