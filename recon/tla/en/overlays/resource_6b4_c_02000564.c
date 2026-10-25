// Draft of overlay 6b4's Func_02000564, translated from its listing: differ
// 813 of 1526. Left: its shape: 1549 instructions against the listing's 1526.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 Event_FinishScreenTransition();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_WaitFrames();
s32 Field_StartPaletteTransition();
s32 Func_020002a4();
s32 Func_0200032c();
s32 Func_02000478();
s32 Func_02001454();
s32 Func_0200147c();
s32 Func_02001484();
s32 Func_020014b4();
s32 Func_020014d4();
s32 Func_020014e4();
s32 Func_020014ec();
s32 Func_020014f4();
s32 Func_0200150c();
s32 Func_02001514();
s32 Func_0200151c();
s32 Func_0200152c();
s32 Func_02001534();
s32 Func_0200153c();
s32 Func_02001544();
s32 Func_02001564();
s32 Func_02001574();
s32 Func_0200157c();
s32 Func_02001584();
s32 Func_0200158c();
s32 Func_02001594();
s32 MemFree();
s32 Motion_SetVarCbAndRefresh();
s32 ObjectDispatch_SetSingleChildFlags();
s32 ObjectMotion_ArmCallback();
s32 ObjectMotion_ResetAndSetPositionInMode2();
s32 ObjectMotion_SetActionVariant();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
s32 Object_SetActionById();
s32 Object_SetModeById();
s32 Runtime_BumpAllocateAlternatePool();
s32 Scheduler_AddOrUpdateCallback();
s32 Scheduler_RemoveCallbackFar();
s32 WaitFrames();
extern u8 Func_02000394[];
extern u8 IwramCopyWords[];
extern u8 MsgGoldenSunEternalLifeAndPower[];
extern u8 MsgGoldenSunFormation[];
extern u8 gBgScroll[];
extern u8 gInput[];
extern u8 gIoWriteQueue[];
extern u8 gPartyState[];

s32 Func_02000564(void) {
  s32 v_r5, v_r9, v_r6, v_r12, v_r7, v_r3, v_r0, v_r1, v_r4, v_r8;
  s32 v_r10, v_r2, t0, t2, t3, t6, t10, t14, t15, t16;
  s32 t17, t18, t19, t28, t29, t32, t45, t46, t48, t52;
  s32 t53, t54, t55, t56, t57, t58, t59, t60, t61, t62;
  s32 t63, t65, t66, t70, t75, t76, t77, t78, t82, t83;
  s32 t84, t85, t86, t87, t95, t96, t97, t139, t151, t152;
  s32 t175, t183, t184, t193, t199, t217, t218, t228, t230, t231;
  s32 t236, t237, t239, t240, t241, t249, t290, t293, t294, t295;
  s32 t296, t300, t301, t302, t317, t320, t323, t324;

  t0 = Value1((s32 (*)())Runtime_BumpAllocateAlternatePool, 0x200);
  Func_02001454();
  t2 = FIELD_AT((s32)gPartyState, u8, 549);
  t3 = FIELD_AT((s32)gPartyState, u8, 550);
  Call2((void (*)())Func_02001484, t2, t3);
  Call3((void (*)())Func_020014b4, 4, 0, 0);
  t6 = FIELD_AT((s32)gPartyState, s16, 482);
  v_r9 = t0;
  v_r5 = t6;
  if (v_r5 != 10)
    goto L_020085c4;
  Call2((void (*)())Func_02001534, 3, 72);
  goto L_020093d6;
L_020085c4:
  if (v_r5 == 2)
    goto L_020085ca;
  goto L_020087fc;
L_020085ca:
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x800000, 0);
  Call2((void (*)())Func_0200151c, 0x10000, 0x2000);
  t10 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t10, 0);
  Call2((void (*)())Object_SetModeById, 8, 12);
  Call2((void (*)())Object_SetActionById, 8, 8);
  t14 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t14, s32, 428) = 0;
  t15 = *(u16 *)0x4000000;
  *(u16 *)0x4000000 = (0xfdff & t15);
  t16 = *(u16 *)0x400000a;
  *(u16 *)0x400000a = (0xfffc & t16);
  t17 = *(u16 *)0x400000a;
  *(u16 *)0x400000a = (t17 | 3);
  t18 = *(u16 *)0x400000e;
  *(u16 *)0x400000e = (0xfffc & t18);
  t19 = *(u16 *)0x400000e;
  *(u16 *)0x400000e = (t19 | 2);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x13e0000, 1);
  v_r5 = 0;
  goto L_02008674;
