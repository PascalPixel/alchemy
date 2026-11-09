#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_02003bb8(struct Actor *object, s32 mode);
s32 Func_02004b90(s32 *position);
s32 Func_02004e20(s32 a, s32 b);

void Func_02003bf0(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 i;

  Event_PlaySound(212);
  Func_02003bb8(object, 1);
  for (i = 0; i < 20; i++) {
    s32 hit = Func_02004b90(&object->x);

    if (hit != 0 && Func_02004e20(2, hit) != 0)
      break;
    System_Wait(1);
  }
  Func_02003bb8(object, 2);
}
