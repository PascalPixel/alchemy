#include "types.h"
#include "actor.h"

s32 Func_080da0b0(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y, s32 to_z,
                  s32 mode);

void Func_080da060(s32 index, s32 from, s32 to, s32 mode) {
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL)
    Func_080da0b0(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000, b->z,
                  mode);
}