L_02008674:
  Func_0200152c();
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x13c0000, -1, 0x13e0000, 1);
  Func_0200152c();
L_0200868e:
  if (v_r5 != 3)
    goto L_020086b4;
  Call2((void (*)())Func_0200151c, 0x1999, 0x333);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x13c0000, -1, 0x12c0000, 1);
L_020086b4:
  t28 = Value1((s32 (*)())Object_GetById, 8);
  t29 = FIELD_AT(t28, s32, 16);
  FIELD_AT(t28, s32, 16) = (t29 - 0x10000);
  Call1((void (*)())WaitFrames, 30);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 10)
    goto L_0200868e;
  Call2((void (*)())Object_SetActionById, 8, 0);
  v_r6 = 0;
L_020086e8:
  v_r7 = 0x500000e;
  v_r5 = 0;
  v_r12 = 0x500000e;
L_020086f2:
  t32 = *(u16 *)v_r12;
  v_r4 = ((31 & t32) + v_r6);
  v_r1 = (((u32)(t32 << 16) >> 26) & 31);
  v_r0 = (((u32)(t32 << 16) >> 21) & 31);
  v_r3 = v_r6;
  if (v_r3 >= 0)
    goto L_02008710;
  v_r3 = (v_r3 + 15);
L_02008710:
  v_r1 = (v_r1 - v_r6);
  v_r0 = (v_r0 + (v_r3 >> 4));
  if (v_r4 <= 31)
    goto L_0200871c;
  v_r4 = 31;
L_0200871c:
  if (v_r0 <= 31)
    goto L_02008730;
  v_r0 = 31;
  goto L_02008730;
L_02008730:
  if (v_r1 > 7)
    goto L_02008736;
  v_r1 = 8;
L_02008736:
  if (v_r1 <= 31)
    goto L_0200873c;
  v_r1 = 31;
L_0200873c:
  *(u16 *)v_r7 = (((v_r1 << 10) | (v_r0 << 5)) | v_r4);
  v_r5 = (v_r5 + 1);
  v_r7 = (v_r7 + 2);
  v_r12 = (v_r12 + 2);
  if (v_r5 != 8)
    goto L_020086f2;
  Call1((void (*)())WaitFrames, 4);
  v_r6 = (v_r6 + 1);
  if (v_r6 != 10)
    goto L_020086e8;
  Call3((void (*)())Func_0200150c, 8, 0x102, 0);
  Call1((void (*)())WaitFrames, 30);
  Call1((void (*)())Func_020014e4, (s32)MsgGoldenSunFormation);
  Call2((void (*)())Func_020014f4, 0x4008, 0);
  Call1((void (*)())WaitFrames, 30);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call2((void (*)())Func_020014f4, 0x4008, 0);
  Call2((void (*)())Object_SetActionById, 8, 32);
  v_r5 = 0;
L_020087a6:
  if (v_r5 != 16)
    goto L_020087c8;
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x13c0000, -1, 0x800000, 1);
  Call2((void (*)())Func_0200151c, 0x20000, 0x4000);
L_020087c8:
  if (v_r5 != 19)
    goto L_020087d0;
  Event_FinishScreenTransition();
L_020087d0:
  t45 = Value1((s32 (*)())Object_GetById, 8);
  t46 = FIELD_AT(t45, s32, 16);
  FIELD_AT(t45, s32, 16) = (t46 - 0x20000);
  Call1((void (*)())WaitFrames, 8);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 20)
    goto L_020087a6;
  v_r0 = 3;
  goto L_02009370;
L_020087fc:
  if (v_r5 == 3)
    goto L_02008804;
  goto L_02009378;
