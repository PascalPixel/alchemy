// Draft of overlay 6ab's Func_02000e80, translated from its listing: differ
// 320 of 563. Left: instruction order and register choice.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 FieldEvent_WaitFrames();
s32 Func_020041e4();
s32 Func_020041ec();
s32 Func_02004204();
s32 Func_0200420c();
s32 Func_0200423c();
s32 Func_02004244();
s32 Func_02004264();
s32 Func_02004294();
s32 Func_020042ac();
s32 Func_020042dc();
s32 Func_020042e4();
s32 Func_020043a4();
s32 Func_020043b4();
s32 Func_020043bc();
s32 Func_020043c4();
s32 Func_020043d4();
s32 Func_02004404();
s32 Func_0200440c();
s32 Func_02004414();
s32 Func_0200444c();
s32 Func_0200445c();
s32 Func_0200446c();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 ObjectMotion_ArmCallback();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
s32 Runtime_AllocateHeapBlockFar();
s32 Runtime_ReleaseHeapBlock();
s32 Scheduler_AddOrUpdateCallback();
s32 Scheduler_RemoveCallbackFar();
s32 UiText_ShowPositionedMessageAndWait();
s32 VramCache_Load();
s32 WaitFrames();
extern u8 Data_02004cec[];
extern u8 Func_02001424[];
extern u8 MsgMarsStar09[];
extern u8 gPartyState[];

void Func_02000e80(void) {
  s32 v_r8, v_r7, v_r0, v_r5, v_r6, v_r11, v_r9, v_r10, t0, t7;
  s32 t8, t9, t10, t11, t12, t14, t18, t20, t22, t24;
  s32 t26, t28, t31, t33, t36, t38, t39, t40, t41, t44;
  s32 t45, t46, t47, t49, t53, t60, t61, t62, t63, t64;
  s32 t65, t67, t71, t73, t75, t81, t91, t107, t112, t115;
  s32 t117, t120, t122, t125, t127, t128, t129, t130, t133, t134;
  s32 t135, t136, t138;

  t0 = Value1((s32 (*)())GameFlag_Test, 0xa9e);
  v_r8 = t0;
  if (v_r8 != 0)
    goto L_02008ea2;
  goto L_0200905c;
L_02008ea2:
  Func_020042dc();
  Call1((void (*)())Func_0200444c, 0);
  Call2((void (*)())UiText_ShowPositionedMessageAndWait, (s32)MsgMarsStar09, 1);
  Call1((void (*)())Func_0200446c, 78);
  Call1((void (*)())WaitFrames, 1);
  Call1((void (*)())Func_0200446c, 182);
  t7 = Value4((s32 (*)())Func_020041e4, 0x1e9, 0x2a80000, 0x80000, 0x930000);
  *(s32 *)(s32)Data_02004cec = t7;
  v_r0 = t7;
  v_r7 = (s32)Data_02004cec;
  if (v_r0 == 0)
    goto L_02008f48;
  t8 = FIELD_AT(v_r0, s32, 80);
  t9 = FIELD_AT(t8, u8, 5);
  t10 = FIELD_AT(t8, u8, 9);
  FIELD_AT(t8, u8, 5) = (-0x21 & t9);
  FIELD_AT(t8, u8, 9) = (((15 & t10) & -13) | 8);
  *(u8 *)(v_r0 + 85) = 0;
  *(u8 *)(v_r0 + 92) = 1;
  FIELD_AT(t8, u8, 26) = 0;
  FIELD_AT(t8, u8, 27) = 0;
  t11 = *(u8 *)(v_r0 + 35);
  *(u8 *)(v_r0 + 35) = (2 | t11);
  t12 = Value2((s32 (*)())Runtime_AllocateHeapBlockFar, 68, 0x608);
  Call1((void (*)())Func_020042ac, 247);
  t14 = FIELD_AT(t8, u8, 16);
  v_r5 = (t12 + 0x400);
  Call3((void (*)())VramCache_Load, t14, 128, v_r5);
  Call4((void (*)())Runtime_ReleaseHeapBlock, 68, 0, 0, 0);
L_02008f48:
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  v_r6 = ((s32)gPartyState + 0x214);
  t18 = *(s32 *)v_r6;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, t18, 0xcccc, 0x6666);
  t20 = *(s32 *)v_r6;
  Call3((void (*)())ObjectMotion_SetPositionAndReset, t20, 0x2a8, 186);
  t22 = *(s32 *)v_r6;
  Call3((void (*)())ObjectMotion_ArmCallback, t22, 0xc000, 80);
  t24 = *(s32 *)v_r6;
  Call3((void (*)())Func_020043b4, t24, 0x101, 40);
  t26 = *(s32 *)v_r7;
  v_r0 = t26;
  if (v_r0 == 0)
    goto L_02008fe6;
  *(u8 *)(v_r0 + 85) = 3;
  FIELD_AT(v_r0, s32, 40) = 0x20000;
  FIELD_AT(v_r0, s32, 48) = 0x20000;
  FIELD_AT(v_r0, s32, 52) = 0x10000;
  Call4((void (*)())Func_02004204, v_r0, 0x2a00000, 0, 0xa00000);
  t28 = *(s32 *)v_r7;
  Call1((void (*)())Func_0200420c, t28);
  Call1((void (*)())Func_0200446c, 135);
  t31 = *(s32 *)v_r7;
  FIELD_AT(t31, s32, 52) = 0x8000;
  FIELD_AT(t31, s32, 40) = 0x40000;
  FIELD_AT(t31, s32, 48) = 0x10000;
  Call4((void (*)())Func_02004204, t31, 0x2980000, 0, 0x9e0000);
  t33 = *(s32 *)v_r7;
  Call1((void (*)())Func_0200420c, t33);
