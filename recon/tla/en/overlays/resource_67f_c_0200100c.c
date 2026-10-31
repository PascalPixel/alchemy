// Near miss, differ 6 of 40 (tla-en, 2026-10-31): the register that holds the
// offset 4 and where object is copied for the flag byte.
// Written with goto the loop keeps the listing's shape (one test at the
// bottom, 0xffff rebuilt on every pass; as a while, for or for (;;) with break
// the test is copied and the constant hoisted to r8; a do-while entered at
// its test compiles as the goto). An empty asm after motionFlags = 0 places
// the copy (4 left), and so does a once-only block around it (4 left).
// The offset 4 is the ldrsh's reload scratch. Reload hands its spill
// registers {r2, r3} out in turn from r2, and the ldrsh is the first reload
// in insn order, so it takes r2; the test's 0xffff is reloaded later into r2
// as well. The native's r3 is what it gets when the test's constant needs no
// spill register (a pinned limit shows it) or when a reload comes before it
// (the while loop's copied test shows it). No plain spelling of the halfword
// (an offset or index local, a byte pointer, an s16 view, an s16 or u16
// flag, the IzumoRuinsPlacement view, whose padded stride is 8) or of the
// test ((s32), (u32), 65535u, (u16)-1, *list + 1 != 0x10000) moves it.
#include "../../../../games/tla/src/scene/izumo_ruins/izumo_ruins.h"

void Func_0200100c(u16 *list) {
  struct Actor *object;
  s32 flag;

  goto test;
next:
  object = Actor_GetById(list[0]);
  flag = (s16)list[2];
  Actor_SetSingleChildFlags(object, 0);
  Func_02001428(object, GameFlag_Test((u16)flag) + 1);
  object->motionFlags = 0;
  object->collisionFlags &= 0xfe;
  object->action = flag;
  list += 3;
  Func_02000f30(object);
test:
  if (list[0] != 0xffff)
    goto next;
}
