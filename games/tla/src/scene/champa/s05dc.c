#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02002a44();
s32 Func_02002b64();
s32 Func_02002a5c();
s32 GameFlag_ClearBit();
s32 Func_02002a4c();

void Func_020005dc(s32 a0)
{
    s32 t2;

    Func_02002a44();
    Call1(Func_02002b64, 0);
    t2 = Value3(Func_02002a5c, a0, 0x19f, -1);
    if (t2 == 0) {
        Call1(GameFlag_ClearBit, 0xa74);
    }
    Func_02002a4c();
}

