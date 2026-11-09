#include "types.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"

void Func_020021b4(void);
void Func_02002bcc(void);
void Func_02003c28(void);

void Func_02002a5c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;
  s32 angle;

  Func_02003c28();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(temporary): the angle named before the call, so it is built
   * before the two coordinates; as the literal argument FACING_NORTH, 4 of
   * 102 instructions differ in tla-en. */
  angle = FACING_NORTH;
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      gMainRoutineWork.currentOwner, PIXELS(248), PIXELS(136), angle);
  Map_CopyCellPictureIndicesRect(65, 64, 15, 8, 1, 1);
  GameFlag_SetBit(0x245);
  Sound_Play(161);
  Func_020021b4();
  Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(9, 0x100, 20);
  Func_02002bcc();
  Event_Wait(20);
  leader->angle = FACING_SOUTH;
  leader->motionFlags = 0;
  Actor_SetSpritePriority(9, 3);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  Event_Wait(16);
  for (i = 0; i < 40; i++) {
    if (i == 5)
      Sound_Play(204);
    leader->scaleX -= 0x400;
    leader->scaleY -= 0x300;
    leader->y -= 0x9999;
    wait(1);
  }
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(23);
}
