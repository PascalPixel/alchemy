#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02001b94();
s32 Func_0200232c();
s32 Func_02002204();
s32 Battle_WaitMode0();
extern u8 gPartyState[];

void Func_02001c1c(s32 a0) {
  s32 t0;

  t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
  Func_02001b94();
  Call1(Func_0200232c, 0xa1);
  Call6(Func_02002204, FIELD_AT(a0, s16, 6), (FIELD_AT(a0, s16, 8) + 1), 1, 1,
        ((FIELD_AT(t0, s32, 8) >> 20) |
         (((0x40 & FIELD_AT(a0, u16, 6)) << 16) >> 16)),
        ((FIELD_AT(t0, s32, 16) >> 20) |
         (((0x40 & FIELD_AT(a0, u16, 8)) << 16) >> 16)));
  Call1(Battle_WaitMode0, 0xc);
}