L_02008804:
  t48 = *(s32 *)0x3000080;
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x900000, 0);
  Call2((void (*)())Func_0200151c, 0x10000, 0x2000);
  Call2((void (*)())Object_SetModeById, 8, 14);
  t52 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t52, u16, 6) = 0;
  t53 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t53, s32, 8) = 0xf80000;
  t54 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t54, s32, 16) = 0x980000;
  t55 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t56 = *(u16 *)(s32)gIoWriteQueue;
  v_r2 = t56;
  v_r10 = t48;
  v_r7 = 0;
  v_r8 = 0x3000000;
  v_r0 = (s32)gIoWriteQueue;
  v_r1 = 0x4000208;
  v_r4 = t55;
  if (v_r2 > 31)
    goto L_0200889e;
  *(u16 *)v_r0 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) = 0x27f;
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 8) = 0x4000050;
  FIELD_AT(((((v_r2 << 1) + v_r2) << 2) + v_r0), s32, 12) = 0x20000;
L_0200889e:
  *(u16 *)v_r1 = v_r4;
  t57 = *(u16 *)v_r1;
  *(u16 *)v_r1 = v_r1;
  t58 = *(u16 *)v_r0;
  v_r2 = t58;
  v_r4 = t57;
  if (v_r2 > 31)
    goto L_020088ce;
  *(u16 *)v_r0 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) = 0x1000;
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 8) = 0x4000052;
  FIELD_AT(((((v_r2 << 1) + v_r2) << 2) + v_r0), s32, 12) = 0x20000;
L_020088ce:
  *(u16 *)v_r1 = v_r4;
  t59 = *(u16 *)0x4000000;
  *(u16 *)0x4000000 = (0xfdff & t59);
  t60 = *(u16 *)0x400000a;
  *(u16 *)0x400000a = (0xfffc & t60);
  t61 = *(u16 *)0x400000a;
  *(u16 *)0x400000a = (t61 | 3);
  t62 = *(u16 *)0x400000e;
  *(u16 *)0x400000e = (0xfffc & t62);
  t63 = *(u16 *)0x400000e;
  *(u16 *)0x400000e = (t63 | 2);
  Call1((void (*)())WaitFrames, 1);
  t65 = FIELD_AT(v_r8, s32, 108);
  FIELD_AT(t65, s32, 428) = v_r7;
  t66 = *(u16 *)0x4000000;
  v_r5 = (0xfdff & t66);
  *(u16 *)0x4000000 = v_r5;
  goto L_0200893c;
L_0200893c:
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3((void (*)())Func_0200147c, 0x40000, 0x40000, 0x10000);
  v_r5 = 0;
L_02008956:
  t70 = FIELD_AT((s32)gBgScroll, u16, 10);
  FIELD_AT((s32)gBgScroll, u16, 10) = (t70 + 8);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 30)
    goto L_02008956;
  Call2((void (*)())Object_SetModeById, 8, 15);
  Call1((void (*)())WaitFrames, 60);
  Call1((void (*)())Func_02001594, 212);
  t75 = *(u16 *)0x400000a;
  *(u16 *)0x400000a = (0xfffc & t75);
  t76 = *(u16 *)0x400000a;
  *(u16 *)0x400000a = (t76 | 1);
  *(u16 *)0x4000050 = 0x3f42;
  t77 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t78 = *(u16 *)(s32)gIoWriteQueue;
  v_r1 = (s32)gIoWriteQueue;
  v_r2 = t78;
  v_r0 = 0x4000208;
  v_r4 = t77;
  if (v_r2 > 31)
    goto L_020089ec;
  goto L_020089c8;
L_020089c8:
  *(u16 *)v_r1 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r1) + 4) = 0x1002;
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r1) + 8) = 0x4000052;
  FIELD_AT(((((v_r2 << 1) + v_r2) << 2) + v_r1), s32, 12) = 0x20000;
L_020089ec:
  *(u16 *)v_r0 = v_r4;
  v_r6 = 0;
L_020089f0:
  if (v_r6 != 16)
    goto L_02008a02;
  Call1((void (*)())Func_02001594, 140);
  Call2((void (*)())Object_SetModeById, 8, 16);
L_02008a02:
  if (v_r6 != 51)
    goto L_02008a2e;
  Call1((void (*)())Func_02001594, 107);
