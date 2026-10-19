// Draft, differ 10 of 51 (was 30), the twin of overlay 664's Func_02000038
// and overlay 66a's Func_02001230: a list of three-halfword entries ended by
// 0xffff (a struct of three halfwords is padded to 8 bytes by agscc, so the
// list is u16 triples). A goto loop keeps the native's register use and
// rebuilds 0xffff at each test, as the native does (a while loop hoists the
// constant into r10, differ 30). Left, all scheduling: the native tests the
// first entry through r0 before it copies the pointer to r7; it steps the
// pointer (adds r7, #6) before the last call, where sched2 here prefers the
// call (equal priority, more dependents); and it loads the next entry before
// it finishes 0xffff. Each is the native issuing a load-related insn earlier.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02000c44(s32 id, u32 column, s32 row, s32 layer);

void Func_02000b54(const u16 *entry) {
  if (entry[0] == 0xffff)
    return;
loop: {
  u32 id = entry[0];
  struct ObjectRuntime *object = Object_GetById(id);
  s16 column = entry[1];
  s16 row = entry[2];

  object->collisionFlags &= ~1;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Func_02000c44(id, (u16)column, (u16)row, object->terrainId);
  entry += 3;
}
  if (entry[0] != 0xffff)
    goto loop;
}
