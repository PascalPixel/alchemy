#include "local.h"

/// Walks a list of three-halfword entries (object id, kind, flag) ended by
/// 0xffff, clearing each object's collision bit and child flags before passing
/// the entry to Func_02001320. The twin of overlay 662's Func_02000b54.
void Func_02001230(const u16 *list) {
  const u16 *entry;

  if (list[0] == 0xffff)
    return;
  entry = list;
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
