#include "local.h"

void Func_02002e04(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->animation, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02003cc4(object, 0);
    Func_02003cc4(object, 2);
  }
}

void Func_02002e34(s32 entrance) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 i;

  Func_02003d7c();
  Func_02003eac(0);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  leader->motionFlags = 0;
  leader->priorityFlags |= 0x20;
  Func_02003ef4(215);
  Func_02003cc4(leader, 18);
  Func_02003ef4(612);
  for (i = 0; i < 60; i++) {
    s32 angle;

    if (i == 30)
      Event_FinishScreenTransition();
    leader->y -= 0x4000;
    angle = leader->angle;
    leader->angle = angle + 0x1000;
    if ((i & 7) == 0)
      Func_02002bd4(Actor_GetById(15));
    wait(1);
  }
  Func_02003d84();
  Func_02003e5c(entrance);
}

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                   s32 *params);

/// On odd frames, spawns a drifting effect above the object, swaying with the
/// frame count; Func_02002030 of Aqua Rock is the same function.
s32 Func_02002ed4(struct Actor *object) {
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
