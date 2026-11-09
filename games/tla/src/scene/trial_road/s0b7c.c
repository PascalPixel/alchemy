#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "event_inline.h"

void Func_02005880(void);
void Func_02005888(void);
void Func_0200095c(s32 id);
void Func_02000924(s32 id);

void Func_02000b7c(s32 id, s32 other, s32 flag, s32 side) {
  struct Actor *object = Actor_GetById(id);
  struct Actor *partner = Actor_GetById(other);
  s32 t;

  if (OBJECT_BYTE(object, 0x63) != 0)
    return;
  Func_02005880();
  Event_ResolvePendingActions(0);
  partner->scaleY = 0x2000;
  wait(1);
  Func_0200095c(other);
  t = 0x2000;
grow:
  Func_0200070c(id, (0x8000 - t) / 2048);
  object->scaleY = 0x10000 - t;
  partner->scaleY = t;
  wait(1);
  t += 0x2000;
  if (t <= 0x10000)
    goto grow;
  object->scaleY = 0x10000;
  Func_02000924(id);
  Sound_Play(138);
  if (side != 0)
    GameFlag_SetBit(flag);
  else
    GameFlag_ClearBit(flag);
  Func_02005888();
}
