#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02001c74();
s32 UiText_OpenMessageAtObject();
s32 Func_02001d34();
s32 Inventory_PromptAndSetObjectMode();
s32 Battle_WaitMode0();
s32 Func_02001c8c();
extern u8 MsgScene665No33[];

void Func_020001f0(s32 a0)
{
    s32 t3;

    Call1(Func_02001c74, (s32)MsgScene665No33);
    Call2(UiText_OpenMessageAtObject, a0, 0);
    t3 = Value2(Inventory_PromptAndSetObjectMode, Func_02001d34(), 0);
    if (t3 == 0) {
        Call1(Battle_WaitMode0, 0xa);
        Call1(Func_02001c74, 0x1b45);
    } else {
        Call1(Battle_WaitMode0, 0x14);
        Call1(Func_02001c74, 0x1b46);
    }
    Call2(Func_02001c8c, a0, 0);
}

