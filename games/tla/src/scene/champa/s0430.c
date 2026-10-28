#include "types.h"
#include "text/msg_ids.h"

s32 *Actor_GetById(s32 id);
void Actor_SetActionCallback(s32 *object, s32 action);
void FieldEvent_WaitFrames(s32 frames);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_02002adc(s32 message);
void Func_02002af4(s32 id, s32 flags);
void Func_02002b04(s32 id, s32 flags);
s32 Func_02002bb4(void);

void Func_02000430(s32 actor) {
  s32 *object = Actor_GetById(actor);

  object[7] = 0x10000;
  object[6] = 0x10000;
  Func_02002adc((s32)MsgChampa07);
  Func_02002af4(actor, 0);
  Func_02002b04(actor, 0);
  Actor_SetActionCallback(object, 8);
}

void Func_02000468(s32 actor) {
  s32 msg = (s32)&MsgChampa29;

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
