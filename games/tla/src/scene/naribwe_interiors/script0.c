#include "types.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020010e0(void);
void Func_020010e8(void);

s32 Func_02000054(void) {
  Func_020010e0();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgTheItemsThat);
  Event_ShowMessageAndWait(10, 0);
  Func_020010e8();
  return 0;
}

s32 Func_0200007c(void) {
  Func_020010e0();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgItemsPlacedBefore);
  Event_ShowMessageAndWait(10, 0);
  Func_020010e8();
  return 0;
}
