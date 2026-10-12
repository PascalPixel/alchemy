/* Near miss, differ 31 of 2,484 (6,308 bytes), lifted mechanically from the listing: 656
 * calls through call.h's layer or direct, with tagged argument barriers where the bytes
 * need them. Left: (1) two call sites one instruction out of order (Call2 with 5, 3 and
 * the call whose first argument is 0x200a); (2) at the sprite scale store the native
 * loads the sprite before it starts 0x6666, and keeps one load of the sprite; (3) after
 * the 21-step loop the native stores the 0x4000 angle before the next call's zero;
 * (4) the literal pools then fall where the native's do (the rest of the count). */
#include "types.h"
#include "call.h"

#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

s32 Battle_WaitMode0();
s32 Func_020013b0();
s32 Func_02001418();
s32 Func_02001454();
s32 Func_020051ac();
s32 Func_020051b4();
s32 Func_0200520c();
s32 Func_0200524c();
s32 Func_0200525c();
s32 Func_0200527c();
s32 Func_0200528c();
s32 Func_02005294();
s32 Func_020052b4();
s32 Func_02005324();
s32 Func_0200532c();
s32 Func_02005334();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 Inventory_PromptAndSetObjectMode();
s32 Motion_CamBounds();
s32 Motion_SetVarCbAndRefresh();
s32 ObjectDispatch_SetSingleChildFlags();
s32 ObjectMotion_ArmCallback();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 ObjectMotion_CommitPositionAndActivate();
s32 ObjectMotion_Launch();
s32 ObjectMotion_ResetAndSetPosition();
s32 ObjectMotion_SetActionVariant();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_SetVariantCallback();
s32 Object_AttachWorkTargetToObject();
s32 Object_GetById();
s32 Object_SetModeById();
s32 UiText_OpenMessageAtObject();
s32 WaitFrames();
s32 __divsi3();
extern u8 MsgScene65FNo28[];
extern u8 gPartyState[];

void Func_02003878(void)
{
    s32 v5;
    s32 v6;
    s32 v7;
    s32 v8;
    s32 t1;
    s32 t2;
    s32 t3;
    s32 t4;
    s32 t5;
    s32 t6;
    s32 t7;
    s32 t8;
    s32 t9;
    s32 t10;
    s32 t11;
    s32 t12;
    s32 t13;
    s32 t14;
    s32 t15;
    s32 t16;
    s32 t17;
    s32 t18;
    s32 t19;
    s32 t20;
    s32 t21;
    s32 t22;
    s32 t23;
    s32 t24;
    s32 t25;
    s32 t26;
    s32 t27;
    s32 t28;
    s32 t29;
    s32 t30;
    s32 t31;
    s32 b1;
    s32 p1;
    s32 p2;

    Call1(GameFlag_SetBit, 0x91f);
    Func_020051ac();
    Call1(Func_02005324, 0);
    Call1(Func_0200524c, ((s32)MsgScene65FNo28));
    Call3(Func_0200520c, 10, 0x1480000, 0x280000);
    Call3(ObjectMotion_ArmCallback, 10, 0x4000, 0);
    Call3(Func_0200525c, 10, 0, 0);
    Call3(ObjectMotion_SetSpeedParameters, 4, 0xcccc, 0x6666);
    Call3(ObjectMotion_SetPositionAndReset, 4, 0x140, 224);
    Call4(Func_02005334, 9, -16, 0, 0xc000);
    Call4(Func_02005334, 5, 16, 0, 0xc000);
    Call4(Func_02005334, 6, 32, 0, 0xc000);
    t1 = Value1(GameFlag_Test, 7);
    if (t1 == 0)
        goto L_0200b920;
    Call4(Func_02005334, 7, 48, 0, 0xc000);
L_0200b920:;
    Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 6);
    Call1(Battle_WaitMode0, 20);
    Call4(Motion_CamBounds, 0x1480000, -1, 0x900000, 1);
    Func_020052b4();
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200520c, 11, 0x1480000, 0x880000);
    t2 = Value1(Object_GetById, 11);
    Call2(ObjectDispatch_SetSingleChildFlags, t2, 0);
    Call1(Func_02001418, 11);
    Call1(Battle_WaitMode0, 30);
    Call3(ObjectMotion_SetSpeedParameters, 10, 0xb333, 0x5999);
    Call3(Func_0200520c, 10, 0x1480000, 0x800000);
    Call1(Battle_WaitMode0, 10);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 0, 16);
    Call1(Battle_WaitMode0, 20);
    Call1(Func_02001454, 11);
    Call3(Func_0200520c, 11, 0, 0);
    Call2(Object_AttachWorkTargetToObject, 10, 1);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 0, 48);
    Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
    Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xa000, 0);
    t3 = Value1(GameFlag_Test, 7);
    if (t3 == 0)
        goto L_0200ba1e;
    Call3(ObjectMotion_ArmCallback, 7, 0xa000, 0);
