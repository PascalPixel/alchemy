#include "types.h"
#include "call.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Battle_WaitMode0();
s32 Func_02001344();
s32 Func_02001418();
s32 Func_02001454();
s32 Func_020051ac();
s32 Func_020051b4();
s32 Func_0200520c();
s32 Func_0200524c();
s32 Func_0200525c();
s32 Func_0200528c();
s32 Func_02005294();
s32 Func_020052a4();
s32 Func_020052b4();
s32 Func_020052bc();
s32 Func_020052c4();
s32 Func_02005324();
s32 Func_02005334();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 Event_AskYesNoWithActorAnimation();
s32 Motion_CamBounds();
s32 Motion_SetVarCbAndRefresh();
s32 ObjectDispatch_SetSingleChildFlags();
s32 ObjectMotion_ArmCallback();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 ObjectMotion_CommitPositionAndActivate();
s32 ObjectMotion_EnableActionAndResetMotion();
s32 ObjectMotion_Launch();
s32 ObjectMotion_OffsetPositionAndReset();
s32 ObjectMotion_ResetAndSetPosition();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_SetVariantCallback();
s32 Object_AttachWorkTargetToObject();
s32 Object_GetById();
s32 Object_LinkObjectAndSetCallback();
s32 Object_SetModeById();
s32 UiText_OpenMessageAtObject();
extern u8 MsgGaroh14[];
extern u8 gPartyState[];

void Func_02002694(void) {
  s32 v5;
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
  s32 b1;
  s32 p1;
  s32 p2;

  Call1(GameFlag_SetBit, 0x91e);
  Func_020051ac();
  Call1(Func_02005324, 0);
  Call1(Func_0200524c, ((s32)MsgGaroh14));
  Call3(Func_0200520c, 10, 0x1480000, 0x280000);
  Call3(Func_0200520c, 8, 0x1480000, 0x280000);
  Call3(ObjectMotion_ArmCallback, 10, 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0x4000, 0);
  Call3(ObjectMotion_SetPositionAndReset, 4, 0x140, 224);
  Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 0x8009, 0, 0);
  Call4(Func_02005334, 9, -16, -8, 0xc000);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 9);
  Call1(Battle_WaitMode0, 30);
  Call2(Func_020052a4, 0x10000, 0x2000);
  Call4(Motion_CamBounds, 0x1480000, -1, 0x980000, 1);
  Call3(ObjectMotion_ArmCallback, 9, 0xa000, 0);
  Call1(Battle_WaitMode0, 25);
  Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
  Call1(Battle_WaitMode0, 25);
  Call3(ObjectMotion_ArmCallback, 9, 0xa000, 0);
  Call1(Battle_WaitMode0, 25);
  Call3(ObjectMotion_ArmCallback, 9, 0xc000, 0);
  Call1(Battle_WaitMode0, 25);
  Call3(Func_0200525c, 9, 0, 0);
  Call3(ObjectMotion_ArmCallback, 4, 0xa000, 0);
  {
    s32 a0 = 9;
    s32 a1 = 0x2000;
    /* FAKEMATCH: the native loads the third argument, then the first, then
     * finishes the second; the plain call loads the first earlier and the third
     * last (differ 2). */
    __asm__("" : "+l"(a0), "+l"(a1));
    Call3(ObjectMotion_ArmCallback, a0, a1, 0);
  }
  {
    s32 a0 = 4;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Func_020052bc(a0, 1);
  }
  Func_020052b4();
  Call2(Func_020052a4, 0x18000, 0x3000);
  UiText_OpenMessageAtObject(9, 0);
  t1 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
  if (t1 != 0)
    goto L_0200a818;
  Call1(Battle_WaitMode0, 20);
  Func_0200528c(9, 0x109, 30);
  Call3(Func_0200525c, 9, 0, 0);
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  goto L_0200a846;
L_0200a818:;
  Call1(Battle_WaitMode0, 30);
  Func_0200528c(9, 0x109, 30);
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  Call3(Func_0200525c, 9, 0, 0);
L_0200a846:;
  Call3(Func_0200525c, 8, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 4, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0xc000, 0);
  Call1(Battle_WaitMode0, 20);
  Call4(Func_02005334, 5, 16, 0, 0xc000);
  Call4(Func_02005334, 6, 32, 0, 0xc000);
  t2 = Value1(GameFlag_Test, 7);
  if (t2 == 0)
    goto L_0200a8b2;
  Call4(Func_02005334, 7, 48, -8, 0xa000);
