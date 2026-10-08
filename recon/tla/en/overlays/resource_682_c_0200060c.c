#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02002bbc();
s32 Func_02002aa4();
s32 GameFlag_SetBit();

void Func_0200060c(void)
{
    Call2(Func_02002bbc, 0x1f, 0x19f);
    {
        s32 y = 0xc000;
        /* FAKEMATCH: the native schedule builds this argument first. */
        __asm__ volatile("" : "+l"(y));
        Call4(Func_02002aa4, 0x1f, 0x1380000, 0x2510000, y);
    }
    Call1(GameFlag_SetBit, 0xa74);
}

