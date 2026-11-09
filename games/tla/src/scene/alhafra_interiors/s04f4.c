#include "alhafra_interiors.h"
#include "event_inline.h"

void Func_020004f4(s32 actor) {
  s32 msg = (s32)MsgAlhafraInteriors74;

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

u8 *Func_02000540(void) {
  if (Event_GetFlag(0x8a7))
    return gData_020019dc;
  if (Event_GetFlag(0x8ab))
    return gData_02001730;
  return gData_020013dc;
}