L_02008fe6:
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  t36 = *(s32 *)v_r6;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, t36, 0x19999, 0xcccc);
  t38 = *(s32 *)v_r6;
  t39 = Value1((s32 (*)())Object_GetById, t38);
  t40 = *(u8 *)(t39 + 90);
  *(u8 *)(t39 + 90) = (254 & t40);
  t41 = *(s32 *)v_r6;
  Call3((void (*)())ObjectMotion_SetPositionAndReset, t41, 0x29e, 170);
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  t44 = *(s32 *)v_r6;
  t45 = Value1((s32 (*)())Object_GetById, t44);
  t46 = *(u8 *)(t45 + 90);
  *(u8 *)(t45 + 90) = (1 | t46);
  t47 = *(s32 *)v_r7;
  if (t47 == 0)
    goto L_02009038;
  Func_020041ec();
L_02009038:
  t49 = *(s32 *)v_r6;
  Call2((void (*)())Func_020043a4, t49, 0x4000);
  Func_0200445c();
  Func_020042e4();
  goto L_020093b0;
L_0200905c:
  t53 = *(s32 *)0x3000080;
  Func_020042dc();
  Call1((void (*)())Func_0200444c, 0);
  Call2((void (*)())UiText_ShowPositionedMessageAndWait, (s32)MsgMarsStar09, 1);
  Call1((void (*)())Func_0200446c, 78);
  Call1((void (*)())WaitFrames, 1);
  Call1((void (*)())Func_0200446c, 182);
  t60 = Value4((s32 (*)())Func_020041e4, 0x1e9, 0x2a80000, 0x80000, 0x930000);
  *(s32 *)(s32)Data_02004cec = t60;
  v_r0 = t60;
  v_r9 = (s32)Data_02004cec;
  v_r11 = t53;
  if (v_r0 == 0)
    goto L_02009112;
  t61 = FIELD_AT(v_r0, s32, 80);
  t62 = FIELD_AT(t61, u8, 5);
  FIELD_AT(t61, u8, 26) = v_r8;
  FIELD_AT(t61, u8, 27) = v_r8;
  t63 = FIELD_AT(t61, u8, 9);
  FIELD_AT(t61, u8, 5) = (-0x21 & t62);
  FIELD_AT(t61, u8, 9) = (((15 & t63) & -13) | 8);
  *(u8 *)(v_r0 + 85) = v_r8;
  *(u8 *)(v_r0 + 92) = 1;
  t64 = *(u8 *)(v_r0 + 35);
  *(u8 *)(v_r0 + 35) = (2 | t64);
  t65 = Value2((s32 (*)())Runtime_AllocateHeapBlockFar, 68, 0x608);
  Call1((void (*)())Func_020042ac, 247);
  t67 = FIELD_AT(t61, u8, 16);
  v_r5 = (t65 + 0x400);
  Call3((void (*)())VramCache_Load, t67, 128, v_r5);
  Call4((void (*)())Runtime_ReleaseHeapBlock, 68, 0, 0, 0);