L_0200ba1e:;
    Call4(Motion_CamBounds, 0x1480000, -1, 0xb80000, 1);
    Func_020052b4();
    Call1(Battle_WaitMode0, 10);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x600a, 0, 0);
    Call2(Object_SetModeById, 4, 3);
    Call2(Object_SetModeById, 5, 3);
    Call2(Object_SetModeById, 6, 3);
    t4 = Value1(GameFlag_Test, 7);
    if (t4 == 0)
        goto L_0200ba8c;
    Call2(Object_SetModeById, 7, 3);
    goto L_0200ba8c;
L_0200ba8c:;
    Call2(Object_SetModeById, 9, 3);
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 9, 0, -16);
    Call1(Battle_WaitMode0, 10);
    Call2(Motion_SetVarCbAndRefresh, 9, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, -8, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 0x600a, 0, 0);
    {
        s32 a1 = 0x102;
        s32 a2 = 30;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(9, a1, a2);
    }
    Call3(Func_0200525c, 9, 0, 0);
    Func_0200528c(10, 0x107, 30);
    Call3(Func_0200525c, 0x600a, 0, 0);
    Func_0200528c(9, 0x109, 30);
    Call3(Func_0200525c, 9, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x600a, 0, 0);
    {
        s32 a1 = 0x101;
        s32 a2 = 30;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(9, a1, a2);
    }
    Call3(Func_0200525c, 9, 0, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 16, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 0x600a, 0, 0);
    {
        s32 a1 = 0x100;
        s32 a2 = 20;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(5, a1, a2);
    }
    Call3(Func_0200525c, 5, 0, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x2000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 0x600a, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 6, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 6, 0, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, -8, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 0x600a, 0, 0);
    {
        s32 a0 = 4;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 35);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, -8, 0);
    {
        s32 a1 = 0x5000;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_ArmCallback(10, a1, a2);
    }
    Call1(Battle_WaitMode0, 25);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x600a, 0, 0);
    {
        s32 a1 = 0x102;
        s32 a2 = 30;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(9, a1, a2);
    }
    Call3(Func_0200525c, 9, 0, 0);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 4);
    }
    Call1(Battle_WaitMode0, 33);
    Call3(Func_0200525c, 0x600a, 0, 0);
    Func_0200528c(9, 0x103, 45);
    Call3(ObjectMotion_Launch, 9, 6, 15);
    Call3(ObjectMotion_Launch, 9, 6, 23);
    Call3(Func_0200525c, 9, 0, 0);
    Func_0200528c(10, 0x108, 30);
    Call3(Func_0200525c, 0x600a, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 9, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(ObjectMotion_ArmCallback, 10, 0xc000, 0);
    Call1(Battle_WaitMode0, 30);
    Call3(Func_0200525c, 0x600a, 0, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 4, -12);
    Call1(Battle_WaitMode0, 10);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x200a, 0, 0);
    {
        s32 a1 = 4;
        s32 a2 = -12;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_CommitPositionAndActivate(10, a1, a2);
    }
    Call1(Battle_WaitMode0, 10);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 4);
    }
    Call1(Battle_WaitMode0, 33);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x4000, 0);
    Call1(Battle_WaitMode0, 30);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call2(Object_SetModeById, 4, 3);
    Call2(Object_SetModeById, 5, 3);
    Call2(Object_SetModeById, 6, 3);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 9, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    Func_0200528c(10, 0x105, 30);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x200a, 0, 0);
    {
        s32 a1 = 0x101;
        s32 a2 = 30;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(6, a1, a2);
    }
    Call3(Func_0200525c, 6, 0, 0);
    {
        s32 a1 = 0x3000;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_ArmCallback(10, a1, a2);
    }
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x200a, 0, 0);
    {
        s32 a1 = 0x102;
        s32 a2 = 30;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(10, a1, a2);
    }
    Call3(Func_0200525c, 0x200a, 0, 0);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    {
        s32 a1 = 0x5000;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_ArmCallback(10, a1, a2);
    }
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 4);
    }
    Call1(Battle_WaitMode0, 33);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0x2000, 0);
    Call3(ObjectMotion_ArmCallback, 4, 0xa000, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0x8000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0, 0);
    t5 = Value1(GameFlag_Test, 7);
    if (t5 == 0)
        goto L_0200bf32;
    Call3(ObjectMotion_ArmCallback, 7, 0x8000, 0);
