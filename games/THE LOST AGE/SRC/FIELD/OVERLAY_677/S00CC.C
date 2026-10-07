#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
void Func_020009b0(void);
void Func_02000a50(s32 a);
s32 GameFlag_Test(s32 a);
void Func_02000a18(s32 a);
void Func_02000a28(s32 a, s32 b);
void Func_020009b8(void);
extern u8 MsgWhatHappenedToSunshineHe[];
extern u8 MsgSunshineSAPrettyStubborn[];

s32 Func_020000cc(void)
{
    s32 t2;

    Func_020009b0();
    Call1(Func_02000a50, 0);
    t2 = Value1(GameFlag_Test, 0x301);
    if (t2 != 0) {
        Call1(Func_02000a18, (s32)MsgWhatHappenedToSunshineHe);
    } else {
        Call1(Func_02000a18, (s32)MsgSunshineSAPrettyStubborn);
    }
    Call2(Func_02000a28, 0xf, 0);
    Func_020009b8();
    return 0;
}

