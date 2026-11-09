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
  Event_ResolvePendingActions(0);
  message = MsgIAmA;
  Event_SetMessageId((s32)message);
  FieldMessage_ShowAtActor(10, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_SetMessageId((s32)(message + 2));
    FieldMessage_ShowAtActor(10, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      if ((u32)gMainRoutineWork.coins > 19) {
        Func_02000390();
        Func_020010e8();
      } else {
        Event_SetMessageId((s32)(message + 4));
        Event_ShowMessageAndWait(10, 0);
        Func_020010e8();
      }
      return;
    }
  }
  Event_SetMessageId((s32)MsgThenStandBack);
  Event_ShowMessageAndWait(10, 0);
  Func_020010e8();
}
