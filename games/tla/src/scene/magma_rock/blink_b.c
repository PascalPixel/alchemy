#include "types.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"

void Func_020020d0(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    Actor_SetSpriteColor(object, 7);
  else
    Actor_SetSpriteColor(object, 0);
}
