#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_02002bbc();
s32 Func_02002aa4();
s32 GameFlag_SetBit();

void Func_0200060c(void)
{
    Call2(Func_02002bbc, 0x1f, 0x19f);
    Call4(Func_02002aa4, 0x1f, 0x1380000, 0x2510000, 0xc000);
    Call1(GameFlag_SetBit, 0xa74);
}

