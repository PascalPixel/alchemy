#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_SetBit();
s32 Func_02004594();
s32 Func_020045e4();
s32 GameFlag_ClearBit();

void Func_020038e8(void)
{
    if (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0xca2), s16, 0) != 0) {
        Call1(GameFlag_SetBit, 0x106);
        Func_02004594();
        Func_020045e4();
        Call1(GameFlag_ClearBit, 0x106);
    }
}

