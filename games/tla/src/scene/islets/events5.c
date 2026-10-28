#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldEvent_Wait();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02003e64();
void Func_02003f7c();
void Func_02003fa4();
void Func_02003ffc();
void Actor_SetSingleChildFlags();
struct Actor *Actor_GetById();
void Actor_SetModeById();
void Func_02003f84();

void Func_02003620(void) {
  s32 v5;
  struct Actor *object0;

  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  Call1((void (*)())Func_02003ffc, 0x226);
  v5 = &gSequenceParentWork;
  v5 = (v5 + 0x214);
  object0 = Actor_GetById(*(s32 *)((u8 *)v5 + 0));
  Call2((void (*)())Actor_SetSingleChildFlags, (s32)object0, 0);
  Call2((void (*)())Actor_SetModeById, *(s32 *)((u8 *)v5 + 0), 18);
  Call1((void (*)())FieldEvent_Wait, 40);
  *(s32 *)((u8 *)((s32)Ram_HeapSlots->eventWork + 0x1ac) + 0) = 0x209;
  ((void (*)())Event_FinishScreenTransition)();
  ((void (*)())Event_WaitTransitionFrames)();
  Value1((s32 (*)())Func_02003f7c, 3);
  return;
}

void Func_0200367c(void) {
  ((void (*)())Func_02003f84)();
  return;
}
