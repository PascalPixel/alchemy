#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_020009b0();
s32 Func_02000a50();
s32 GameFlag_Test();
s32 Func_02000a18();
s32 Func_02000a28();
s32 Func_020009b8();
extern u8 MsgItSBeenALong[];
extern u8 MsgIfItSNotRaining[];

s32 Func_0200008c(void)
{
    s32 t2;

    Func_020009b0();
    Call1(Func_02000a50, 0);
    t2 = Value1(GameFlag_Test, 0x301);
    if (t2 != 0) {
        Call1(Func_02000a18, (s32)MsgItSBeenALong);
    } else {
        Call1(Func_02000a18, (s32)MsgIfItSNotRaining);
    }
    Call2(Func_02000a28, 0xe, 0);
    Func_020009b8();
    return 0;
}

