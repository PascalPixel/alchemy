// Near miss, differ 10 of 51 (was 28), the twin of overlay 664's
// Func_02000038 and overlay 662's Func_02000b54. A goto loop rebuilds 0xffff
// at each test as the native does (a while loop hoists it into r10). Left, all
// scheduling: the native tests the first entry through r0 before it copies the
// pointer to r7; it steps the pointer (adds r7, #6) before the last call, where
// sched2 here prefers the call (equal priority 4, the call has more
// dependents), also with the step written before the call; and it loads the
// next entry before it finishes 0xffff.
#include "../../../../games/tla/src/scene/alhafran_cave/local.h"

void Func_02001230(const u16 *entry) {
  if (entry[0] == 0xffff)
    return;
loop: {
  u32 id = entry[0];
  struct ObjectRuntime *object = Object_GetById(id);
  s16 kind = entry[1];
  s16 flag = entry[2];

  object->collisionFlags &= ~1;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Func_02001320(id, (u16)kind, (u16)flag, object->terrainId);
  entry += 3;
}
  if (entry[0] != 0xffff)
    goto loop;
}
