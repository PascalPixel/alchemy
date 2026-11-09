#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02000644();
s32 Func_02004634();
s32 Func_02004464();
s32 Func_0200446c();
extern u8 MsgProx135[];
extern u8 MsgProx56[];

void Func_02000674(void) {
  s32 t0;
  s32 t4;

  t0 = Func_02000644();
  if (t0 != 0) {
    Func_02004634(0xe, 0x16);
  } else {
    Func_02004464();
    Event_ResolvePendingActions(0);
    t4 = GameFlag_Test(0xa3d);
    if (t4 != 0) {
      Event_SetMessageId((s32)MsgProx135);
    } else {
      Event_SetMessageId((s32)MsgProx56);
    }
    Event_ShowMessage(22, 0);
    Func_0200446c();
  }
}
