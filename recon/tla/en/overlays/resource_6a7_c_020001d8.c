// Draft of overlay 6a7's Func_020001d8, translated from its listing: differ
// 46 of 438. Left: its shape: 440 instructions against the listing's 438.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 Camera_FollowActor();
s32 Camera_MoveWithinMapBounds();
s32 Event_AskYesNoWithActorAnimation();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_WaitFrames();
s32 Func_020006b0();
s32 Func_02000a30();
s32 Func_02000a68();
s32 Func_02000a70();
s32 Func_02000ab0();
s32 Func_02000ac0();
s32 Func_02000ad0();
s32 Func_02000ad8();
s32 Func_02000af0();
s32 Func_02000b00();
s32 Func_02000b10();
s32 Func_02000b30();
s32 Func_02000b58();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 ObjectDispatch_SetSingleChildFlags();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 ObjectMotion_OffsetPositionAndReset();
s32 ObjectMotion_OffsetPositionAndResetMotion();
s32 ObjectMotion_ResetAndSetPositionInMode2();
s32 ObjectMotion_SetPositionAndCommit();
s32 ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
s32 Object_SetModeById();
s32 UiText_OpenMessageAtObject();
s32 WaitFrames();
extern u8 Func_02000064[];
extern u8 MsgShipSafeFiringDistance[];

void Func_020001d8(void) {
  s32 sp_[1];
  s32 v_r5, v_r8, v_r6, v_r7, v_r10, t0, t1, t2, t3, t10;
  s32 t11, t12, t13, t14, t16, t18, t20, t26, t31, t36;
  s32 t37, t40, t41, t45, t52, t53, t59, t60, t61, t66;
  s32 t70, t79, t82, t85, t88, t91, t96;

  t0 = Value1((s32 (*)())Object_GetById, 9);
  t1 = Value1((s32 (*)())Object_GetById, 10);
  t2 = Value1((s32 (*)())Object_GetById, 8);
  t3 = Value1((s32 (*)())GameFlag_Test, 0x90e);
  v_r7 = t2;
  v_r6 = t0;
  v_r8 = t1;
  v_r5 = t3;
  if (v_r5 == 0)
    goto L_0200820c;
  goto L_020085f2;
L_0200820c:
  Func_02000a68();
  Call1((void (*)())Func_02000b30, 0);
  Call1((void (*)())Func_02000ac0, (s32)MsgShipSafeFiringDistance);
  Call3((void (*)())ObjectMotion_SetPositionAndCommit, 8, 0x1d0, 216);
  Call2((void (*)())Func_02000ad8, 8, 0xc000);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  t10 = Value1((s32 (*)())Object_GetById, 11);
  FIELD_AT(t10, s32, 24) = v_r5;
  t11 = Value1((s32 (*)())Object_GetById, 7);
  FIELD_AT(t11, s32, 24) = v_r5;
  t12 = Value1((s32 (*)())Object_GetById, 5);
  FIELD_AT(t12, s32, 24) = v_r5;
  t13 = Value1((s32 (*)())Object_GetById, 6);
  FIELD_AT(t13, s32, 24) = v_r5;
  t14 = Value1((s32 (*)())Object_GetById, 11);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t14, 0);
  t16 = Value1((s32 (*)())Object_GetById, 7);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t16, 0);
  t18 = Value1((s32 (*)())Object_GetById, 5);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t18, 0);
  t20 = Value1((s32 (*)())Object_GetById, 6);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t20, 0);
  Call3((void (*)())Func_02000ab0, 11, 0x1d00000, 0xd80000);
  Call3((void (*)())Func_02000ab0, 7, 0x1d00000, 0xd80000);
  Call3((void (*)())Func_02000ab0, 5, 0x1d00000, 0xd80000);
  Call3((void (*)())Func_02000ab0, 6, 0x1d00000, 0xd80000);
  t26 = Value1((s32 (*)())GameFlag_Test, 0x90d);
  v_r10 = (s32)MsgShipSafeFiringDistance;
  if (t26 != 0)
    goto L_020082ea;
  Call1((void (*)())GameFlag_SetBit, 0x90d);
  Call3((void (*)())Func_02000ad0, 0x2007, 0, 5);
L_020082ea:
  Call1((void (*)())Func_02000ac0, (v_r10 + 1));
  UiText_OpenMessageAtObject(11, 0);
  t31 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0);
  if (t31 == 0)
    goto L_0200832c;
  Call3((void (*)())Func_02000ad0, 11, 0, 5);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 8, 0, 16);
  Call1((void (*)())Func_02000a30, v_r7);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  goto L_020085c6;