L_02008a2e:
  v_r2 = (v_r6 - 16);
  if ((u32)v_r2 > (u32)12)
    goto L_02008a74;
  t82 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t83 = *(u16 *)(s32)gIoWriteQueue;
  t84 = (((v_r2 + ((u32)v_r2 >> 31)) >> 1) + 2);
  v_r0 = (s32)gIoWriteQueue;
  v_r2 = t83;
  v_r4 = 0x4000208;
  v_r5 = t82;
  v_r1 = t84;
  if (v_r2 > 31)
    goto L_02008a72;
  *(u16 *)v_r0 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) = (v_r1 | 0x1000);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 8) = 0x4000052;
  FIELD_AT(((((v_r2 << 1) + v_r2) << 2) + v_r0), s32, 12) = 0x20000;
L_02008a72:
  *(u16 *)v_r4 = v_r5;
L_02008a74:
  v_r2 = (v_r6 - 34);
  if ((u32)v_r2 > (u32)16)
    goto L_02008abc;
  t85 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t86 = *(u16 *)(s32)gIoWriteQueue;
  t87 = (((v_r2 + ((u32)v_r2 >> 31)) >> 1) + 8);
  v_r0 = (s32)gIoWriteQueue;
  v_r2 = t86;
  v_r4 = 0x4000208;
  v_r5 = t85;
  v_r1 = t87;
  if (v_r2 > 31)
    goto L_02008aba;
  *(u16 *)v_r0 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) = (v_r1 | 0x1000);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 8) = 0x4000052;
  FIELD_AT(((((v_r2 << 1) + v_r2) << 2) + v_r0), s32, 12) = 0x20000;
L_02008aba:
  *(u16 *)v_r4 = v_r5;
L_02008abc:
  if ((u32)(v_r6 - 20) > (u32)31)
    goto L_02008ad6;
  Call3((void (*)())Func_020002a4, 0x5000080, 0x50000c0, 16);
L_02008ad6:
  if (v_r6 <= 49)
    goto L_02008ae6;
  Call3((void (*)())Func_0200032c, 0x5000000, 224, 1);
L_02008ae6:
  Call1((void (*)())WaitFrames, 4);
  v_r6 = (v_r6 + 1);
  if (v_r6 == 84)
    goto L_02008af4;
  goto L_020089f0;
L_02008af4:
  Call2((void (*)())ObjectMotion_SetActionVariant, 8, 3);
  Call3((void (*)())Func_0200147c, -1, -1, 0xe666);
  Call1((void (*)())Func_02001594, 0x186);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x13e0000, 0);
  t95 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t96 = *(u16 *)(s32)gIoWriteQueue;
  v_r1 = (s32)gIoWriteQueue;
  v_r3 = t96;
  v_r4 = 0x4000208;
  v_r5 = t95;
  if (v_r3 > 31)
    goto L_02008b62;
  *(u16 *)v_r1 = (v_r3 + 1);
  t97 = *(u16 *)0x400000a;
  *(s32 *)(((((v_r3 << 1) + v_r3) << 2) + v_r1) + 4) = ((-4 & t97) | 3);
  *(s32 *)(((((v_r3 << 1) + v_r3) << 2) + v_r1) + 8) = 0x400000a;
  FIELD_AT(((((v_r3 << 1) + v_r3) << 2) + v_r1), s32, 12) = 0x20000;
L_02008b62:
  *(u16 *)v_r4 = v_r5;
  Call1((void (*)())Func_02001594, 212);
  Call1((void (*)())WaitFrames, 1);
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02000394, 0x480);
  v_r6 = 0;
