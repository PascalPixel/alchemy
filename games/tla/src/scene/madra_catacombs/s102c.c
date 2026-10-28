#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_02000958(void);

/// Mirrors actors 8 and 9 and puts them on the ground; once flag 0x8c9 is set,
/// each steps 12 pixels outward unless flag 0x109 is set too, and Func_02000958
/// runs.
void Func_0200102c(void) {
  struct Actor *left = Actor_GetById(8);
  struct Actor *right = Actor_GetById(9);

  left->scaleX = 0x14000;
  right->scaleX = -0x14000;
  left->motionFlags = 0;
  right->motionFlags = 0;
  left->y = 0;
  right->y = 0;
  if (GameFlag_Test(0x8c9) != 0) {
    if (GameFlag_Test(0x109) == 0) {
      left->x += -0xc0000;
      {
        s32 x = right->x;

        /* FAKEMATCH(barrier): an empty asm on the loaded x, so the step's
         * first half is built before right->x loads; as right->x += 0xc0000, 2
         * of 43 instructions differ in tla-en. */
        asm("" : "+l"(x));
        right->x = x + 0xc0000;
      }
    }
    Func_02000958();
  }
}
