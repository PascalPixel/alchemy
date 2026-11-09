#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02003e64();
void Actor_SetSingleChildFlags();
void Func_02003f84();

void Func_02003620(void) {
  s32 v5;
  struct Actor *object0;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x226);
  v5 = &gMainRoutineWork;
  v5 = (v5 + 0x214);
  object0 = Actor_Get(*(s32 *)((u8 *)v5 + 0));
  Actor_SetSingleChildFlags((s32)object0, 0);
  Actor_SetAnimation(*(s32 *)((u8 *)v5 + 0), 18);
  FieldEvent_Wait(40);
  *(s32 *)((u8 *)((s32)Ram_HeapSlots->eventWork + 0x1ac) + 0) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(3);
  return;
}

void Func_0200367c(void) {
  Func_02003f84();
  return;
}
