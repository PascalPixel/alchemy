// Draft, differ 10 of 51: the copy of overlay 662's Func_02000b54 (whose draft
// holds the notes), calling Izumo Ruins' Func_02000128 for its cell placement.
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02000128(s32 id, u32 column, s32 row, s32 layer);

void Func_02000038(const u16 *entry) {
  if (entry[0] == 0xffff)
    return;
loop:
  {
    u32 id = entry[0];
    struct ObjectRuntime *object = Object_GetById(id);
    s16 column = entry[1];
    s16 row = entry[2];

    object->collisionFlags &= ~1;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02000128(id, (u16)column, (u16)row, object->terrainId);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}
