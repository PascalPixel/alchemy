#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_020009b0();
s32 Func_02000a50();
s32 Func_02000a18();
s32 Func_02000a28();
s32 Func_020009b8();
extern u8 MsgIShouldnTTellHim[];

s32 Func_020001b4(void)
{
    Func_020009b0();
    Call1(Func_02000a50, 0);
    Call1(Func_02000a18, (s32)MsgIShouldnTTellHim);
    Call2(Func_02000a28, 0xb, 0);
    Func_020009b8();
    return 0;
}