L_0200a8b2:;
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 6);
  Call1(Battle_WaitMode0, 20);
  {
    s32 a1 = 0x100;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(5, a1, a2);
  }
  Call3(Func_0200525c, 5, 0, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0xa000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(ObjectMotion_ArmCallback, 6, 0xe000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(ObjectMotion_ArmCallback, 6, 0xa000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(ObjectMotion_ArmCallback, 6, 0xc000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 6, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(9, a1, a2);
  }
  Call3(ObjectMotion_ArmCallback, 9, 0xa000, 0);
  Call1(Battle_WaitMode0, 25);
  Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
  Call1(Battle_WaitMode0, 25);
  Call3(ObjectMotion_ArmCallback, 9, 0xa000, 0);
  Call1(Battle_WaitMode0, 25);
  Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
  Call1(Battle_WaitMode0, 25);
  Call3(ObjectMotion_ArmCallback, 9, 0x2000, 0);
  Call1(Battle_WaitMode0, 25);
  Call3(Func_0200525c, 9, 0, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0, 0);
  Call1(Battle_WaitMode0, 40);
  Call3(ObjectMotion_ArmCallback, 6, 0xa000, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0xa000, 0);
  Call1(Battle_WaitMode0, 25);
  Call2(Object_SetModeById, 6, 3);
  {
    s32 a0 = 5;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 9, 0, 0);
  Call3(Func_0200525c, 10, 0, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
  Call4(Motion_CamBounds, 0x1480000, -1, 0x900000, 1);
  Func_020052b4();
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200520c, 11, 0x1480000, 0x880000);
  t3 = Value1(Object_GetById, 11);
  Call2(ObjectDispatch_SetSingleChildFlags, t3, 0);
  Call1(Func_02001418, 11);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 5, 0, 0);
  Call3(ObjectMotion_SetSpeedParameters, 8, 0xb333, 0x5999);
  Call3(ObjectMotion_SetSpeedParameters, 10, 0xb333, 0x5999);
  Call3(Func_0200520c, 10, 0x1480000, 0x800000);
  Call1(Battle_WaitMode0, 10);
  Call3(ObjectMotion_CommitPositionAndActivate, 10, 0, 16);
  Call3(Func_0200520c, 8, 0x1480000, 0x800000);
  Call1(Battle_WaitMode0, 10);
  Call3(ObjectMotion_OffsetPositionAndReset, 8, 0, 16);
  Call3(ObjectMotion_CommitPositionAndActivate, 10, 0, 16);
  Call1(Battle_WaitMode0, 10);
  Call1(Func_02001454, 11);
  Call3(Func_0200520c, 11, 0, 0);
  Call2(Object_AttachWorkTargetToObject, 10, 1);
  Call1(Battle_WaitMode0, 20);
  Call3(ObjectMotion_OffsetPositionAndReset, 8, 0, 24);
  Call3(ObjectMotion_CommitPositionAndActivate, 10, 0, 24);
  Call3(ObjectMotion_CommitPositionAndActivate, 8, 16, 8);
  Call3(ObjectMotion_ArmCallback, 8, 0x4000, 0);
  Call1(Battle_WaitMode0, 20);
  Call4(Motion_CamBounds, 0x1480000, -1, 0xd00000, 1);
  Func_020052b4();
  Call1(Battle_WaitMode0, 20);
  Call2(Motion_SetVarCbAndRefresh, 8, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 8, 0, 0);
  Call2(ObjectMotion_SetVariantCallback, 9, 2);
  Call2(ObjectMotion_SetVariantCallback, 6, 2);
  Call2(ObjectMotion_SetVariantCallback, 5, 2);
  t4 = Value1(GameFlag_Test, 7);
  if (t4 == 0)
    goto L_0200ab98;
  Call2(ObjectMotion_SetVariantCallback, 7, 2);
L_0200ab98:;
  Call2(Motion_SetVarCbAndRefresh, 4, 2);
  Call1(Battle_WaitMode0, 20);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 4);
  }
  Call1(Battle_WaitMode0, 33);
  Call3(Func_0200525c, 10, 0, 0);
  Func_0200528c(6, 0x100, 30);
  Call3(Func_0200525c, 6, 0, 0);
  Call2(ObjectMotion_SetVariantCallback, 8, 3);
  Call2(Func_02005294, 8, 0x102);
  Call1(Battle_WaitMode0, 40);
  Call3(Func_0200525c, 8, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 9, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 9, 0, 0);
  Call3(ObjectMotion_Launch, 8, 6, 15);
  Call3(ObjectMotion_Launch, 8, 6, 23);
  {
    s32 a1 = 0x6000;
    s32 a2 = 0;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    ObjectMotion_ArmCallback(8, a1, a2);
  }
  Call1(Battle_WaitMode0, 20);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 4);
  }
  Call1(Battle_WaitMode0, 33);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 0;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(4, a1, a2);
  }
  {
    s32 a1 = 0x101;
    s32 a2 = 0;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(5, a1, a2);
  }
  Call3(Func_0200528c, 6, 0x101, 0);
  t5 = Value1(GameFlag_Test, 7);
  if (t5 == 0)
    goto L_0200ac86;
  Call3(Func_0200528c, 7, 0x101, 0);
