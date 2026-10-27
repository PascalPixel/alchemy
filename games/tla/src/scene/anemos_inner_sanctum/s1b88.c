#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02003398();
s32 Func_02003488();
s32 Func_02003460();
s32 Func_02003458();
s32 Func_02003468();
s32 FieldEvent_WaitTicks();
s32 Func_02003388();
s32 GameFlag_SetBit();
s32 Func_020033a0();
extern u8 MsgSummonInheritedEarthDarkness[];

void Func_02001b88(void) {
  Func_02003398();
  Call1(Func_02003488, 0);
  Call2(Func_02003460, 0x10000, 0);
  Call2(Func_02003458, 0x10003, 1);
  Call1(Func_02003468, 0x3c);
  Call1(FieldEvent_WaitTicks, 0x3c);
  Call3(Func_02003388, (s32)MsgSummonInheritedEarthDarkness, 0, 6);
  Call2(Func_02003458, 0x10000, 0);
  Call1(Func_02003468, 0x3c);
  Call1(FieldEvent_WaitTicks, 0x3c);
  Call1(GameFlag_SetBit, 0xa8c);
  Func_020033a0();
}