L_0200bf32:;
    Call1(Battle_WaitMode0, 40);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x200a, 0, 0);
    t6 = Value1(GameFlag_Test, 7);
    if (t6 == 0)
        goto L_0200bf6a;
    Call3(ObjectMotion_ArmCallback, 7, 0xa000, 0);
L_0200bf6a:;
    Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
    Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xa000, 0);
    Call1(Battle_WaitMode0, 25);
    Call2(Motion_SetVarCbAndRefresh, 9, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    Func_0200528c(10, 0x101, 30);
    Call3(Func_0200525c, 0x200a, 0, 0);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x200a, 0, 0);
    {
        s32 a1 = 0x100;
        s32 a2 = 30;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(10, a1, a2);
    }
    Call3(Func_0200525c, 0x200a, 0, 0);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    {
        s32 a1 = 0x2000;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_ArmCallback(9, a1, a2);
    }
    Call1(Battle_WaitMode0, 30);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 15);
    {
        s32 a1 = 0xc000;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_ArmCallback(10, a1, a2);
    }
    Call1(Battle_WaitMode0, 30);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call2(Object_SetModeById, 4, 3);
    {
        s32 a0 = 5;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 30);
    {
        s32 a1 = 0xe000;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_ArmCallback(9, a1, a2);
    }
    Call1(Battle_WaitMode0, 25);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    Call2(ObjectMotion_SetVariantCallback, 10, 3);
    Call2(Func_02005294, 10, 0x102);
    Call1(Battle_WaitMode0, 40);
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call1(Battle_WaitMode0, 30);
    Call3(Func_0200525c, 0x200a, 0, 0);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    Func_0200528c(10, 0x100, 30);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 9, 0, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 9, 16, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 9, 16, 0);
    Call3(ObjectMotion_ArmCallback, 4, 0xe000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 9, 16, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x3000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0xe000, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xc000, 0);
    t7 = Value1(GameFlag_Test, 7);
    if (t7 == 0)
        goto L_0200c216;
    Call3(ObjectMotion_ArmCallback, 7, 0xc000, 0);
L_0200c216:;
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 9, 16, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xe000, 0);
    Call1(Battle_WaitMode0, 20);
    Call4(Motion_CamBounds, 0x1580000, -1, 0xd00000, 1);
    Func_020052b4();
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 9, 0, 0);
    Call3(ObjectMotion_SetSpeedParameters, 10, 0x13333, 0x9999);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 16, 16);
    Call3(ObjectMotion_SetSpeedParameters, 10, 0xb333, 0x5999);
    Call3(Func_0200525c, 10, 0, 0);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    t8 = Value1(GameFlag_Test, 7);
    if (t8 == 0)
        goto L_0200c2bc;
    Call3(ObjectMotion_ArmCallback, 7, 0xa000, 0);
L_0200c2bc:;
    Call3(ObjectMotion_ArmCallback, 4, 0xe000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0xa000, 0);
    Call1(Battle_WaitMode0, 30);
    Call3(Func_0200525c, 9, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 10, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 5, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 5, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 10, 0, 0);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 4);
    }
    Call1(Battle_WaitMode0, 33);
    Call3(Func_0200525c, 9, 0, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x2000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 10, 0, 0);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call1(Battle_WaitMode0, 20);
    Func_0200528c(6, 0x106, 40);
    Call3(ObjectMotion_ArmCallback, 6, 0x8000, 0);
    Call1(Battle_WaitMode0, 20);
    UiText_OpenMessageAtObject(6, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0x5000, 0);
    Call3(ObjectMotion_ArmCallback, 4, 0, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0, 0);
    t9 = Value1(GameFlag_Test, 7);
    if (t9 == 0)
        goto L_0200c400;
    Call3(ObjectMotion_ArmCallback, 7, 0x8000, 0);
