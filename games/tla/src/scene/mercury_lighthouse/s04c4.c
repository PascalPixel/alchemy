#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern u32 gData_020007e4;

struct Actor;
void Func_020001b0(void);
void Func_02000568(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

s32 Func_020004c4(void) {
  if (gMainRoutineWork.entrance == 12) {
    Func_02000568();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Sound_Play(201);
    BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
    BattleEffect_StartBufferInterpolation(20);
    wait(20);
    Import_EventSetExitRequest(9);
  } else {
    Func_020001b0();
  }
  return 0;
}
