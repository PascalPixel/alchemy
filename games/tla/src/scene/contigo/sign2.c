#include "types.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_0200406c(void);
void Func_02004074(void);

void Func_02001070(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigo102);
  Event_AskYesNoAndShowResponse(14, 0);
  Func_02004074();
}
