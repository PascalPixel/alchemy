#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020005c4();
void Func_020005cc();
void Func_020004dc(void);

void Func_020004b0(void) {
  s32 result0;

  result0 = Event_GetFlag(0xa39);
  if (result0 != 0)
    goto L_020084c6;
  Func_020004dc();
  goto L_020084d4;
L_020084c6:
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors22);
  Event_ShowMessage(18, 0);
L_020084d4:
  return;
}

void Func_020004dc(void) {
  s32 result0;

  Func_020005c4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors19);
  Actor_ShowEmote(18, 0x100, 60);
  Actor_FaceActor(18, gMainRoutineWork.currentOwner, 0);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 60);
  Actor_SetAnimation(18, 4);
  FieldEvent_Wait(15);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(18, 0x108, 60);
  Event_OpenMessage(18, 0);
  result0 =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (result0 != 0)
    goto L_02008568;
  Event_ShowMessage(18, 0);
  Event_SetFlag(0x340);
  goto L_02008582;
L_02008568:
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Event_ShowMessage(18, 0);
L_02008582:
  Event_SetFlag(0xa39);
  Event_SetFlag(0x341);
  Func_020005cc();
  return;
}
