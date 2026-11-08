#include "types.h"
#include "call.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);

void Func_02001c44(s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->scaleX = SPLIT_CONSTANT(0xb333);
  object->scaleY = SPLIT_CONSTANT(0xb333);
  object->radius = 8;
}
