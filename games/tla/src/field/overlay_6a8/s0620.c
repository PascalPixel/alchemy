#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02004464();
s32 Func_020045ec();
s32 Func_0200451c();
s32 Func_0200453c();
s32 Func_0200446c();
extern u8 MsgScene6A8No33[];

void Func_02000620(void)
{
    Func_02004464();
    Call1(Func_020045ec, 0);
    Call1(Func_0200451c, (s32)MsgScene6A8No33);
    Call2(Func_0200453c, 0xf, 0);
    Func_0200446c();
}

