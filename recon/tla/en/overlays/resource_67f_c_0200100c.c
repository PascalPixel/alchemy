// Near miss, differ 6 of 40: written with goto the loop keeps the listing's
// shape (one test at the bottom, 0xffff rebuilt on every pass; as a while loop
// the test is copied and the constant hoisted to r8). Left: the register that
// holds the offset 4 and where object is copied for the flag byte. An empty
// asm after motionFlags = 0 places the copy (4 left); the offset 4 is the
// ldrsh's reload scratch, which reload gives r2 where the native has r3, and
// no plain spelling of the halfword (an offset or index local, a byte
// pointer, an s16 view) moves it.
#include "../../../../games/tla/src/scene/izumo_ruins/izumo_ruins.h"

void Func_0200100c(u16 *list) {
  struct ObjectRuntime *object;
  s32 flag;

  goto test;
next:
  object = Object_GetById(list[0]);
  flag = (s16)list[2];
  ObjectDispatch_SetSingleChildFlags(object, 0);
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
