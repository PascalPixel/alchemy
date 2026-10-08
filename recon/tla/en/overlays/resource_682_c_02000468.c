#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02002adc();
s32 UiText_OpenMessageAtObject();
s32 Func_02002bb4();
s32 Inventory_PromptAndSetObjectMode();
s32 Battle_WaitMode0();
s32 Func_02002af4();
extern u8 MsgScene682No29[];

void Func_02000468(s32 a0)
{
    s32 t3;

    Call1(Func_02002adc, (s32)MsgScene682No29);
    Call2(UiText_OpenMessageAtObject, a0, 0);
    t3 = Value2(Inventory_PromptAndSetObjectMode, Func_02002bb4(), 0);
    if (t3 == 0) {
        Call1(Battle_WaitMode0, 0xa);
        Call1(Func_02002adc, 0x25c6);
    } else {
        Call1(Battle_WaitMode0, 0x14);
        Call1(Func_02002adc, 0x25c7);
    }
    Call2(Func_02002af4, a0, 0);
}

