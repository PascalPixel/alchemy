#include "types.h"
#include "call.h"

s32 Battle_WaitMode0();
s32 Func_020051ac();
s32 Func_020051b4();
s32 Func_0200520c();
s32 Func_0200524c();
s32 Func_0200525c();
s32 Func_0200528c();
s32 Func_02005294();
s32 Func_020052b4();
s32 Func_02005324();
s32 Func_02005334();
s32 GameFlag_SetBit();
s32 Motion_CamBounds();
s32 Motion_SetVarCbAndRefresh();
s32 ObjectMotion_ArmCallback();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 ObjectMotion_Launch();
s32 ObjectMotion_ResetAndSetPosition();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_SetVariantCallback();
s32 Object_AttachWorkTargetToObject();
s32 Object_GetById();
s32 Object_SetModeById();
extern u8 MsgGaroh13[];
extern u8 gPartyState[];

void Func_020022f4(void)
{
    s32 v5;
    s32 t1;
    s32 t2;
    s32 t3;
    s32 b1;

    Call1(GameFlag_SetBit, 0x91d);
    Func_020051ac();
    Call1(Func_02005324, 0);
    Call1(Func_0200524c, ((s32)MsgGaroh13));
    Call3(Func_0200520c, 8, 0x900000, 0x2f00000);
    Call3(ObjectMotion_SetPositionAndReset, 4, 0x118, 0x308);
    Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 8, 0, 0);
    Call4(Func_02005334, 9, 16, 0, 0xb000);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 9);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_ArmCallback, 9, 0x8000, 0);
    Call3(ObjectMotion_ArmCallback, 4, 0, 0);
    Call1(Battle_WaitMode0, 20);
    Call2(ObjectMotion_SetVariantCallback, 9, 3);
    Call2(Func_02005294, 9, 0x102);
    Call2(ObjectMotion_SetVariantCallback, 4, 3);
    Call2(Func_02005294, 4, 0x102);
    Call1(Battle_WaitMode0, 45);
    Call3(Func_0200525c, 8, 0, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0xb000, 0);
    Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
    Call1(Battle_WaitMode0, 20);
    Call4(Motion_CamBounds, 0x1180000, -1, 0x2f00000, 1);
    Func_020052b4();
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200520c, 8, 0x900000, 0x2d00000);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0xcccc, 0x6666);
    Call3(ObjectMotion_SetPositionAndReset, 8, 168, 0x2d0);
    Call1(Battle_WaitMode0, 10);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x20000, 0x10000);
    Call3(ObjectMotion_Launch, 8, 6, 0);
    Call3(ObjectMotion_SetPositionAndReset, 8, 200, 0x2d0);
    Call1(Battle_WaitMode0, 10);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0xcccc, 0x6666);
    Call3(ObjectMotion_SetPositionAndReset, 8, 216, 0x2c8);
    Call3(ObjectMotion_SetPositionAndReset, 8, 0x140, 0x2c8);
    Call3(Func_0200525c, 9, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 8, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x9999, 0x4ccc);
    t1 = Value1(Object_GetById, 8);
    *(u8 *)((t1 + 90)) = (254 & *(u8 *)((t1 + 90)));
    Call3(ObjectMotion_SetPositionAndReset, 8, 0x128, 0x2c8);
    Call1(Battle_WaitMode0, 1);
    t2 = Value1(Object_GetById, 8);
    /* FAKEMATCH: the value is ORed into the register that holds the constant, as the native does; as one |= the constant is built second. */
    b1 = 1;
    b1 |= *(u8 *)((t2 + 90));
    *(u8 *)((t2 + 90)) = b1;
    Call1(Battle_WaitMode0, 15);
    Call3(ObjectMotion_ArmCallback, 8, 0x9000, 0);
    Call1(Battle_WaitMode0, 25);
    Call3(ObjectMotion_ArmCallback, 8, 0xf000, 0);
    Call1(Battle_WaitMode0, 25);
    Call3(ObjectMotion_ArmCallback, 8, 0x9000, 0);
    Call1(Battle_WaitMode0, 25);
    Call3(ObjectMotion_ArmCallback, 8, 0x4000, 0);
    Call1(Battle_WaitMode0, 25);
    {
        s32 a1 = 0x102;
        s32 a2 = 45;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(8, a1, a2);
    }
    Call3(ObjectMotion_Launch, 8, 6, 15);
    Call3(ObjectMotion_Launch, 8, 6, 23);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x26666, 0x13333);
    Call3(ObjectMotion_SetPositionAndReset, 8, 200, 0x2c8);
    Call1(Battle_WaitMode0, 3);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x20000, 0x10000);
    Call3(ObjectMotion_Launch, 8, 6, 0);
    Call3(ObjectMotion_SetPositionAndReset, 8, 168, 0x2c8);
    Call1(Battle_WaitMode0, 3);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x26666, 0x13333);
    Call3(ObjectMotion_SetPositionAndReset, 8, 144, 0x2c8);
    {
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the third argument first; behind the barrier that follows, the plain call loads the second first (one reordered pair, differ 2). */
        asm volatile("" : "+l"(a2));
        Call3(Func_0200520c, 8, 0, a2);
    }
    Call1(Battle_WaitMode0, 30);
    v5 = ((s32)gPartyState);
    v5 = (v5 + 0x214);
    {
        s32 a0 = *(s32 *)(v5);
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_AttachWorkTargetToObject(a0, 1);
    }
    Func_020052b4();
    Call1(Battle_WaitMode0, 20);
    Func_0200528c(9, 0x102, 30);
    Call3(Func_0200525c, 9, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 9, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(ObjectMotion_ArmCallback, 9, 0x8000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 9, 0, 0);
    Call3(ObjectMotion_ArmCallback, 4, 0, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_Launch, 9, 6, 15);
    Call3(ObjectMotion_Launch, 9, 6, 23);
    {
        s32 a0 = 4;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(ObjectMotion_SetSpeedParameters, 9, 0x13333, 0x9999);
    Call2(Object_SetModeById, 9, 2);
    t3 = Value1(Object_GetById, *(s32 *)(v5));
    if (t3 == 0)
        goto L_0200a66c;
    Call3(ObjectMotion_ResetAndSetPosition, 9, *(s16 *)(t3 + 10), *(s16 *)(t3 + 18));
L_0200a66c:;
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 9);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Func_0200520c(a0, 0, 0);
    }
    Func_020051b4();
}
