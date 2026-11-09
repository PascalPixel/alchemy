#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 gData_0200466c[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200406c(void);

void Func_02001c5c(s32 a, s32 b, s32 c, s32 d) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  Sound_Play(188);
  {
    s32 width = 1;
    s32 height = 2;

    Map_CopyCellPictureIndicesRect(a, b, c, d, width, height);
    Map_CopyCellPictureIndicesRect(a, b + 2, c + 1, d, width, height);
  }
  Event_Wait(20);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_0200466c);
  wait(12);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
