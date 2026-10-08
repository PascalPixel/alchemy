#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02000b24();
s32 UiText_OpenMessageAtObject();
s32 Func_02000ba4();
s32 Inventory_PromptAndSetObjectMode();
s32 Battle_WaitMode0();
s32 Func_02000b3c();
extern u8 MsgScene666No74[];

void Func_020004f4(s32 a0)
{
    s32 t3;

    Call1(Func_02000b24, (s32)MsgScene666No74);
    Call2(UiText_OpenMessageAtObject, a0, 0);
    t3 = Value2(Inventory_PromptAndSetObjectMode, Func_02000ba4(), 0);
    if (t3 == 0) {
        Call1(Battle_WaitMode0, 0xa);
        Call1(Func_02000b24, 0x1c68);
    } else {
        Call1(Battle_WaitMode0, 0x14);
        Call1(Func_02000b24, 0x1c69);
    }
    Call2(Func_02000b3c, a0, 0);
}

