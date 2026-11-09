#include "alhafra_interiors.h"

void Func_020004f4(s32 actor) {
  s32 msg = (s32)MsgAlhafraInteriors74;

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

u8 *Func_02000540(void) {
  if (GameFlag_Test(0x8a7))
    return gData_020019dc;
  if (GameFlag_Test(0x8ab))
    return gData_02001730;
  return gData_020013dc;
}
