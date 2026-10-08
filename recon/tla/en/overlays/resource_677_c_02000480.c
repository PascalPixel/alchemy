#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
void Func_020009b0(void);
void Func_02000a50(s32 a);
void ObjectMotion_SetVariantCallback(s32 a, s32 b);
void Func_02000a18(s32 a);
void UiText_OpenMessageAtObject(s32 a, s32 b);
s32 Func_02000a58(void);
s32 Inventory_PromptAndSetObjectMode(s32 a, s32 b);
void Battle_WaitMode0(s32 a);
void Func_02000a28(s32 a, s32 b);
void Func_020009b8(void);
extern u8 MsgScene677No31[];

void Func_02000480(void)
{
    s32 t6;

    Func_020009b0();
    Call1(Func_02000a50, 0);
    Call2(ObjectMotion_SetVariantCallback, 0xe, 2);
    Call1(Func_02000a18, (s32)MsgScene677No31);
    Call2(UiText_OpenMessageAtObject, 0xe, 0);
    t6 = Value2(Inventory_PromptAndSetObjectMode, Func_02000a58(), 0);
    if (t6 == 0) {
        Call1(Battle_WaitMode0, 0xa);
        Call1(Func_02000a18, 0x2224);
    } else {
        Call1(Battle_WaitMode0, 0x14);
        Call1(Func_02000a18, 0x2225);
    }
    Call2(Func_02000a28, 0xe, 0);
    Func_020009b8();
}

