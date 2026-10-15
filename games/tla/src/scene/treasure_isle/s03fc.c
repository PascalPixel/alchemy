#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02000f9c();
s32 Func_02000f94();
s32 GameFlag_SetBit();

void Func_020003fc(s32 a0, s32 a1) {
  Call6(Func_02000f9c, 0, 0x64, 3, 4,
        (FIELD_AT((((a1 << 16) >> 12) + a0), s32, 0) + 0x3f),
        (FIELD_AT((((a1 << 16) >> 12) + a0), s32, 4) - 2));
  Call6(Func_02000f94, 1, 0, 1, 1, FIELD_AT((((a1 << 16) >> 12) + a0), s32, 0),
        FIELD_AT((((a1 << 16) >> 12) + a0), s32, 4));
  Call1(GameFlag_SetBit, FIELD_AT((((a1 << 16) >> 12) + a0), u16, 12));
}
