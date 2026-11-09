#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

struct EventNote {
  u8 unk000[0x15a];
  u16 unk15a;
};

void Func_020042dc(void);
void Func_020042e4(void);

void Func_02000e50(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar07, 1);
  ((struct EventNote *)work)->unk15a = 1;
  Func_020042e4();
}
