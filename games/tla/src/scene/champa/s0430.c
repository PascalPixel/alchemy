#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Actor_SetActionCallback(s32 *object, s32 action);

void Func_02000430(s32 actor) {
  s32 *object = ((s32 *)Actor_Get(actor));

  object[7] = 0x10000;
  object[6] = 0x10000;
  FieldEvent_SetMessageId((s32)MsgChampa07);
  Event_ShowMessage(actor, 0);
  Actor_FaceDirectionAndWait(actor, FACING_EAST);
  Actor_SetActionCallback(object, 8);
}

void Func_02000468(s32 actor) {
  s32 msg = (s32)&MsgChampa29;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}
