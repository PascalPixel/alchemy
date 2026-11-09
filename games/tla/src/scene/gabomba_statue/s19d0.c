#include "types.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020056b4(struct Actor *object);
void Func_020056bc(void);
void Func_020057b4(void);
void Func_020020f0(s32 value);

void Func_020019d0(void) {
  struct Actor *actor = Actor_GetById(26);

  Func_020056b4(actor);
  Event_ResolvePendingActions(0);
  Func_080d4568()->motionFlags = 0;
  Camera_SetSpeedParameters(0x9999, 0x1333);
  FieldCamera_MoveTo(180 << 17, 0, 248 << 17, 1);
  Func_020057b4();
  Sound_Play(586);
  Func_020020f0(0);
  Event_Wait(40);
  Sound_Play(582);
  actor->motionFlags = 0;
  actor->y = PIXELS(8);
  Event_Wait(10);
  actor->y = 0;
  Event_Wait(10);
  Func_020020f0(-1);
  GameFlag_SetBit(0x8cd);
  Event_Wait(20);
  Func_020056bc();
}
