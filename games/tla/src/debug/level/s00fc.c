#include "types.h"
#include "event_inline.h"

extern const u8 MsgIsaac2[];
void Func_020006e4(void);
void Func_020006ec(void);
void Func_0200068c(s32 index, s32 flags);

void Func_020000fc(s32 index) {
  Func_020006e4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(index + (s32)MsgIsaac2);
  Func_0200068c(index, 1);
  Event_ShowMessageAndWait(index, 0);
  if (index == 1)
    Event_ShowMessageAndWait(0, 0);
  Func_020006ec();
}
