#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02002488(void);
void Func_02002490(void);
void Func_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Yallam_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                   s32 a5, s32 a6) {
  Func_02002598(a0, a1, a2, a3, a4, a5, a6);
}

void Func_0200038c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Yallam_02002598(26, 95, 2, 1, 26, 76, 2);
  if (GameFlag_Test(0xf62) == 0) {
    Actor_SetPosition(66, PIXELS(424), PIXELS(232));
    if (leader->x >> 20 == 26 && leader->z >> 20 == 14) {
      Func_02002488();
      Event_ResolvePendingActions(0);
      Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02002490();
    }
  }
}

void Func_0200041c(void) { Yallam_02002598(67, 25, 3, 3, 106, 20, 1); }

void Func_0200043c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Yallam_02002598(67, 25, 3, 3, 106, 20, 1);
  if (GameFlag_Test(0xf60) == 0) {
    Actor_SetPosition(64, PIXELS(680), PIXELS(344));
    if (leader->x >> 20 == 42 && leader->z >> 20 == 21) {
      Func_02002488();
      Event_ResolvePendingActions(0);
      Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -16);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02002490();
    }
  }
}
