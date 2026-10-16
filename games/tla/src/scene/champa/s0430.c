#include "types.h"
#include "text/msg_ids.h"

s32 *Object_GetById(s32 id);
void Object_SetActionCallback(s32 *object, s32 action);
void Battle_WaitMode0(s32 frames);
void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_02002adc(s32 message);
void Func_02002af4(s32 id, s32 flags);
void Func_02002b04(s32 id, s32 flags);
s32 Func_02002bb4(void);

void Func_02000430(s32 actor) {
  s32 *object = Object_GetById(actor);

  object[7] = 0x10000;
  object[6] = 0x10000;
  Func_02002adc((s32)MsgChampa07);
  Func_02002af4(actor, 0);
  Func_02002b04(actor, 0);
  Object_SetActionCallback(object, 8);
}

void Func_02000468(s32 actor) {
  s32 msg = (s32)&MsgChampa29;

  Func_02002adc(msg);
  UiText_OpenMessageAtObject(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02002bb4(), 0) == 0) {
    Battle_WaitMode0(10);
    Func_02002adc(msg + 1);
  } else {
    Battle_WaitMode0(20);
    Func_02002adc(msg + 2);
  }
  Func_02002af4(actor, 0);
}
