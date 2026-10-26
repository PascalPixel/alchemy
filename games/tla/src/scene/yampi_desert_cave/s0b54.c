#include "yampi_desert_cave.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02000c44(s32 id, u32 column, s32 row, s32 layer);

/// Walks a list of three-halfword entries (object id, column, row) ended by
/// 0xffff, clearing each object's collision bit and child flags before passing
/// the entry to Func_02000c44. A struct of three halfwords would be padded to
/// 8 bytes, so the list is read as u16 triples. The twin of overlay 66a's
/// Func_02001230.
void Func_02000b54(const u16 *list) {
  const u16 *entry;

  if (list[0] == 0xffff)
    return;
  entry = list;
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
