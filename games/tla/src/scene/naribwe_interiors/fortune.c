#include "types.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020010e0(void);
void Func_020010e8(void);
void Func_02000390(void);

void Func_02000304(void) {
  u8 *message;

  Func_020010e0();
  FieldEvent_ResolvePendingActions(0);
  message = MsgIAmA;
  FieldEvent_SetMessageId((s32)message);
  Event_OpenMessage(10, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_SetMessageId((s32)(message + 2));
    Event_OpenMessage(10, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
      if ((u32)gMainRoutineWork.coins > 19) {
        Func_02000390();
        Func_020010e8();
      } else {
        FieldEvent_SetMessageId((s32)(message + 4));
        Event_ShowMessage(10, 0);
        Func_020010e8();
      }
      return;
    }
  }
  FieldEvent_SetMessageId((s32)MsgThenStandBack);
  Event_ShowMessage(10, 0);
  Func_020010e8();
}
