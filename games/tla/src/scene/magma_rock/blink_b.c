#include "types.h"
#include "frame.h"
#include "actor.h"

void Actor_SetPartAttribute(struct Actor *object, s32 value);

void Func_020020d0(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    Actor_SetPartAttribute(object, 7);
  else
    Actor_SetPartAttribute(object, 0);
}
