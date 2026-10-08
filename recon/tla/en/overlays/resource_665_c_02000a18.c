#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02001bcc();

s32 Func_02000a18(void)
{
    s32 t0;

    t0 = Value1(GameFlag_Test, 0x8ab);
    if (t0 != 0) {
        FIELD_AT(0x50000f6, u16, 0) = FIELD_AT(0x5000116, u16, 0);
        FIELD_AT(0x50000f8, u16, 0) = FIELD_AT(0x5000118, u16, 0);
        FIELD_AT(0x50000fa, u16, 0) = FIELD_AT(0x500011a, u16, 0);
        FIELD_AT(0x50000fc, u16, 0) = FIELD_AT(0x500011a, u16, 2);
        Call6(Func_02001bcc, 0x25, 0x12, 2, 2, 0x25, 0x21);
        Call6(Func_02001bcc, 0x25, 0x12, 2, 3, 0x27, 0x1f);
        Call6(Func_02001bcc, 0x32, 0x50, 7, 5, 0x23, 0x5f);
    }
    return 0;
}

