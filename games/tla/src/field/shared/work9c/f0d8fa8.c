#include "types.h"
#include "object_runtime.h"

s32 Func_080d8ff8(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y, s32 to_z,
                  s32 mode);

void Func_080d8fa8(s32 index, s32 from, s32 to, s32 mode) {
  struct ObjectRuntime *a = ObjectTable_Get(from);
  struct ObjectRuntime *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL)
    Func_080d8ff8(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000, b->z,
                  mode);
}
