#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 GameFlag_Test();
s32 GameFlag_SetBit();
s32 Func_02002adc();
s32 Func_02002af4();
extern u8 MsgObabaIsTheLastPossessor[];
extern u8 MsgYouDThinkObabaWould[];

void Func_02000554(s32 a0)
{
    s32 t0;

    t0 = Value1(GameFlag_Test, 0x972);
    if (t0 == 0) {
        Call1(GameFlag_SetBit, 0x972);
        Call1(Func_02002adc, (s32)MsgObabaIsTheLastPossessor);
    } else {
        Call1(Func_02002adc, (s32)MsgYouDThinkObabaWould);
    }
    Call2(Func_02002af4, a0, 0);
}

