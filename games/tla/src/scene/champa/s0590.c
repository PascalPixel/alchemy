#include "types.h"
#include "text/msg_ids.h"

void FieldEvent_WaitFrames(s32 frames);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_02002adc(s32 message);
void Func_02002af4(s32 id, s32 flags);
s32 Func_02002bb4(void);

void Func_02000590(s32 actor) {
  s32 msg = (s32)&MsgChampa11;

  Func_02002adc(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02002bb4(), 0) == 0) {
    FieldEvent_WaitFrames(10);
    Func_02002adc(msg + 1);
  } else {
    FieldEvent_WaitFrames(20);
    Func_02002adc(msg + 2);
  }
  Func_02002af4(actor, 0);
}
