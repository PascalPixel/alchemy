#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Func_0200406c(void);
void Func_02004074(void);
void Func_020040d4(s32 id, s32 x, s32 z);
void Func_0200417c(s32 id, s32 a);
void Func_02004204(s32 a);
void Func_0200423c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return GameFlag_Test(flag);
}

static __inline__ void
ObjectMotion_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Func_020040d4_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020040d4(id, x, z);
}

static __inline__ void Func_0200417c_(s32 id, s32 a) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200417c(id, a);
}

static __inline__ void Func_02004204_(s32 a) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02004204(a);
}

static __inline__ void Func_0200423c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200423c(a, b, c, d, e, f, g);
}

void Func_020012c4(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

  Func_0200423c_(16, 37, 1, 1, 17, 37, 0);
  if (GameFlag_Test_(0xfc7) == 0) {
    Func_020040d4_(66, 0x1180000, 0x2980000);
    if (leader->x >> 20 == 17 && leader->z >> 20 == 41) {
      Func_0200406c();
      Func_02004204_(0);
      Func_0200417c_(gPartyState.currentOwner, 0x102);
      ObjectMotion_OffsetPositionAndResetMotion_(gPartyState.currentOwner, 0,
                                                 -16);
      leader->velocityY = 0x60000;
      ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
      Func_02004074();
    }
  }
}
