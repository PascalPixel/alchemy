#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02000644();
s32 Func_02004634();
s32 Func_02004464();
s32 Func_020045ec();
s32 GameFlag_Test();
s32 Func_0200451c();
s32 Func_02004534();
s32 Func_0200446c();
extern u8 MsgScene6A8No135[];
extern u8 MsgScene6A8No56[];

void Func_02000674(void)
{
    s32 t0;
    s32 t4;

    t0 = Func_02000644();
    if (t0 != 0) {
        Call2(Func_02004634, 0xe, 0x16);
    } else {
        Func_02004464();
        Call1(Func_020045ec, 0);
        t4 = Value1(GameFlag_Test, 0xa3d);
        if (t4 != 0) {
            Call1(Func_0200451c, (s32)MsgScene6A8No135);
        } else {
            Call1(Func_0200451c, (s32)MsgScene6A8No56);
        }
        Call2(Func_02004534, 0x16, 0);
        Func_0200446c();
    }
}

