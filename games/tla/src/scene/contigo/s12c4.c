#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_0200406c(void);
void Func_02004074(void);
void Func_0200423c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

static __inline__ void
Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Func_0200423c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200423c(a, b, c, d, e, f, g);
}

void Func_020012c4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_0200423c_(16, 37, 1, 1, 17, 37, 0);
  if (GameFlag_Test(0xfc7) == 0) {
    Actor_SetPosition(66, PIXELS(280), PIXELS(664));
    if (leader->x >> 20 == 17 && leader->z >> 20 == 41) {
      Func_0200406c();
      Event_ResolvePendingActions(0);
      Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion_(gMainRoutineWork.currentOwner, 0,
                                                 -16);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02004074();
    }
  }
}
