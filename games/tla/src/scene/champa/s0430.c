#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Actor_SetActionCallback(s32 *object, s32 action);

void Func_02000430(s32 actor) {
  s32 *object = ((s32 *)Actor_GetById(actor));

  object[7] = 0x10000;
  object[6] = 0x10000;
  Event_SetMessageId((s32)MsgChampa07);
  Event_ShowMessageAndWait(actor, 0);
  Actor_ArmCallbackAndRefresh(actor, FACING_EAST);
  Actor_SetActionCallback(object, 8);
}

void Func_02000468(s32 actor) {
  s32 msg = (s32)&MsgChampa29;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}