L_02009112:
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  v_r7 = (0x214 + (s32)gPartyState);
  t71 = *(s32 *)v_r7;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, t71, 0xcccc, 0x6666);
  t73 = *(s32 *)v_r7;
  Call3((void (*)())ObjectMotion_SetPositionAndReset, t73, 0x2a8, 186);
  t75 = *(s32 *)v_r7;
  Call3((void (*)())ObjectMotion_ArmCallback, t75, 0xc000, 40);
  Call1((void (*)())Func_0200446c, 0);
  Call1((void (*)())Func_0200446c, 141);
  Call3((void (*)())Func_02004264, 0x20000, 0x20000, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  t81 = *(s32 *)v_r7;
  Call2((void (*)())Func_020043bc, t81, 0x102);
  Call3((void (*)())Func_02004264, 0x10000, 0x10000, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Func_0200440c, 0x10000, 0);
  Call4((void (*)())Func_02004404, 0x10006, 0, 0, 0);
  Call1((void (*)())Func_02004414, 80);
  Call1((void (*)())FieldEvent_WaitFrames, 80);
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02001424, 0x480);
  Call2((void (*)())Func_020043c4, 0x4cccc, 0x9999);
  t91 = Value4((s32 (*)())Camera_MoveWithinMapBounds, 0x2a80000, -1, 0x1e00000,
               1);
  Call2((void (*)())Func_020043d4, t91, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 80);
  Call3((void (*)())Func_02004264, -1, -1, 0xe666);
  Call1((void (*)())Func_0200446c, 0x186);
  Call4((void (*)())Func_02004404, 0x7fff, 0, 0, 0);
  Call1((void (*)())Func_02004414, 80);
  Call1((void (*)())WaitFrames, 60);
  Call1((void (*)())Scheduler_RemoveCallbackFar, (s32)Func_02001424);
  Call1((void (*)())WaitFrames, 20);
  Call3((void (*)())Func_02004264, 0, 0, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call6((void (*)())Func_0200423c, 65, 65, 9, 21, 38, 24);
  Call6((void (*)())Func_02004244, 65, 65, 9, 21, 38, 24);
  Call6((void (*)())Func_02004244, 76, 64, 9, 22, 38, 22);
  Call6((void (*)())Func_02004244, 76, 64, 9, 22, 22, 9);
  t107 = *(s16 *)(0x1fa + (s32)gPartyState);
  Call2((void (*)())Func_02004294, (t107 + 19), v_r11);
  *(u16 *)v_r11 = v_r8;
  Call4((void (*)())Func_02004404, 0x10000, 0, v_r11, v_r8);
  Call1((void (*)())Func_02004414, 120);
  Call1((void (*)())WaitFrames, 160);
  t112 =
      Value4((s32 (*)())Camera_MoveWithinMapBounds, 0x2a80000, -1, 0xba0000, 1);
  Call2((void (*)())Func_020043d4, t112, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  t115 = *(s32 *)v_r9;
  v_r0 = t115;
  v_r10 = (s32)gPartyState;
  if (v_r0 == 0)
    goto L_0200932e;
  *(u8 *)(v_r0 + 85) = 3;
  FIELD_AT(v_r0, s32, 40) = 0x20000;
  FIELD_AT(v_r0, s32, 48) = 0x20000;
  FIELD_AT(v_r0, s32, 52) = 0x10000;
  Call4((void (*)())Func_02004204, v_r0, 0x2a00000, 0, 0xa00000);
  t117 = *(s32 *)v_r9;
  Call1((void (*)())Func_0200420c, t117);
  Call1((void (*)())Func_0200446c, 135);
  t120 = *(s32 *)v_r9;
  FIELD_AT(t120, s32, 52) = 0x8000;
  FIELD_AT(t120, s32, 40) = 0x40000;
  FIELD_AT(t120, s32, 48) = 0x10000;
  Call4((void (*)())Func_02004204, t120, 0x2980000, 0, 0x9e0000);
  t122 = *(s32 *)v_r9;
  Call1((void (*)())Func_0200420c, t122);
L_0200932e:
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  t125 = *(s32 *)v_r7;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, t125, 0x19999, 0xcccc);
  t127 = *(s32 *)v_r7;
  t128 = Value1((s32 (*)())Object_GetById, t127);
  t129 = *(u8 *)(t128 + 90);
  *(u8 *)(t128 + 90) = (254 & t129);
  t130 = *(s32 *)v_r7;
  Call3((void (*)())ObjectMotion_SetPositionAndReset, t130, 0x29e, 170);
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  t133 = *(s32 *)v_r7;
  t134 = Value1((s32 (*)())Object_GetById, t133);
  t135 = *(u8 *)(t134 + 90);
  *(u8 *)(t134 + 90) = (1 | t135);
  t136 = *(s32 *)v_r9;
  if (t136 == 0)
    goto L_02009382;
  Func_020041ec();
L_02009382:
  t138 = *(s32 *)v_r7;
  Call2((void (*)())Func_020043a4, t138, 0x4000);
  Func_0200445c();
  Call1((void (*)())GameFlag_SetBit, 0xa30);
  Call1((void (*)())GameFlag_SetBit, 0xa9e);
  *(u16 *)(0x1f6 + v_r10) = 105;
  Func_020042e4();
L_020093b0:
  return;
}
