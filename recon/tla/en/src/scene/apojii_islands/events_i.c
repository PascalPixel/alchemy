#include "../../../../../../games/tla/src/scene/apojii_islands/local.h"

void Func_02001308(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->animation, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02001c6c(object, 0);
    Func_02001c6c(object, 2);
  }
}

void Func_02001338(s32 entrance) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 i;

  Func_02001cc4();
  Func_02001dcc(0);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  leader->motionFlags = 0;
  leader->priorityFlags |= 0x20;
  Func_02001e2c(215);
  Func_02001c6c(leader, 18);
  Func_02001e2c(612);
  for (i = 0; i < 60; i++) {
    s32 angle;

    if (i == 30)
      Event_FinishScreenTransition();
    leader->y -= 0x4000;
    angle = leader->angle;
    leader->angle = angle + 0x1000;
    if ((i & 7) == 0)
      Func_020010d8(Actor_GetById(15));
    wait(1);
  }
  Func_02001ccc();
  Func_02001d94(entrance);
}

/// Every other frame, emits particle 7 (5 on every other such frame) from the
/// object, 26 pixels up, swaying with the frame count and drifting by a random
/// amount.
s32 Func_020013d8(struct Actor *object) {
  s32 params[10];

  if (gFrameCount & 1) {
    s32 drift;
    s32 x;
    s32 y;

    params[1] = 7;
    if ((gFrameCount & 2) == 0)
      params[1] = 5;
    params[2] = SPLIT_CONSTANT(0xcccc);
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
