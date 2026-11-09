#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_02000a98(s32 a, s32 b);

void Func_02000ef0(struct Actor *object) {
  object->x = Actor_GetById(8)->x;
}