L_02008b7c:
  Call3((void (*)())Func_020002a4, 0x5000000, v_r9, 224);
  Call1((void (*)())WaitFrames, 4);
  v_r6 = (v_r6 + 1);
  if (v_r6 != 32)
    goto L_02008b7c;
  Call2((void (*)())ObjectMotion_SetActionVariant, 8, 1);
  Call2((void (*)())Object_SetModeById, 8, 1);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x800000, 1);
  Call2((void (*)())Func_0200151c, 0x10000, 0x2000);
  Func_0200152c();
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0x4ccc, 0x2666);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 8, 248, 128);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call1((void (*)())Func_02001594, 78);
  Call1((void (*)())Func_020014e4, (s32)MsgGoldenSunEternalLifeAndPower);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call3((void (*)())ObjectMotion_ArmCallback, 8, 0x4000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 8, 14);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call2((void (*)())Object_SetModeById, 8, 15);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 150);
  Func_0200150c(8, 0x101, 30);
  Call2((void (*)())Object_SetModeById, 8, 1);
  Call3((void (*)())ObjectMotion_ArmCallback, 8, 0x6000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 8, 0x2000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 8, 0x6000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 8, 0x2000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 8, 0x4000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 50);
  Call2((void (*)())Object_SetModeById, 8, 15);
  Call1((void (*)())FieldEvent_WaitFrames, 50);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call1((void (*)())FieldEvent_WaitFrames, 50);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  t139 = Value1((s32 (*)())Object_GetById, 9);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t139, 0);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 9, 216, 96);
  Call3((void (*)())ObjectMotion_ArmCallback, 8, 0x2000, 0);
  Call3((void (*)())Func_0200150c, 8, 0x102, 30);
  Call1((void (*)())Func_020014e4, ((s32)MsgGoldenSunEternalLifeAndPower + 4));
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call1((void (*)())Func_02001594, 0x2d2);
  Call3((void (*)())Func_020014ec, 9, 0, 5);
  Call2((void (*)())Object_SetModeById, 8, 1);
  Call3((void (*)())ObjectMotion_ArmCallback, 8, 0xa000, 0);
  Call3((void (*)())Func_0200150c, 8, 0x100, 30);
  t151 = Value1((s32 (*)())Object_GetById, 8);
  t152 = FIELD_AT(t151, u8, 90);
  FIELD_AT(t151, u8, 90) = (254 & t152);
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 8, 0x100, 136);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0x9999, 0x4ccc);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Object_SetModeById, 8, 1);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call2((void (*)())Object_SetModeById, 9, 4);
  Call3((void (*)())Func_020014ec, 9, 0, 5);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  v_r6 = 0;
  v_r5 = (s32)MsgGoldenSunEternalLifeAndPower;
  v_r1 = 5;
  v_r0 = 9;
  goto L_02008df4;
L_02008df4:
  Call2((void (*)())Object_SetModeById, v_r0, v_r1);
  Call3((void (*)())Func_020014ec, 9, 0, 5);
  Call2((void (*)())Func_02001514, 8, 0x102);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call2((void (*)())Object_SetModeById, 9, 6);
  Call3((void (*)())Func_020014ec, 9, 0, 5);
  Call3((void (*)())Func_0200150c, 8, 0x107, 30);
  v_r5 = (v_r5 + 12);
  Call1((void (*)())Func_020014e4, v_r5);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call2((void (*)())Object_SetModeById, 8, 13);
  Func_02001574();
  t175 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())Func_0200158c, t175, 2);
  Call1((void (*)())Func_02001594, 178);
  Call2((void (*)())Func_02001564, 8, 0x4084);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Object_SetModeById, 9, 3);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 9, 0x1999, 0xccc);
L_02008e9e:
  if ((1 & v_r6) == 0)
    goto L_02008eb2;
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 9, 216, 96);
  goto L_02008ecc;
L_02008eb2:
  t183 = Value1((s32 (*)())Object_GetById, 9);
  t184 = FIELD_AT(t183, u8, 90);
  FIELD_AT(t183, u8, 90) = (254 & t184);
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 9, 208, 88);
L_02008ecc:
  v_r5 = 0;
L_02008ece:
  Call2((void (*)())Func_020014d4, 9, 0x100);
  Call1((void (*)())FieldEvent_WaitFrames, 3);
  Call2((void (*)())Func_020014d4, 9, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 3);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 10)
    goto L_02008ece;
  v_r6 = (v_r6 + 1);
  if (v_r6 != 4)
    goto L_02008e9e;
  Func_0200150c(8, 0x108, 30);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  t193 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())Func_0200158c, t193, 0);
  Func_02001584();
  Func_0200157c();
  Call2((void (*)())Object_SetModeById, 9, 6);
  Func_02001574();
  t199 = Value1((s32 (*)())Object_GetById, 9);
  Call2((void (*)())Func_0200158c, t199, 2);
  Call1((void (*)())WaitFrames, 30);
  Call1((void (*)())Func_02001594, 212);
  Call3((void (*)())Func_0200147c, 0x100000, 0x100000, 0x10000);
  v_r5 = 0;
