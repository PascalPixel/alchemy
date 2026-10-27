#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02003398();
s32 Func_02003488();
s32 Func_02003328();
s32 Func_020034b8();
s32 FieldEvent_WaitTicks();
s32 GameFlag_ClearBit();
s32 Func_020033a0();

void Func_020016fc(void) {
  s32 t0;

  t0 = Value1(Object_GetById, 0xe);
  Func_02003398();
  Call1(Func_02003488, 0);
  FIELD_AT(t0, s32, 8) = 0x2180000;
  FIELD_AT(t0, s32, 16) = 0x1680000;
  FIELD_AT(t0, s32, 20) = -917504;
  FIELD_AT(t0, s32, 12) = -917504;
  Call6(Func_02003328, 0x33, 0x16, 0x21, 0x16, 1, 1);
  Call1(Func_020034b8, 0x248);
  Call1(FieldEvent_WaitTicks, 0x1e);
  Call1(Func_020034b8, 0xbc);
  Call6(Func_02003328, 0x32, 0x14, 0x1f, 0x14, 1, 2);
  Call1(FieldEvent_WaitTicks, 5);
  Call6(Func_02003328, 0x34, 0x14, 0x1f, 0x14, 1, 2);
  Call1(FieldEvent_WaitTicks, 5);
  Call1(GameFlag_ClearBit, 0x200);
  Func_020033a0();
}