L_0200c400:;
    t10 = Value2(Inventory_PromptAndSetObjectMode, 4, 0);
    if (t10 != 0)
        goto L_0200c434;
    Call1(Battle_WaitMode0, 25);
    Call3(Func_0200525c, 6, 0, 0);
    *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) = (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
    goto L_0200c456;
L_0200c434:;
    Call1(Battle_WaitMode0, 40);
    *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) = (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
    Call3(Func_0200525c, 6, 0, 0);
L_0200c456:;
    {
        s32 a1 = 0x101;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(9, a1, a2);
    }
    Call3(Func_0200528c, 5, 0x101, 0);
    t11 = Value1(GameFlag_Test, 7);
    if (t11 == 0)
        goto L_0200c484;
    Call3(Func_0200528c, 7, 0x101, 0);
L_0200c484:;
    {
        s32 a1 = 0x101;
        s32 a2 = 50;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(10, a1, a2);
    }
    Call3(ObjectMotion_ArmCallback, 9, 0x5000, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xe000, 0);
    Call1(Battle_WaitMode0, 30);
    Call3(Func_0200525c, 6, 0, 0);
    Func_0200528c(9, 0x101, 30);
    Call3(Func_0200525c, 9, 0, 0);
    Func_0200528c(6, 0x107, 30);
    Call3(Func_0200525c, 6, 0, 0);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 4);
    }
    Call1(Battle_WaitMode0, 33);
    Call3(Func_0200525c, 10, 0, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xc000, 0);
    Call1(Battle_WaitMode0, 20);
    Func_0200528c(6, 0x103, 30);
    Call3(Func_0200525c, 6, 0, 0);
    {
        s32 a0 = 6;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 4);
    }
    Call1(Battle_WaitMode0, 33);
    Call3(Func_0200525c, 6, 0, 0);
    {
        s32 a1 = 0x105;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(9, a1, a2);
    }
    Func_0200528c(10, 0x105, 50);
    Call2(Motion_SetVarCbAndRefresh, 6, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 6, 0, 0);
    Call2(Object_SetModeById, 4, 3);
    Call2(Object_SetModeById, 5, 3);
    Call2(Object_SetModeById, 10, 3);
    t12 = Value1(GameFlag_Test, 7);
    if (t12 == 0)
        goto L_0200c596;
    Call2(Object_SetModeById, 7, 3);
L_0200c596:;
    Call2(Object_SetModeById, 9, 3);
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    Call2(Motion_SetVarCbAndRefresh, 6, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 6, 0, 0);
    Call2(ObjectMotion_SetVariantCallback, 10, 3);
    Call2(Func_02005294, 10, 0x102);
    Call2(ObjectMotion_SetVariantCallback, 9, 3);
    Call2(Func_02005294, 9, 0x102);
    Call1(Battle_WaitMode0, 40);
    Call3(ObjectMotion_ArmCallback, 4, 0xe000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0xe000, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0xa000, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x2000, 0);
    Call1(Battle_WaitMode0, 25);
    Call3(ObjectMotion_Launch, 9, 6, 15);
    Call3(ObjectMotion_Launch, 9, 6, 23);
    Call1(Battle_WaitMode0, 10);
    Call2(Object_SetModeById, 9, 3);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    v5 = 0;
    Call11(Func_0200527c, 10, 9, 2, 9, 5, 9, 17, 14, 20, 9, v5);
    t13 = Value1(GameFlag_Test, 7);
    if (t13 == 0)
        goto L_0200c694;
    Call3(ObjectMotion_ArmCallback, 7, 0xa000, 0);
