#include "types.h"

void Func_02001f5c(void);
void Func_02001f64(void);
void Func_020020ac(s32 a);
void Func_02001f34(s32 a);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_WaitTicks(s32 ticks);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_020001cc(void);

void Func_02000210(void) {
  Func_02001f5c();
  Func_020020ac(0);
  Event_StartScreenTransition();
  Scheduler_AddOrUpdateCallback(Func_020001cc, 0x480);
  Event_WaitTransitionFrames();
  FieldEvent_WaitTicks(60);
  Func_02001f34(13);
  FieldEvent_WaitTicks(60);
  FieldEvent_WaitTicks(60);
  Func_02001f34(14);
  Func_02001f64();
}