L_0200ac86:;
  Func_0200528c(9, 0x101, 60);
  Call2(Motion_SetVarCbAndRefresh, 10, 2);
  Call1(Battle_WaitMode0, 10);
  UiText_OpenMessageAtObject(10, 0);
  t6 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
  if (t6 == 0)
    goto L_0200ace6;
  Call1(Battle_WaitMode0, 40);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 4);
  }
  Call1(Battle_WaitMode0, 33);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Func_0200525c(a0, 0, 0);
  }
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  goto L_0200ad16;
L_0200ace6:;
  Call1(Battle_WaitMode0, 30);
  Call2(Motion_SetVarCbAndRefresh, 10, 2);
  Call1(Battle_WaitMode0, 10);
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  Call3(Func_0200525c, 10, 0, 0);
L_0200ad16:;
  Call2(Motion_SetVarCbAndRefresh, 8, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 8, 0, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0, 0);
  Call1(Battle_WaitMode0, 30);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(5, a1, a2);
  }
  Call3(Func_0200525c, 5, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 10, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(ObjectMotion_ArmCallback, 8, 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0x4000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 10, 0, 0);
  Func_0200528c(6, 0x106, 30);
  Call3(ObjectMotion_ArmCallback, 6, 0xa000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 6, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 4);
  }
  Call1(Battle_WaitMode0, 33);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(10, a1, a2);
  }
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 0;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(4, a1, a2);
  }
  {
    s32 a1 = 0x101;
    s32 a2 = 0;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(5, a1, a2);
  }
  Call3(Func_0200528c, 6, 0x101, 0);
  t7 = Value1(GameFlag_Test, 7);
  if (t7 == 0)
    goto L_0200ae32;
  Call3(Func_0200528c, 7, 0x101, 0);
L_0200ae32:;
  {
    s32 a1 = 0x101;
    s32 a2 = 60;
    Func_0200528c(9, a1, a2);
  }
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call2(UiText_OpenMessageAtObject, 10, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0x2000, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0x8000, 0);
  t8 = Value1(GameFlag_Test, 7);
  if (t8 == 0)
    goto L_0200ae8e;
  Call3(ObjectMotion_ArmCallback, 7, 0x8000, 0);
L_0200ae8e:;
  Call1(Battle_WaitMode0, 20);
  t9 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
  if (t9 == 0)
    goto L_0200aed2;
  Call1(Battle_WaitMode0, 40);
  Call2(Motion_SetVarCbAndRefresh, 5, 2);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 5, 0, 0);
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  goto L_0200af08;
L_0200aed2:;
  Call1(Battle_WaitMode0, 30);
  {
    s32 a0 = 5;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 20);
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  Call3(Func_0200525c, 5, 0, 0);
L_0200af08:;
  Call3(ObjectMotion_ArmCallback, 10, 0xe000, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0x6000, 0);
  Call1(Battle_WaitMode0, 30);
  Call3(Func_0200525c, 8, 0, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0xc000, 0);
  t10 = Value1(GameFlag_Test, 7);
  if (t10 == 0)
    goto L_0200af6a;
  Call3(ObjectMotion_ArmCallback, 7, 0xa000, 0);
