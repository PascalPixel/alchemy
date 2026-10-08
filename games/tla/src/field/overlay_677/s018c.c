#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
void Func_020009b0(void);
void Func_02000a50(s32 a);
void Func_02000a18(s32 a);
void Func_02000a28(s32 a, s32 b);
void Func_020009b8(void);
extern u8 MsgScene677No22[];

s32 Func_0200018c(void)
{
    Func_020009b0();
    Call1(Func_02000a50, 0);
    Call1(Func_02000a18, (s32)MsgScene677No22);
    Call2(Func_02000a28, 0xa, 0);
    Func_020009b8();
    return 0;
}

