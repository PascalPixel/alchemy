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
s32 FieldEvent_Wait();
s32 Func_02003388();
s32 GameFlag_SetBit();
s32 Func_020033a0();
extern u8 MsgSummonInheritedEarthDarkness[];

void Func_02001b88(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 10 of 35 instructions
   * differ in tla-en. */
  Func_02003398();
  Func_02003488(0);
  Call2(Func_02003460, 0x10000, 0);
  Func_02003458(SPLIT_CONSTANT(0x10000, 3), 1);
  Func_02003468(0x3c);
  FieldEvent_Wait(60);
  Call3(Func_02003388, (s32)MsgSummonInheritedEarthDarkness, 0, 6);
  Func_02003458(0x10000, 0);
  Func_02003468(0x3c);
  FieldEvent_Wait(60);
  GameFlag_SetBit(SPLIT_CONSTANT(0xa00, 140));
  Func_020033a0();
}
