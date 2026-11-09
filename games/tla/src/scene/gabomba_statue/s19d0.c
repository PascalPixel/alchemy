#include "types.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020056b4(struct Actor *object);
void Func_020056bc(void);
void Func_020057b4(void);
void Func_020020f0(s32 value);

void Func_020019d0(void) {
  struct Actor *actor = Actor_Get(26);

  Func_020056b4(actor);
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0x9999, 0x1333);
  FieldCamera_MoveTo(180 << 17, 0, 248 << 17, 1);
  Func_020057b4();
  Event_PlaySound(586);
  Func_020020f0(0);
  FieldEvent_Wait(40);
  Event_PlaySound(582);
  actor->motionFlags = 0;
  actor->y = PIXELS(8);
  FieldEvent_Wait(10);
  actor->y = 0;
  FieldEvent_Wait(10);
  Func_020020f0(-1);
  Event_SetFlag(0x8cd);
  FieldEvent_Wait(20);
  Func_020056bc();
}
