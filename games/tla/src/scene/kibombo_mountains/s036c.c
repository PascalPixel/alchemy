#include "types.h"
#include "actor.h"
#include "flag_call.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02001084(void);
void Func_0200108c(void);

void Func_0200036c(void) {
  /* FAKEMATCH: the placements go through call.h's Call3 and the flags through
   * flag_call.h, which build each constant again at each call (direct calls
   * share 764 and 380 in saved registers, differ 36). */
  Func_02001084();
  Event_ResolvePendingActions(0);
  Actor_SetDestination(17, 764, 384);
  Actor_MoveToAndWait(16, 746, 380);
  Actor_GetById(17)->velocityY = PIXELS(4);
  Actor_WaitForMoveById(17);
  Actor_GetById(17)->priorityFlags |= 2;
  if (!Flag_IsSet(0x8ff) && !Flag_IsSet(0x201)) {
    Actor_Stop(10);
    Actor_MoveToAndWait(10, 764, 388);
    Actor_SetDestination(17, 764, 376);
    Actor_WalkToAndWait(10, 764, 380);
    Actor_SetModeById(10, 4);
    Func_080d489c(10, 257);
    Flag_Set(0x202);
  }
  Func_0200108c();
}
