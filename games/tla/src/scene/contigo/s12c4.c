#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Func_0200406c(void);
void Func_02004074(void);
void Func_020040d4(s32 id, s32 x, s32 z);
void Func_0200417c(s32 id, s32 a);
void Func_02004204(s32 a);
void Func_0200423c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void
Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Func_020040d4_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020040d4(id, x, z);
}

static __inline__ void Func_0200417c_(s32 id, s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200417c(id, a);
}

static __inline__ void Func_02004204_(s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004204(a);
}

static __inline__ void Func_0200423c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200423c(a, b, c, d, e, f, g);
}

void Func_020012c4(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

  Func_0200423c_(16, 37, 1, 1, 17, 37, 0);
  if (GameFlag_Test_(0xfc7) == 0) {
    Func_020040d4_(66, 0x1180000, 0x2980000);
    if (leader->x >> 20 == 17 && leader->z >> 20 == 41) {
      Func_0200406c();
      Func_02004204_(0);
      Func_0200417c_(gSequenceParentWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion_(gSequenceParentWork.currentOwner, 0,
                                                 -16);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gSequenceParentWork.currentOwner);
      Func_02004074();
    }
  }
}
