#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001010(void);

void Func_02000c8c(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Func_02001010();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
  Event_Wait(8);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(*(s16 *)(work + 340));
}