L_0200c694:;
    Call2(ObjectMotion_SetVariantCallback, 10, 3);
    Call2(Func_02005294, 10, 0x102);
    Call1(Battle_WaitMode0, 40);
    Call3(Func_0200525c, 10, 0, 0);
    Call2(ObjectMotion_SetVariantCallback, 9, 3);
    Call2(Func_02005294, 9, 0x102);
    Call1(Battle_WaitMode0, 40);
    Call3(Func_0200525c, 9, 0, 0);
    Func_0200528c(10, 0x100, 30);
    Call3(Func_0200525c, 10, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 10, 0, 0);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 10, 0, 0);
    {
        s32 a0 = 6;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 6, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 9, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(ObjectMotion_ArmCallback, 9, 0x6000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 9, 0, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0xa000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 9, 0, 0);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(ObjectMotion_CommitPositionAndActivate, 9, -18, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0x5000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 0x2009, 0, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0xc000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    Call2(Object_SetModeById, 5, 3);
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    t14 = Value1(GameFlag_Test, 7);
    if (t14 == 0)
        goto L_0200c880;
    Call3(ObjectMotion_ArmCallback, 9, 0x2000, 0);
    Call3(Func_0200525c, 0x2009, 0, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    Call2(Object_SetModeById, 7, 3);
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    goto L_0200c892;
L_0200c880:;
    *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) = (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
L_0200c892:;
    Call3(ObjectMotion_CommitPositionAndActivate, 9, -42, 0);
    Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
    {
        s32 a1 = 0x3000;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_ArmCallback(9, a1, a2);
    }
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, -16, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xa000, 0);
    {
        s32 a1 = 0x5000;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_ArmCallback(10, a1, a2);
    }
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 30);
    {
        s32 a0 = 4;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 25);
    Call3(Func_0200525c, 0x200a, 0, 0);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 4);
    }
    Call1(Battle_WaitMode0, 33);
    Call3(Func_0200525c, 9, 0, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0xc000, 0);
    Call1(Battle_WaitMode0, 40);
    Call1(Func_020013b0, 10);
    Call1(Func_020013b0, 10);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 16, 0);
    Call1(Battle_WaitMode0, 20);
    Call2(ObjectMotion_SetVariantCallback, 10, 3);
    Call2(Func_02005294, 10, 0x102);
    Call1(Battle_WaitMode0, 40);
    Call3(ObjectMotion_ArmCallback, 10, 0x8000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, -16, 0);
    Call1(Battle_WaitMode0, 10);
    {
        s32 a1 = -16;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_CommitPositionAndActivate(10, a1, a2);
    }
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 4);
    }
    Call1(Battle_WaitMode0, 33);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        ObjectMotion_ArmCallback(a0, 0, 0);
    }
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 16, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_ArmCallback, 10, 0xc000, 0);
    Call1(Battle_WaitMode0, 20);
    Call2(ObjectMotion_SetVariantCallback, 10, 3);
    Call2(Func_02005294, 10, 0x102);
    Call1(Battle_WaitMode0, 40);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 0, -12);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 0, -12);
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    Func_0200528c(10, 0x106, 40);
    Call3(ObjectMotion_ArmCallback, 10, 0x4000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 10, 0, 0);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 10);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 5);
    t15 = Value1(Object_GetById, 12);
    v5 = t15;
    *(s32 *)(*(s32 *)(v5 + 80) + 12) = 0x6666;
    v8 = *(s32 *)(v5 + 80);
    Call2(ObjectMotion_SetActionVariant, 12, 1);
    t16 = Value1(Object_GetById, 12);
    Call2(ObjectDispatch_SetSingleChildFlags, t16, 0);
    Call3(Func_0200520c, 12, 0x1480000, 0xa00000);
    Call3(ObjectMotion_Launch, 12, 9, 0);
    v7 = 0;
    v6 = 20;
L_0200caf6:;
    t17 = Value2(__divsi3, v7, 20);
    *(s32 *)(v8 + 12) = (t17 + 0x6666);
    {
        s32 angle = *(u16 *)(v5 + 6);
        /* FAKEMATCH: the native starts the 0x1000 step before it loads the angle; the plain sum loads first (one reordered pair, differ 2). */
        __asm__("" : "+l"(angle));
        *(u16 *)(v5 + 6) = angle + 0x1000;
    }
    Call1(WaitFrames, 1);
    v6 = (v6 - 1);
    v7 = (v7 + 0x9999);
    if (v6 >= 0)
        goto L_0200caf6;
    FIELD_AT(v5, u16, 6) = 0x4000;
    {
        s32 a1 = 0;
        s32 a2 = 8;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_CommitPositionAndActivate(12, a1, a2);
    }
    t18 = Value1(Object_GetById, 12);
    Call2(ObjectDispatch_SetSingleChildFlags, t18, 1);
    Call1(Battle_WaitMode0, 5);
    Call3(ObjectMotion_CommitPositionAndActivate, 12, 0, 8);
    Call1(Battle_WaitMode0, 5);
    Call3(ObjectMotion_CommitPositionAndActivate, 12, 0, 8);
    Battle_WaitMode0(5);
    Call3(ObjectMotion_CommitPositionAndActivate, 12, 0, 8);
    Call1(Battle_WaitMode0, 5);
    Func_0200528c(5, 0x100, 30);
    Call3(Func_0200525c, 0x2005, 0, 0);
    Func_0200528c(10, 0x101, 30);
    Call3(ObjectMotion_ArmCallback, 10, 0x3000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 0, 24);
    Call3(ObjectMotion_ArmCallback, 10, 0x3000, 0);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call1(Battle_WaitMode0, 20);
    Call2(UiText_OpenMessageAtObject, 0x200a, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0x3000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0x8000, 0);
    t19 = Value1(GameFlag_Test, 7);
    if (t19 == 0)
        goto L_0200cc42;
    Call3(ObjectMotion_ArmCallback, 7, 0x8000, 0);