L_0200832c:
  t36 = FIELD_AT(0x3000000, s32, 108);
  t37 = FIELD_AT(t36, u16, 452);
  FIELD_AT(t36, u16, 452) = (t37 + 1);
  Call3((void (*)())Func_02000ad0, 11, 0, 5);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0x30000, 0x18000);
  t40 = Value1((s32 (*)())Object_GetById, 8);
  t41 = FIELD_AT(t40, u8, 90);
  FIELD_AT(t40, u8, 90) = (254 & t41);
  Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 8, 0, 16);
  Call1((void (*)())Func_02000b58, 144);
  Call1((void (*)())FieldEvent_WaitFrames, 4);
  FIELD_AT(v_r8, s32, 24) = 0x18000;
  FIELD_AT(v_r8, s32, 28) = 0x18000;
  t45 = Value1((s32 (*)())Object_GetById, 10);
  Call2((void (*)())Animation_ApplyChildValues, t45, 6);
  Call3((void (*)())Func_02000ab0, 10, 0x1d00000, 0xc80000);
  Call2((void (*)())Object_SetModeById, 10, 1);
  FIELD_AT(v_r6, s32, 24) = 0x8000;
  FIELD_AT(v_r6, s32, 28) = 0x8000;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 9, 0x20000, 0x10000);
  Call3((void (*)())Func_02000ab0, 9, 0x1d00000, 0xd80000);
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 9, 0x1d0, 144);
  v_r5 = 15;
L_020083da:
  t52 = FIELD_AT(v_r6, s32, 24);
  FIELD_AT(v_r6, s32, 24) = (t52 + 0x800);
  t53 = FIELD_AT(v_r6, s32, 28);
  FIELD_AT(v_r6, s32, 28) = (t53 + 0x800);
  Call1((void (*)())WaitFrames, 1);
  v_r5 = (v_r5 - 1);
  if (v_r5 >= 0)
    goto L_020083da;
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1d80000, -1, 0xb00000, 1);
  Func_02000af0();
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 9);
  Call1((void (*)())Func_02000b58, 145);
  t59 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(sp_, s32, 12) = (t59 - 0x20000);
  t60 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(sp_, s32, 16) = t60;
  t61 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(sp_, s32, 20) = (t61 - 0x80000);
  *(s32 *)sp_ = 0x1900000;
  FIELD_AT(sp_, s32, 4) = 0;
  FIELD_AT(sp_, s32, 8) = 0x500000;
  Call3((void (*)())Func_020006b0, ((s32)sp_ + 12), (s32)sp_,
        (s32)Func_02000064);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_02000ab0, 10, 0, 0);
  Call3((void (*)())Func_02000ab0, 9, 0, 0);
  t66 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t66, s32, 436) = 100;
  Call2((void (*)())Func_02000b10, 0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  t70 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t70, s32, 436) = 8;
  Call3((void (*)())Func_02000ad0, 0x2005, 0, 5);
  Call3((void (*)())Func_02000ad0, 11, 0, 5);
  Call3((void (*)())Func_02000ad0, 0x2006, 0, 5);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0xcccc, 0x6666);
  Call3((void (*)())ObjectMotion_SetPositionAndCommit, 8, 0x1d0, 176);
  Call2((void (*)())Camera_FollowActor, 8, 1);
  Call3((void (*)())ObjectMotion_SetPositionAndCommit, 8, 0x1d0, 152);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0x9999, 0x4ccc);
  t79 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t79 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t79 - 0x1000);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 8, 0, -16);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 8);
  t82 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t82 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t82 - 0x1000);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 8, 0, -16);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 8);
  t85 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t85 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t85 - 0x1000);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 8, 0, -16);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 8);
  t88 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t88 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t88 - 0x1000);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 8, 0, -16);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 8);
  t91 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t91 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t91 - 0x1000);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 8, 0, -16);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 8);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0x3333, 0x1999);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 8, 0, -12);
  v_r5 = 19;
L_02008594:
  t96 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t96 - 0x900);
  FIELD_AT(v_r7, s32, 24) = (t96 - 0x900);
  Call1((void (*)())WaitFrames, 7);
  v_r5 = (v_r5 - 1);
  if (v_r5 >= 0)
    goto L_02008594;
  FIELD_AT(v_r7, s32, 28) = 0;
  FIELD_AT(v_r7, s32, 24) = 0;
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call1((void (*)())GameFlag_SetBit, 0x90e);
  Call1((void (*)())Func_02000b00, 4);
L_020085c6:
  Call3((void (*)())Func_02000ab0, 11, 0, 0);
  Call3((void (*)())Func_02000ab0, 5, 0, 0);
  Call3((void (*)())Func_02000ab0, 6, 0, 0);
  Call3((void (*)())Func_02000ab0, 7, 0, 0);
  Func_02000a70();
L_020085f2:
  return;
}
