#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02000644();
s32 Func_0200462c();
s32 Func_02004464();
s32 Func_0200446c();
extern u8 MsgProx139[];
extern u8 MsgProx60[];

void Func_02000714(void) {
  s32 t0;
  s32 t4;

  t0 = Func_02000644();
  if (t0 != 0) {
    Func_0200462c(8);
  } else {
    Func_02004464();
    Event_ResolvePendingActions(0);
    t4 = GameFlag_Test(0xa3d);
    if (t4 != 0) {
      Event_SetMessageId((s32)MsgProx139);
    } else {
      Event_SetMessageId((s32)MsgProx60);
    }
    Event_ShowMessage(8, 0);
    Func_0200446c();
  }
}
