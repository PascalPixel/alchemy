#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02000644();
s32 Func_02004624();
s32 Func_02004464();
s32 Func_020045ec();
s32 GameFlag_Test();
s32 Func_0200451c();
s32 Func_02004534();
s32 Func_0200446c();
extern u8 MsgScene6A8No132[];
extern u8 MsgScene6A8No53[];

void Func_020006c4(void)
{
    s32 t0;
    s32 t4;

    t0 = Func_02000644();
    if (t0 != 0) {
        Call2(Func_02004624, 0x1f, 0xa);
    } else {
        Func_02004464();
        Call1(Func_020045ec, 0);
        t4 = Value1(GameFlag_Test, 0xa3d);
        if (t4 != 0) {
            Call1(Func_0200451c, (s32)MsgScene6A8No132);
        } else {
            Call1(Func_0200451c, (s32)MsgScene6A8No53);
        }
        Call2(Func_02004534, 0xa, 0);
        Func_0200446c();
    }
}