L_0200af6a:;
  Call1(Battle_WaitMode0, 30);
  Call2(Motion_SetVarCbAndRefresh, 6, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 6, 0, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0x4000, 0);
  Call1(Battle_WaitMode0, 30);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a0 = 9;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 9, 0, 0);
  {
    s32 a1 = 0x100;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(10, a1, a2);
  }
  Call3(Func_0200525c, 10, 0, 0);
  Call2(Object_SetModeById, 4, 3);
  Call2(Object_SetModeById, 5, 3);
  Call2(Object_SetModeById, 6, 3);
  t11 = Value1(GameFlag_Test, 7);
  if (t11 == 0)
    goto L_0200affc;
  Call2(Object_SetModeById, 7, 3);
L_0200affc:;
  Call2(Object_SetModeById, 9, 3);
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 20);
  Call3(ObjectMotion_ArmCallback, 9, 0x2000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 9, 0, 0);
  {
    s32 a1 = 0xe000;
    s32 a2 = 0;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    ObjectMotion_ArmCallback(9, a1, a2);
  }
  Call1(Battle_WaitMode0, 30);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(8, a1, a2);
  }
  Call3(Func_0200525c, 8, 0, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0xe000, 0);
  Call1(Battle_WaitMode0, 30);
  Call2(Motion_SetVarCbAndRefresh, 5, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 5, 0, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0x3000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a1 = 0x109;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(6, a1, a2);
  }
  Call3(Func_0200525c, 6, 0, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0x4000, 0);
  Call1(Battle_WaitMode0, 20);
  Call2(Motion_SetVarCbAndRefresh, 8, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 8, 0, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 9, 0, 0);
  {
    s32 a1 = 0xe000;
    s32 a2 = 0;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    ObjectMotion_ArmCallback(9, a1, a2);
  }
  Call1(Battle_WaitMode0, 30);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(5, a1, a2);
  }
  Call3(Func_0200525c, 5, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 10, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a0 = 9;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 4);
  }
  Call1(Battle_WaitMode0, 33);
  Call3(Func_0200525c, 9, 0, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0x4000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(ObjectMotion_CommitPositionAndActivate, 8, 0, 16);
  Call1(Battle_WaitMode0, 10);
  Call3(ObjectMotion_Launch, 8, 6, 15);
  Call3(ObjectMotion_Launch, 8, 6, 23);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(6, a1, a2);
  }
  Call3(Func_0200525c, 6, 0, 0);
  Call3(ObjectMotion_SetSpeedParameters, 10, 0x28000, 0x14000);
  Call3(ObjectMotion_SetSpeedParameters, 8, 0x28000, 0x14000);
  Call3(ObjectMotion_ArmCallback, 10, 0, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(ObjectMotion_CommitPositionAndActivate, 10, 8, 0);
  Call3(ObjectMotion_Launch, 8, 4, 0);
  t12 = Value1(Object_GetById, 10);
  v5 = 254;
  *(u8 *)((t12 + 90)) = (v5 & *(u8 *)((t12 + 90)));
  Call3(ObjectMotion_OffsetPositionAndReset, 10, -8, 0);
  t13 = Value1(Object_GetById, 8);
  v5 = (v5 & *(u8 *)((t13 + 90)));
  *(u8 *)((t13 + 90)) = v5;
  Call3(ObjectMotion_CommitPositionAndActivate, 8, 16, 0);
  Call1(Battle_WaitMode0, 1);
  t14 = Value1(Object_GetById, 8);
  /* FAKEMATCH: the value is ORed into the register that holds the constant, as
   * the native does; as one |= the constant is built second. */
  b1 = 1;
  b1 |= *(u8 *)((t14 + 90));
  *(u8 *)((t14 + 90)) = b1;
  Call2(Object_SetModeById, 8, 5);
  Call1(Battle_WaitMode0, 20);
  Call3(ObjectMotion_SetSpeedParameters, 8, 0x14ccc, 0xa666);
  Call3(ObjectMotion_SetSpeedParameters, 10, 0xb333, 0x5999);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 4);
  }
  Call1(Battle_WaitMode0, 33);
  Call1(Battle_WaitMode0, 10);
  {
    s32 a0 = 8;
    /* FAKEMATCH: the native loads the first argument after the second is
     * finished; the plain call loads it one instruction earlier (differ 2). */
    __asm__("" : "+l"(a0));
    Call3(Func_0200528c, a0, 0x102, 50);
  }
  Call2(Object_SetModeById, 8, 1);
  Call3(ObjectMotion_Launch, 8, 6, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0x8000, 0);
  Call1(Battle_WaitMode0, 30);
  Call2(Func_02005294, 8, 0x102);
  {
    s32 a0 = 8;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 15);
  Call2(Func_02005294, 8, 0x102);
  {
    s32 a0 = 8;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 20);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(8, a1, a2);
  }
  Call3(Func_0200525c, 8, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 4);
  }
  Call1(Battle_WaitMode0, 33);
  Call3(Func_0200525c, 10, 0, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
  Call1(Battle_WaitMode0, 45);
  Call3(ObjectMotion_ArmCallback, 8, 0x8000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 8, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 4);
  }
  Call1(Battle_WaitMode0, 33);
  Call3(Func_0200525c, 10, 0, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
  Call1(Battle_WaitMode0, 45);
  Call3(ObjectMotion_ArmCallback, 10, 0, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a0 = 8;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 20);
  {
    s32 a1 = 0x4000;
    s32 a2 = 0;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    ObjectMotion_ArmCallback(8, a1, a2);
  }
  Call1(Battle_WaitMode0, 35);
  {
    s32 a0 = 8;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 8, 0, 0);
  Call2(Object_SetModeById, 4, 3);
  Call2(Object_SetModeById, 5, 3);
  Call2(Object_SetModeById, 6, 3);
  {
    s32 a0 = 9;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 20);
  Call2(Object_LinkObjectAndSetCallback, 4, 8);
  Call2(Object_LinkObjectAndSetCallback, 9, 8);
  Call2(Object_LinkObjectAndSetCallback, 5, 8);
  Call2(Object_LinkObjectAndSetCallback, 6, 8);
  Call2(Object_LinkObjectAndSetCallback, 10, 8);
  t15 = Value1(GameFlag_Test, 7);
  if (t15 == 0)
    goto L_0200b46e;
  Call2(Object_LinkObjectAndSetCallback, 7, 8);
L_0200b46e:;
  Call3(ObjectMotion_SetPositionAndReset, 8, 0x178, 208);
  Call3(ObjectMotion_CommitPositionAndActivate, 8, 0, 16);
  Call3(ObjectMotion_CommitPositionAndActivate, 8, -16, 24);
  Call3(ObjectMotion_CommitPositionAndActivate, 8, -32, 16);
  Call3(ObjectMotion_CommitPositionAndActivate, 8, 0, 48);
  Call1(Battle_WaitMode0, 20);
  Call1(ObjectMotion_EnableActionAndResetMotion, 4);
  Call1(ObjectMotion_EnableActionAndResetMotion, 9);
  Call1(ObjectMotion_EnableActionAndResetMotion, 5);
  Call1(ObjectMotion_EnableActionAndResetMotion, 6);
  Call1(ObjectMotion_EnableActionAndResetMotion, 10);
  t16 = Value1(GameFlag_Test, 7);
  if (t16 == 0)
    goto L_0200b4da;
  Call1(ObjectMotion_EnableActionAndResetMotion, 7);
L_0200b4da:;
  Call3(Func_0200520c, 8, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 10, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 10, 0, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 4, 0xc000, 0);
  t17 = Value1(GameFlag_Test, 7);
  if (t17 == 0)
    goto L_0200b542;
  Call3(ObjectMotion_ArmCallback, 7, 0xa000, 0);
L_0200b542:;
  Call1(Battle_WaitMode0, 30);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 10, 0, 0);
  Func_0200528c(9, 0x102, 30);
  Call3(Func_0200525c, 9, 0, 0);
  Func_0200528c(10, 0x108, 30);
  Call3(Func_0200525c, 10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_0200528c(6, a1, a2);
  }
  Call3(Func_0200525c, 6, 0, 0);
  Func_0200528c(10, 0x109, 30);
  Call3(Func_0200525c, 10, 0, 0);
  Func_0200528c(9, 0x100, 30);
  Call3(Func_0200525c, 9, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 10);
  Call2(Motion_SetVarCbAndRefresh, 5, 2);
  Call1(Battle_WaitMode0, 10);
  Call3(ObjectMotion_ArmCallback, 5, 0x4000, 0);
  Call1(Battle_WaitMode0, 30);
  Call3(Func_0200525c, 5, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 4);
  }
  Call1(Battle_WaitMode0, 33);
  Call3(Func_0200525c, 10, 0, 0);
  Func_0200528c(5, 0x101, 50);
  Call3(ObjectMotion_ArmCallback, 5, 0xb000, 0);
  Call1(Battle_WaitMode0, 30);
  Call3(ObjectMotion_ArmCallback, 9, 0x2000, 0);
  Call1(Battle_WaitMode0, 20);
  Call3(Func_0200525c, 9, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 10);
  Call3(Func_0200525c, 10, 0, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0xe000, 0);
  Call1(Battle_WaitMode0, 20);
  Call2(Object_SetModeById, 4, 3);
  Call2(Object_SetModeById, 5, 3);
  Call2(Object_SetModeById, 6, 3);
  {
    s32 a0 = 9;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 20);
  Call3(ObjectMotion_ArmCallback, 9, 0x2000, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0x8000, 0);
  t18 = Value1(GameFlag_Test, 7);
  if (t18 == 0)
    goto L_0200b6f4;
  Call3(ObjectMotion_ArmCallback, 7, 0x8000, 0);
