#include "jupiter_room_m.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_020047d4(void);
void Func_020047dc(void);
void Func_0200494c(s32 a);
void Func_020049c4(s32 cue);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 d);
void Func_020048f4(void);
void Func_0200474c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Camera_FollowActor(s32 id, s32 flag);

static __inline__ void Func_0200474c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (both calls direct: differ 12 of 61). */
  Func_0200474c(a, b, c, d, e, f);
}

void Func_020008c8(void) {
  if (GameFlag_Test(0xa1f) != 0)
    return;
  GameFlag_ClearBit(0x202);
  Func_020047d4();
  Func_0200494c(0);
  Func_020049c4(125);
  Camera_MoveWithinMapBounds(776 << 16, -1, 208 << 16, 1);
  Func_020048f4();
  FieldEvent_WaitFrames(6);
  Func_0200474c_(49, 103, 4, 3, 49, 76);
  FieldEvent_WaitFrames(6);
  Func_020049c4(133);
  Func_0200474c_(49, 100, 4, 3, 49, 76);
  FieldEvent_WaitFrames(20);
  Camera_FollowActor(gPartyState.currentOwner, 1);
  Func_020048f4();
  Func_020047dc();
}
