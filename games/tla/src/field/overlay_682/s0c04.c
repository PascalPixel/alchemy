#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02002a0c();
s32 Func_02002a04();

s32 Func_02000c04(void)
{
    s32 t0;

    t0 = Value1(GameFlag_Test, 0x97f);
    if (t0 != 0) {
        Call6(Func_02002a0c, 0xb, 0x33, 5, 4, 0x10, 0x17);
        Call6(Func_02002a04, 0xb, 0x33, 5, 6, 0x10, 0x17);
    }
    return 0;
}

