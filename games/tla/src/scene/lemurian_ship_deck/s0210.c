#include "types.h"
#include "event_inline.h"

void Func_02001f5c(void);
void Func_02001f64(void);
void Func_02001f34(s32 a);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020001cc(void);

void Func_02000210(void) {
  Func_02001f5c();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  FieldTask_AddFrame(Func_020001cc, 0x480);
  Event_WaitTransitionFrames();
  FieldEvent_Wait(60);
  Func_02001f34(13);
  FieldEvent_Wait(60);
  FieldEvent_Wait(60);
  Func_02001f34(14);
  Func_02001f64();
}