L_0200cc42:;
    Call1(Battle_WaitMode0, 20);
    t20 = Value2(Inventory_PromptAndSetObjectMode, 4, 0);
    if (t20 != 0)
        goto L_0200cc7c;
    Battle_WaitMode0(30);
    Call3(Func_0200525c, 0x200a, 0, 0);
    *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) = (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
    goto L_0200cca2;
L_0200cc7c:;
    Battle_WaitMode0(40);
    *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) = (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
    {
        s32 a0 = 0x200a;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Func_0200525c(a0, 0, 0);
    }
L_0200cca2:;
    Call3(ObjectMotion_ArmCallback, 5, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xa000, 0);
    t21 = Value1(GameFlag_Test, 7);
    if (t21 == 0)
        goto L_0200ccdc;
    Call3(ObjectMotion_ArmCallback, 7, 0xa000, 0);
L_0200ccdc:;
    Call1(Battle_WaitMode0, 30);
    Call2(Motion_SetVarCbAndRefresh, 10, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 0x200a, 0, 0);
    Call3(ObjectMotion_SetSpeedParameters, 10, 0x26666, 0x13333);
    t22 = Value1(Object_GetById, 10);
    *(u8 *)((t22 + 90)) = (254 & *(u8 *)((t22 + 90)));
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 0, 8);
    Call3(ObjectMotion_CommitPositionAndActivate, 10, 0, -8);
    Call1(Battle_WaitMode0, 1);
    t23 = Value1(Object_GetById, 10);
    /* FAKEMATCH: the value is ORed into the register that holds the constant, as the native does; as one |= the constant is built second. */
    b1 = 1;
    b1 |= *(u8 *)((t23 + 90));
    *(u8 *)((t23 + 90)) = b1;
    Call1(Battle_WaitMode0, 1);
    Call3(ObjectMotion_SetSpeedParameters, 10, 0xb333, 0x5999);
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0x3000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
    t24 = Value1(GameFlag_Test, 7);
    if (t24 == 0)
        goto L_0200cda2;
    Call3(ObjectMotion_ArmCallback, 7, 0x8000, 0);
