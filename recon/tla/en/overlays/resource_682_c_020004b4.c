#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02002adc();
s32 UiText_OpenMessageAtObject();
s32 Func_02002bb4();
s32 Inventory_PromptAndSetObjectMode();
s32 Battle_WaitMode0();
s32 Func_02002af4();
extern u8 MsgScene682No30[];
extern u8 MsgScene682No05[];

void Func_020004b4(s32 a0)
{
    s32 t0;
    s32 t4;
    s32 t13;

    t0 = Value1(GameFlag_Test, 0x97f);
    if (t0 != 0) {
        Call1(Func_02002adc, (s32)MsgScene682No30);
        Call2(UiText_OpenMessageAtObject, a0, 0);
        t4 = Value2(Inventory_PromptAndSetObjectMode, Func_02002bb4(), 0);
        if (t4 == 0) {
            Call1(Battle_WaitMode0, 0xa);
            Call1(Func_02002adc, 0x25c9);
        } else {
            Call1(Battle_WaitMode0, 0x14);
            Call1(Func_02002adc, 0x25ca);
        }
        Call2(Func_02002af4, a0, 0);
    } else {
        Call1(Func_02002adc, (s32)MsgScene682No05);
        Call2(UiText_OpenMessageAtObject, a0, 0);
        t13 = Value2(Inventory_PromptAndSetObjectMode, Func_02002bb4(), 0);
        if (t13 == 0) {
            Call1(Battle_WaitMode0, 0xa);
            Call1(Func_02002adc, 0x23f1);
        } else {
            Call1(Battle_WaitMode0, 0x14);
            Call1(Func_02002adc, 0x23f2);
        }
        Call2(Func_02002af4, a0, 0);
    }
}

