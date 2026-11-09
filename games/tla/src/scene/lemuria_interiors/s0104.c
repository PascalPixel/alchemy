#include "types.h"
#include "event_inline.h"

extern u8 MsgLemuriaInteriors06[];
extern u8 MsgLemuriaInteriors09[];
extern u8 MsgLemuriaInteriors12[];

void Func_02000104(s32 unused, s32 id) {
  if (GameFlag_Test(0x966)) {
    Event_SetMessageId((s32)MsgLemuriaInteriors12);
    Event_ShowMessageAndWait(id, 0);
  } else if (GameFlag_Test(0x965)) {
    Event_SetMessageId((s32)MsgLemuriaInteriors09);
    Event_ShowMessageAndWait(id, 0);
  } else {
    Event_SetMessageId((s32)MsgLemuriaInteriors06);
    Event_ShowMessageAndWait(id, 0);
  }
}
