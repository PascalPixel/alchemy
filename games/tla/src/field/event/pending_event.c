#include "types.h"
#include "event_work.h"
#include "text/msg_ids.h"

s32 GameFlag_Test(s32 flag);
void FieldMessage_ShowFar(s32 message, s32 position);

s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode,
                                 s32 target) {
  if (event != NULL) {
    if (event->action != 0) {
      if (event->action < 0x10000) {
        Event_Start();
        Event_SetMessageId(event->action);
        Event_ShowMessageAndWait(target, 0);
        Event_End();
      } else {
        ((void (*)(s32, s32))event->action)(mode, target);
      }
    }
    if (GameFlag_Test(0x142)) {
      Event_Start();
      FieldMessage_ShowFar((s32)MsgNothingHappens, 1);
      Event_End();
    }
  }
  return 0;
}