L_0200b6f4:;
  Call1(Battle_WaitMode0, 20);
  Call2(Object_SetModeById, 5, 3);
  Call2(Object_SetModeById, 6, 3);
  t19 = Value1(GameFlag_Test, 7);
  if (t19 == 0)
    goto L_0200b71c;
  Call2(Object_SetModeById, 7, 3);
L_0200b71c:;
  Call2(Object_SetModeById, 9, 3);
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 20);
  {
    s32 a0 = 4;
    /* FAKEMATCH: the native loads the first argument before the others; the
     * plain call loads it last. */
    asm volatile("" : "+l"(a0));
    Object_SetModeById(a0, 3);
  }
  Call1(Battle_WaitMode0, 20);
  Call1(Battle_WaitMode0, 10);
  Call3(ObjectMotion_SetSpeedParameters, 9, 0x13333, 0x9999);
  Call2(Object_SetModeById, 9, 2);
  p1 = (s32)gPartyState;
  v5 = (p1 + 0x214);
  t20 = Value1(Object_GetById, *(s32 *)(v5));
  if (t20 == 0)
    goto L_0200b77a;
  Call3(ObjectMotion_ResetAndSetPosition, 9, *(s16 *)(t20 + 10),
        *(s16 *)(t20 + 18));
L_0200b77a:;
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 9);
  Call3(Func_0200520c, 9, 0, 0);
  Call3(ObjectMotion_SetSpeedParameters, 5, 0x13333, 0x9999);
  Call2(Object_SetModeById, 5, 2);
  t21 = Value1(Object_GetById, *(s32 *)(v5));
  if (t21 == 0)
    goto L_0200b7b8;
  Call3(ObjectMotion_ResetAndSetPosition, 5, *(s16 *)(t21 + 10),
        *(s16 *)(t21 + 18));
