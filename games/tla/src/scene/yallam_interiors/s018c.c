#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
void Func_020009b0(void);
void Func_020009b8(void);
extern u8 MsgYallamInteriors22[];

s32 Func_0200018c(void) {
  Func_020009b0();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriors22);
  Event_ShowMessageAndWait(10, 0);
  Func_020009b8();
  return 0;
}
