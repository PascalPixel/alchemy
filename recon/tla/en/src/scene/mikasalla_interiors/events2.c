#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldEvent_Wait();
void Func_020005c4();
void Func_020005cc();
void Func_02000604();
void Func_02000614();
void Func_02000624();
void Func_0200062c();
void GameFlag_SetBit();
void GameFlag_Test();
void Event_AskYesNoWithActorAnimation();
void Actor_SetAngleToward();
void Actor_SetModeById();
void FieldMessage_ShowAtActor();
void Func_020004dc(void);

void Func_020004b0(void) {
  s32 result0;

  result0 = Value1((s32 (*)())GameFlag_Test, 0xa39);
  if (result0 != 0)
    goto L_020084c6;
  Func_020004dc();
  goto L_020084d4;
L_020084c6:
  Func_02000604((s32)MsgMikasallaInteriors22);
  Func_02000614(18, 0);
L_020084d4:
  return;
}

void Func_020004dc(void) {
  s32 result0;

  Func_020005c4();
  Func_0200062c(0);
  Func_02000604((s32)MsgMikasallaInteriors19);
  Value3((s32 (*)())Func_02000624, 18, 0x100, 60);
  Actor_SetAngleToward(18, gMainRoutineWork.currentOwner, 0);
  Func_02000614(18, 0);
  Value3((s32 (*)())Func_02000624, gMainRoutineWork.currentOwner, 0x100, 60);
  Actor_SetModeById(18, 4);
  FieldEvent_Wait(15);
  Func_02000614(18, 0);
  Value3((s32 (*)())Func_02000624, 18, 0x108, 60);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 18, 0);
  result0 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation,
                   gMainRoutineWork.currentOwner, 0);
  if (result0 != 0)
    goto L_02008568;
  Func_02000614(18, 0);
  GameFlag_SetBit(0x340);
  goto L_02008582;
L_02008568:
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Func_02000614(18, 0);
L_02008582:
  GameFlag_SetBit(0xa39);
  GameFlag_SetBit(0x341);
  Func_020005cc();
  return;
}
