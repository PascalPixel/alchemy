#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02000fcc();
s32 Func_0200104c();
s32 Func_0200043c();
s32 GameFlag_SetBit();
s32 Func_02000fd4();
extern u8 Data_0200117c[];

void Func_02000728(void)
{
    s32 t0;

    t0 = Value1(GameFlag_Test, 0x307);
    if (t0 == 0) {
        Func_02000fcc();
        Call1(Func_0200104c, 0);
        Call2(Func_0200043c, (s32)Data_0200117c, 4);
        Call1(GameFlag_SetBit, 0x307);
        Func_02000fd4();
    }
}

