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
extern u8 MsgYallamInteriors36[];
extern u8 MsgYallamInteriors32[];

s32 Func_0200014c(void) {
  s32 t2;

  Func_020009b0();
  Event_ResolvePendingActions(0);
  t2 = GameFlag_Test(0x301);
  if (t2 != 0) {
    Event_SetMessageId((s32)MsgYallamInteriors36);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriors32);
  }
  Event_ShowMessageAndWait(16, 0);
  Func_020009b8();
  return 0;
}