L_0200b7b8:;
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 5);
  Call3(Func_0200520c, 5, 0, 0);
  Call3(ObjectMotion_SetSpeedParameters, 6, 0x13333, 0x9999);
  Call2(Object_SetModeById, 6, 2);
  t22 = Value1(Object_GetById, *(s32 *)(v5));
  if (t22 == 0)
    goto L_0200b7f6;
  Call3(ObjectMotion_ResetAndSetPosition, 6, *(s16 *)(t22 + 10),
        *(s16 *)(t22 + 18));
L_0200b7f6:;
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 6);
  Call3(Func_0200520c, 6, 0, 0);
  t23 = Value1(GameFlag_Test, 7);
  if (t23 == 0)
    goto L_0200b84e;
  Call3(ObjectMotion_SetSpeedParameters, 7, 0x13333, 0x9999);
  Call2(Object_SetModeById, 7, 2);
  t24 = Value1(Object_GetById, *(s32 *)(v5));
  if (t24 == 0)
    goto L_0200b83e;
  Call3(ObjectMotion_ResetAndSetPosition, 7, *(s16 *)(t24 + 10),
        *(s16 *)(t24 + 18));
L_0200b83e:;
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 7);
  Call3(Func_0200520c, 7, 0, 0);
L_0200b84e:;
  p2 = (s32)gPartyState;
  {
    s32 a0 = *(s32 *)((p2 + 0x214));
    Object_AttachWorkTargetToObject(a0, 1);
  }
  Func_02001344();
  Call1(Func_020052c4, 11);
  Func_020051b4();
}
