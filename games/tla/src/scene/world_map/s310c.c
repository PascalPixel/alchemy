#include "types.h"
#include "call.h"

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_02004c20(void);
void Func_02004d40(s32 value);
void Func_02004db8(s32 value);

void Func_0200310c(void) {
  Func_02004c20();
  Func_02004db8(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3((void (*)())ObjectMotion_SetPositionAndCommit, 8, 10720, 9075);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004d40(66);
}