L_02008f62:
  Call2((void (*)())Func_0200153c, 0x10005, 1);
  Call1((void (*)())Func_02001544, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 2);
  Call2((void (*)())Func_0200153c, 0x10000, 1);
  Call1((void (*)())Func_02001544, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 2);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 4)
    goto L_02008f62;
  Call3((void (*)())Func_0200147c, 0, 0, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())Field_StartPaletteTransition, 1);
  Call2((void (*)())Func_02001564, 8, 0x1110);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0x80000, 0x40000);
  Call2((void (*)())Func_02000478, 128, 8);
  t217 = Value1((s32 (*)())Object_GetById, 8);
  t218 = FIELD_AT(t217, u8, 90);
  FIELD_AT(t217, u8, 90) = (254 & t218);
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 8, 0x118, 160);
  Call2((void (*)())Object_SetModeById, 8, 17);
  Call1((void (*)())Func_02001594, 134);
  Call1((void (*)())FieldEvent_WaitFrames, 2);
  Call2((void (*)())Func_02000478, 128, 8);
  Call1((void (*)())FieldEvent_WaitFrames, 2);
  Call2((void (*)())Func_02000478, 128, 8);
  Call1((void (*)())FieldEvent_WaitFrames, 56);
  Call1((void (*)())Func_02001594, 178);
  t228 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t228, u8, 85) = 0;
  v_r5 = 0;
L_02009034:
  if (v_r5 != 16)
    goto L_02009040;
  Call2((void (*)())Object_SetModeById, 8, 18);
L_02009040:
  t230 = Value1((s32 (*)())Object_GetById, 8);
  t231 = FIELD_AT(t230, s32, 12);
  FIELD_AT(t230, s32, 12) = (t231 + 0x2666);
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 64)
    goto L_02009034;
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  v_r5 = 0;
L_0200907c:
  t236 = Value1((s32 (*)())Object_GetById, 8);
  t237 = FIELD_AT(t236, s32, 12);
  FIELD_AT(t236, s32, 12) = (t237 + 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 32)
    goto L_0200907c;
  t239 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t239, u8, 85) = 1;
  v_r5 = 0;
L_020090a6:
  t240 = Value1((s32 (*)())Object_GetById, 8);
  t241 = FIELD_AT(t240, s32, 12);
  FIELD_AT(t240, s32, 12) = (t241 - 0x851eb);
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 5)
    goto L_020090a6;
  Call2((void (*)())Func_02000478, 0x100, 16);
  Call2((void (*)())Object_SetModeById, 8, 17);
  Call1((void (*)())Func_02001594, 145);
  Call3((void (*)())Func_0200147c, 0x100000, 0x100000, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200147c, -1, -1, 0xe666);
  t249 = Value1((s32 (*)())Object_GetById, 9);
  Call2((void (*)())Func_0200158c, t249, 0);
  Func_02001584();
  Func_0200157c();
  Call1((void (*)())Func_02001594, 0x186);
  Call2((void (*)())Object_SetModeById, 9, 4);
  Call1((void (*)())Field_StartPaletteTransition, 32);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020014ec, 9, 0, 5);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call2((void (*)())Object_SetModeById, 9, 3);
  Call3((void (*)())Func_020014ec, 9, 0, 5);
  Call2((void (*)())Object_SetModeById, 8, 18);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call1((void (*)())FieldEvent_WaitFrames, 120);
  Call1((void (*)())Func_02001594, 107);
  v_r5 = 0;
L_02009192:
  Call3((void (*)())Func_0200147c, ((v_r5 << 16) + 0x10000),
        ((v_r5 << 16) + 0x10000), 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 8);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 2)
    goto L_02009192;
  Call2((void (*)())Object_SetModeById, 9, 8);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call2((void (*)())Object_SetModeById, 9, 7);
  Call3((void (*)())Func_020014ec, 9, 0, 5);
  Func_0200150c(8, 0x101, 30);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call2((void (*)())Object_SetModeById, 9, 8);
  Call3((void (*)())Func_020014ec, 9, 0, 5);
  v_r5 = 0;
  v_r6 = 0x20000;
