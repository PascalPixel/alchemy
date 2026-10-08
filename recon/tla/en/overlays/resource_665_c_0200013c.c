#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02001c74();
s32 UiText_OpenMessageAtObject();
s32 Func_02001d34();
s32 Inventory_PromptAndSetObjectMode();
s32 GameFlag_SetBit();
s32 Func_02001c8c();
extern u8 MsgScene665No03[];
extern u8 MsgScene665No04[];

void Func_0200013c(s32 a0)
{
    s32 t0;
    s32 t4;

    t0 = Value1(GameFlag_Test, 0x8b0);
    if (t0 == 0) {
        Call1(Func_02001c74, (s32)MsgScene665No03);
        Call2(UiText_OpenMessageAtObject, a0, 0);
        t4 = Value2(Inventory_PromptAndSetObjectMode, Func_02001d34(), 0);
        if (t4 == 0) {
            Call1(Func_02001c74, 0x1ac2);
            Call1(GameFlag_SetBit, 0x8b0);
        } else {
            Call1(Func_02001c74, 0x1ac3);
        }
        Call2(Func_02001c8c, a0, 0);
    } else {
        Call1(Func_02001c74, (s32)MsgScene665No04);
        Call2(Func_02001c8c, a0, 0);
    }
}

