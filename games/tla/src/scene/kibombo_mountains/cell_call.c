#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_0200114c(void);
void Func_0200103c(struct Actor *object, s32 value);
void Func_02000da8(s32 value);
void Func_02000e28(s32 value);

void Func_020004f4(s32 id, s32 a, s32 b) {
  struct Actor *object = Actor_Get(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  MapGrid_CopyCellAttributes(a, b, 1, 1, x, z);
}
