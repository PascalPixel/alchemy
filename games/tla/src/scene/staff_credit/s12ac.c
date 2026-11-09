#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_02001274(struct Actor *object, s32 mode);
s32 Func_02002004(s32 *position);
s32 Func_020020a4(s32 mode, s32 value);

void Func_020012ac(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 i;

  Event_PlaySound(212);
  Func_02001274(object, 1);
  for (i = 0; i < 20; i++) {
    s32 hit = Func_02002004(&object->x);

    if (hit != 0 && Func_020020a4(2, hit) != 0)
      break;
    System_Wait(1);
  }
  Func_02001274(object, 2);
}