L_020091fe:
  Call3((void (*)())Func_0200147c, v_r6, v_r6, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 8);
  v_r5 = (v_r5 + 1);
  v_r6 = (v_r6 + 0x10000);
  if (v_r5 != 4)
    goto L_020091fe;
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  Call2((void (*)())Object_SetModeById, 9, 3);
  Call3((void (*)())Func_020014ec, 9, 0, 5);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call2((void (*)())Object_SetModeById, 9, 4);
  Call3((void (*)())Func_020014ec, 9, 0, 5);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  t290 = FIELD_AT(0x3000000, s32, 32);
  Call3((void (*)())Func_0200147c, 0x100000, 0x100000, 0x10000);
  v_r7 = 0xfffe2000;
  v_r8 = t290;
  v_r6 = 0;
L_0200928c:
  if (v_r6 != 20)
    goto L_02009298;
  Call2((void (*)())Object_SetModeById, 9, 7);
L_02009298:
  if ((u32)(v_r6 - 30) > (u32)16)
    goto L_020092c6;
  t293 = Value1((s32 (*)())Object_GetById, 9);
  FIELD_AT(t293, s32, 24) = (0x10000 - v_r7);
  t294 = Value1((s32 (*)())Object_GetById, 9);
  v_r5 = (v_r7 + 0x10000);
  FIELD_AT(t294, s32, 28) = v_r5;
  t295 = Value1((s32 (*)())Object_GetById, 9);
  t296 = FIELD_AT(t295, s32, 16);
  FIELD_AT(t295, s32, 16) = (t296 - 0x20000);
L_020092c6:
  if (v_r6 != 47)
    goto L_020092d4;
  Call3((void (*)())Func_020014b4, 9, 0, 0);
L_020092d4:
  if (v_r6 != 64)
    goto L_020092ea;
  Call2((void (*)())Func_0200153c, 0x7fff, 0);
  Call1((void (*)())Func_02001544, 64);
L_020092ea:
  t300 = FIELD_AT(v_r8, s32, 332);
  FIELD_AT(v_r8, s32, 332) = (t300 - 0x10000);
  t301 = Value1((s32 (*)())Object_GetById, 8);
  t302 = FIELD_AT(t301, s32, 16);
  FIELD_AT(t301, s32, 16) = (t302 + 0x10000);
  Call1((void (*)())WaitFrames, 1);
  v_r6 = (v_r6 + 1);
  v_r7 = (v_r7 + 0x1000);
  if (v_r6 != 128)
    goto L_0200928c;
  Call1((void (*)())Func_02001594, 78);
  Call1((void (*)())FieldEvent_WaitFrames, 32);
  Call1((void (*)())Func_02001594, 208);
  Call1((void (*)())FieldEvent_WaitFrames, 120);
  Call1((void (*)())Scheduler_RemoveCallbackFar, (s32)Func_02000394);
  Call2((void (*)())Func_0200153c, 0x7fff, 0);
  Call1((void (*)())Func_02001544, 1);
  Call1((void (*)())WaitFrames, 1);
  Call1((void (*)())Func_02001594, 0x186);
  Call2((void (*)())Func_0200153c, 0, 0);
  Call1((void (*)())Func_02001544, 120);
  Call1((void (*)())WaitFrames, 0x12c);
  v_r0 = 0x136;
L_02009370:
  Call2((void (*)())Func_02001534, v_r0, 1);
  goto L_020093d6;
L_02009378:
  t317 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t317, s32, 428) = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  v_r6 = 0;
  goto L_020093ae;
L_02009394:
  if (v_r6 != 0)
    goto L_020093ac;
  t320 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t320, 0);
  Call2((void (*)())Object_SetModeById, 8, 12);
L_020093ac:
  v_r6 = (v_r6 + 1);
L_020093ae:
  if (v_r6 == 0x140)
    goto L_020093ce;
  t323 = *(s32 *)(s32)gInput;
  v_r1 = (s32)gInput;
  if ((t323 & 2) == 0)
    goto L_020093c4;
  v_r6 = 0;
L_020093c4:
  t324 = *(s32 *)v_r1;
  if ((t324 & 1) == 0)
    goto L_02009394;
L_020093ce:
  Call2((void (*)())Func_02001534, 3, 0);
L_020093d6:
  Call4((void (*)())MemFree, v_r9, v_r1, v_r2, v_r3);
  return 0;
}
