#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_02001274(struct Actor *object, s32 mode);

void Func_020012f8(void) {
  Func_02001274(Actor_Get(FieldParty_GetLeaderActor()), 0);
}
