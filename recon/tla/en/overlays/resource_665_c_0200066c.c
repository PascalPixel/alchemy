#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02001c74();
s32 UiText_OpenMessageAtObject();
s32 Func_02001d34();
s32 Inventory_PromptAndSetObjectMode();
s32 Battle_WaitMode0();
s32 Func_02001c8c();
s32 GameFlag_SetBit();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_ArmCallback();
s32 ObjectMotion_SetSpeedParameters();
s32 Motion_SetModeAndWaitAnimation();
extern u8 MsgScene665No92[];
extern u8 gPartyState[];

void Func_0200066c(s32 a0)
{
    s32 t0;
    s32 t6;

    t0 = Value1(GameFlag_Test, 0x8b9);
    if (t0 != 0) {
        Call1(Func_02001c74, 0x1ca9);
        Call2(UiText_OpenMessageAtObject, a0, 0);
    } else {
        Call1(Func_02001c74, (s32)MsgScene665No92);
        Call2(UiText_OpenMessageAtObject, a0, 0);
        t6 = Value2(Inventory_PromptAndSetObjectMode, Func_02001d34(), 0);
        if (t6 != 0) {
            Call1(Battle_WaitMode0, 0x14);
            Call1(Func_02001c74, 0x1ca6);
            Call2(Func_02001c8c, a0, 0);
        } else {
            Call1(GameFlag_SetBit, 0x8b9);
            Call1(Battle_WaitMode0, 0x14);
            Call1(Func_02001c74, 0x1ca7);
            Call2(Func_02001c8c, a0, 0);
            Call3(ObjectMotion_SetPositionAndReset, FIELD_AT(gPartyState, s32, 532), 0x48, 0x158);
            Call3(ObjectMotion_ArmCallback, FIELD_AT(gPartyState, s32, 532), 0xc000, 0);
            Call1(Battle_WaitMode0, 0xa);
            Call3(ObjectMotion_SetSpeedParameters, a0, 0x10000, 0x8000);
            Call3(ObjectMotion_SetPositionAndReset, a0, 0x38, 0x148);
            Call3(ObjectMotion_ArmCallback, FIELD_AT(gPartyState, s32, 532), 0xa000, 0);
            Call3(ObjectMotion_ArmCallback, a0, 0x2000, 0);
            Call1(Battle_WaitMode0, 0x14);
            Call2(Motion_SetModeAndWaitAnimation, a0, 3);
            Call1(Battle_WaitMode0, 0xa);
            Call2(Func_02001c8c, a0, 0);
        }
    }
}