L_0200cda2:;
    Call3(ObjectMotion_ArmCallback, 6, 0x8000, 0);
    Call3(Func_0200532c, 12, 3, 9);
    Call4(Motion_CamBounds, -1, -1, -1, 0);
    Call2(Object_SetModeById, 4, 1);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x4000, 0);
    Call1(Battle_WaitMode0, 20);
    *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) = (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
    {
        s32 a1 = 0x108;
        s32 a2 = 30;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(6, a1, a2);
    }
    Call3(Func_0200525c, 6, 0, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0, 0);
    Call1(Battle_WaitMode0, 30);
    {
        s32 a0 = 5;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
    Call1(Battle_WaitMode0, 20);
    Call3(Func_0200525c, 9, 0, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xa000, 0);
    {
        s32 a1 = 0xc000;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_ArmCallback(5, a1, a2);
    }
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 4);
    }
    Call1(Battle_WaitMode0, 33);
    Call3(Func_0200525c, 0x200a, 0, 0);
    {
        s32 a0 = 9;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 10);
    {
        s32 a1 = 0x108;
        s32 a2 = 30;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        Func_0200528c(5, a1, a2);
    }
    Call3(Func_0200525c, 0x2005, 0, 0);
    {
        s32 a1 = 0x3000;
        s32 a2 = 0;
        /* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier. */
        asm volatile("" : "+l"(a1), "+l"(a2));
        ObjectMotion_ArmCallback(10, a1, a2);
    }
    Call1(Battle_WaitMode0, 20);
    {
        s32 a0 = 10;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    Call2(Object_SetModeById, 4, 3);
    Call2(Object_SetModeById, 5, 3);
    Call2(Object_SetModeById, 6, 3);
    t25 = Value1(GameFlag_Test, 7);
    if (t25 == 0)
        goto L_0200cf2a;
    Call2(Object_SetModeById, 7, 3);
L_0200cf2a:;
    Call2(Object_SetModeById, 9, 3);
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_CommitPositionAndActivate, 9, 12, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0x8000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0x4000, 0);
    Call1(Battle_WaitMode0, 20);
    Call2(Motion_SetVarCbAndRefresh, 9, 2);
    Call1(Battle_WaitMode0, 10);
    Call3(Func_0200525c, 9, 0, 0);
    {
        s32 a0 = 4;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    t26 = Value1(GameFlag_Test, 7);
    if (t26 == 0)
        goto L_0200cfbc;
    Call2(Object_SetModeById, 7, 3);
L_0200cfbc:;
    Call2(Object_SetModeById, 5, 3);
    Call2(Object_SetModeById, 6, 3);
    {
        s32 a0 = 4;
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_SetModeById(a0, 3);
    }
    Call1(Battle_WaitMode0, 20);
    Call1(Battle_WaitMode0, 20);
    Call3(ObjectMotion_SetSpeedParameters, 9, 0x13333, 0x9999);
    Call2(Object_SetModeById, 9, 2);
    p1 = (s32)gPartyState;
    v5 = (p1 + 0x214);
    t27 = Value1(Object_GetById, *(s32 *)(v5));
    if (t27 == 0)
        goto L_0200d016;
    Call3(ObjectMotion_ResetAndSetPosition, 9, *(s16 *)(t27 + 10), *(s16 *)(t27 + 18));
L_0200d016:;
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 9);
    Call3(Func_0200520c, 9, 0, 0);
    Call3(ObjectMotion_SetSpeedParameters, 5, 0x13333, 0x9999);
    Call2(Object_SetModeById, 5, 2);
    t28 = Value1(Object_GetById, *(s32 *)(v5));
    if (t28 == 0)
        goto L_0200d054;
    Call3(ObjectMotion_ResetAndSetPosition, 5, *(s16 *)(t28 + 10), *(s16 *)(t28 + 18));
L_0200d054:;
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 5);
    Call3(Func_0200520c, 5, 0, 0);
    Call3(ObjectMotion_SetSpeedParameters, 6, 0x13333, 0x9999);
    Call2(Object_SetModeById, 6, 2);
    t29 = Value1(Object_GetById, *(s32 *)(v5));
    if (t29 == 0)
        goto L_0200d092;
    Call3(ObjectMotion_ResetAndSetPosition, 6, *(s16 *)(t29 + 10), *(s16 *)(t29 + 18));
L_0200d092:;
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 6);
    Call3(Func_0200520c, 6, 0, 0);
    t30 = Value1(GameFlag_Test, 7);
    if (t30 == 0)
        goto L_0200d0fa;
    Call3(ObjectMotion_SetSpeedParameters, 7, 0x13333, 0x9999);
    Call2(Object_SetModeById, 7, 2);
    t31 = Value1(Object_GetById, *(s32 *)(v5));
    if (t31 == 0)
        goto L_0200d0ea;
    goto L_0200d0dc;
L_0200d0dc:;
    Call3(ObjectMotion_ResetAndSetPosition, 7, *(s16 *)(t31 + 10), *(s16 *)(t31 + 18));
L_0200d0ea:;
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 7);
    Call3(Func_0200520c, 7, 0, 0);
L_0200d0fa:;
    p2 = (s32)gPartyState;
    {
        s32 a0 = *(s32 *)((p2 + 0x214));
        /* FAKEMATCH: the native loads the first argument before the others; the plain call loads it last. */
        asm volatile("" : "+l"(a0));
        Object_AttachWorkTargetToObject(a0, 1);
    }
    Func_020051b4();
}
