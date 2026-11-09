#include "jupiter_lighthouse_room_m.h"
#include "scene_tables.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_020047d4(void);
void Func_020047dc(void);
void Event_ResolvePendingActions(s32 a);
void Sound_Play(s32 cue);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 d);
void Func_020048f4(void);
void Func_0802b450(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Camera_FollowActor(s32 id, s32 flag);

static __inline__ void Func_0200474c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (both calls direct: differ 12 of 61). */
  Func_0802b450(a, b, c, d, e, f);
}

void Func_020008c8(void) {
  if (GameFlag_Test(0xa1f) != 0)
    return;
  GameFlag_ClearBit(0x202);
  Func_020047d4();
  Event_ResolvePendingActions(0);
  Sound_Play(125);
  Camera_MoveWithinMapBounds(PIXELS(776), -1, PIXELS(208), 1);
  Func_020048f4();
  Event_Wait(6);
  Func_0200474c_(49, 103, 4, 3, 49, 76);
  Event_Wait(6);
  Sound_Play(133);
  Func_0802b450(49, 100, 4, 3, 49, 76);
  Event_Wait(20);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020048f4();
  Func_020047dc();
}
