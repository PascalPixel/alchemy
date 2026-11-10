#include "local.h"

void Func_02002998(void);
void Func_020029a0(void);
void Event_ResolvePendingActions(s32 a);
void Sound_Play(s32 cue);
void Import_EventSetExitRequest(s32 a);
void Func_02001d30(struct Actor *object);
void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                   s32 *params);

void Func_02001f90(s32 arg0) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 i;
  s32 angle;

  Func_02002998();
  Event_ResolvePendingActions(0);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  object->motionFlags = 0;
  object->priorityFlags |= 0x20;
  Sound_Play(215);
  Func_02002910(object, 18);
  Sound_Play(612);
  for (i = 0; i < 60; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    object->y -= 0x4000;
    angle = object->angle;
    angle += 0x1000;
    object->angle = angle;
    if ((i & 7) == 0)
      Func_02001d30(Actor_GetById(15));
    wait(1);
  }
  Func_020029a0();
  Import_EventSetExitRequest(arg0);
}

/// Every other frame, emits particle 7 (5 on every other such frame) from the
/// object, 26 pixels up, swaying with the frame count and drifting by a random
/// amount.
s32 Func_02002030(struct Actor *object) {
  s32 params[10];

  if (gFrameCount & 1) {
    s32 drift;
    s32 x;
    s32 y;

    params[1] = 7;
    if ((gFrameCount & 2) == 0)
      params[1] = 5;
    params[2] = 0xcccc;
    params[3] = 0xcccc;
    params[0] = 0;
    drift = ((u32)(Random16Far() << 3) >> 16) * 0x3333;
    x = object->x;
    y = object->y;
    Func_0200015c(x + ((8 - (gFrameCount & 15)) << 16), y + 0x1a0000, object->z,
                  0, -drift, 0, 0xb0000, params);
  }
  return 0;
}
