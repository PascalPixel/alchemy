// Draft of overlay 6b4's Func_02000564, translated from its listing: differ
// 802 of 1526. Left: its shape: 1580 instructions against the listing's 1526.
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
extern u8 gAdventure[];

s32 Func_02000564(s32 p0, s32 p1, s32 p2, s32 p3) {
  s32 v_r5, v_r9, v_r6, v_r12, v_r7, v_r3, v_r0, v_r1, v_r4, v_r8;
  s32 v_r10, v_r2, t0, t2, t3, t6, t10, t14, t15, t16;
  s32 t17, t18, t19, t28, t29, t33, t46, t47, t49, t54;
  s32 t55, t56, t57, t58, t59, t60, t61, t62, t63, t64;
  s32 t65, t67, t68, t72, t77, t78, t79, t80, t86, t87;
  s32 t88, t89, t90, t91, t99, t100, t101, t143, t155, t156;
  s32 t179, t187, t188, t197, t203, t221, t222, t232, t234, t235;
  s32 t240, t241, t243, t244, t245, t253, t294, t297, t298, t299;
  s32 t300, t304, t305, t306, t321, t324, t327, t328;

  t0 = Value4((s32 (*)())Runtime_BumpAllocateAlternatePool, 0x200, p1, p2, p3);
  Func_02001454();
  t2 = FIELD_AT((s32)gAdventure, u8, 549);
  t3 = FIELD_AT((s32)gAdventure, u8, 550);
  Call2((void (*)())Func_02001484, t2, t3);
  Call3((void (*)())Func_020014b4, 4, 0, 0);
  t6 = FIELD_AT((s32)gAdventure, s16, 482);
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
  ((s32 (*)())(s32)IwramCopyWords)(v_r9, 0x5000008, 14);
  v_r6 = 0;
L_020086e8:
  v_r7 = 0x500000e;
  v_r5 = 0;
  v_r12 = 0x500000e;
L_020086f2:
  t33 = *(u16 *)v_r12;
  v_r4 = ((31 & t33) + v_r6);
  v_r1 = (((u32)(t33 << 16) >> 26) & 31);
  v_r0 = (((u32)(t33 << 16) >> 21) & 31);
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
  t46 = Value1((s32 (*)())Object_GetById, 8);
  t47 = FIELD_AT(t46, s32, 16);
  FIELD_AT(t46, s32, 16) = (t47 - 0x20000);
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
  t49 = *(s32 *)0x3000080;
  ((s32 (*)())(s32)IwramCopyWords)(v_r9, 0x5000000, 0x1c0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x900000, 0);
  Call2((void (*)())Func_0200151c, 0x10000, 0x2000);
  Call2((void (*)())Object_SetModeById, 8, 14);
  t54 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t54, u16, 6) = 0;
  t55 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t55, s32, 8) = 0xf80000;
  t56 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t56, s32, 16) = 0x980000;
  t57 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t58 = *(u16 *)(s32)gIoWriteQueue;
  v_r2 = t58;
  v_r10 = t49;
  v_r7 = 0;
  v_r8 = 0x3000000;
  v_r0 = (s32)gIoWriteQueue;
  v_r1 = 0x4000208;
  v_r4 = t57;
  if (v_r2 > 31)
    goto L_0200889e;
  *(u16 *)v_r0 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) = 0x27f;
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 8) = 0x4000050;
  FIELD_AT(((((v_r2 << 1) + v_r2) << 2) + v_r0), s32, 12) = 0x20000;
L_0200889e:
  *(u16 *)v_r1 = v_r4;
  t59 = *(u16 *)v_r1;
  *(u16 *)v_r1 = v_r1;
  t60 = *(u16 *)v_r0;
  v_r2 = t60;
  v_r4 = t59;
  if (v_r2 > 31)
    goto L_020088ce;
  *(u16 *)v_r0 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) = 0x1000;
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 8) = 0x4000052;
  FIELD_AT(((((v_r2 << 1) + v_r2) << 2) + v_r0), s32, 12) = 0x20000;
L_020088ce:
  *(u16 *)v_r1 = v_r4;
  t61 = *(u16 *)0x4000000;
  *(u16 *)0x4000000 = (0xfdff & t61);
  t62 = *(u16 *)0x400000a;
  *(u16 *)0x400000a = (0xfffc & t62);
  t63 = *(u16 *)0x400000a;
  *(u16 *)0x400000a = (t63 | 3);
  t64 = *(u16 *)0x400000e;
  *(u16 *)0x400000e = (0xfffc & t64);
  t65 = *(u16 *)0x400000e;
  *(u16 *)0x400000e = (t65 | 2);
  Call1((void (*)())WaitFrames, 1);
  t67 = FIELD_AT(v_r8, s32, 108);
  FIELD_AT(t67, s32, 428) = v_r7;
  t68 = *(u16 *)0x4000000;
  v_r5 = (0xfdff & t68);
  *(u16 *)0x4000000 = v_r5;
  goto L_0200893c;
L_0200893c:
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3((void (*)())Func_0200147c, 0x40000, 0x40000, 0x10000);
  v_r5 = 0;
L_02008956:
  t72 = FIELD_AT((s32)gBgScroll, u16, 10);
  FIELD_AT((s32)gBgScroll, u16, 10) = (t72 + 8);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 30)
    goto L_02008956;
  Call2((void (*)())Object_SetModeById, 8, 15);
  Call1((void (*)())WaitFrames, 60);
  Call1((void (*)())Func_02001594, 212);
  t77 = *(u16 *)0x400000a;
  *(u16 *)0x400000a = (0xfffc & t77);
  t78 = *(u16 *)0x400000a;
  *(u16 *)0x400000a = (t78 | 1);
  *(u16 *)0x4000050 = 0x3f42;
  t79 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t80 = *(u16 *)(s32)gIoWriteQueue;
  v_r1 = (s32)gIoWriteQueue;
  v_r2 = t80;
  v_r0 = 0x4000208;
  v_r4 = t79;
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
  ((s32 (*)())(s32)IwramCopyWords)(v_r9, 0x5000000, 0x1c0);
  ((s32 (*)())(s32)IwramCopyWords)((v_r10 + 0xf80), (v_r10 + 0x1040), 96);
L_02008a2e:
  v_r2 = (v_r6 - 16);
  if ((u32)v_r2 > (u32)12)
    goto L_02008a74;
  t86 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t87 = *(u16 *)(s32)gIoWriteQueue;
  t88 = (((v_r2 + ((u32)v_r2 >> 31)) >> 1) + 2);
  v_r0 = (s32)gIoWriteQueue;
  v_r2 = t87;
  v_r4 = 0x4000208;
  v_r5 = t86;
  v_r1 = t88;
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
  t89 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t90 = *(u16 *)(s32)gIoWriteQueue;
  t91 = (((v_r2 + ((u32)v_r2 >> 31)) >> 1) + 8);
  v_r0 = (s32)gIoWriteQueue;
  v_r2 = t90;
  v_r4 = 0x4000208;
  v_r5 = t89;
  v_r1 = t91;
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
  t99 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t100 = *(u16 *)(s32)gIoWriteQueue;
  v_r1 = (s32)gIoWriteQueue;
  v_r3 = t100;
  v_r4 = 0x4000208;
  v_r5 = t99;
  if (v_r3 > 31)
    goto L_02008b62;
  *(u16 *)v_r1 = (v_r3 + 1);
  t101 = *(u16 *)0x400000a;
  *(s32 *)(((((v_r3 << 1) + v_r3) << 2) + v_r1) + 4) = ((-4 & t101) | 3);
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
  t143 = Value1((s32 (*)())Object_GetById, 9);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t143, 0);
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
  t155 = Value1((s32 (*)())Object_GetById, 8);
  t156 = FIELD_AT(t155, u8, 90);
  FIELD_AT(t155, u8, 90) = (254 & t156);
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
  t179 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())Func_0200158c, t179, 2);
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
  t187 = Value1((s32 (*)())Object_GetById, 9);
  t188 = FIELD_AT(t187, u8, 90);
  FIELD_AT(t187, u8, 90) = (254 & t188);
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
  t197 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())Func_0200158c, t197, 0);
  Func_02001584();
  Func_0200157c();
  Call2((void (*)())Object_SetModeById, 9, 6);
  Func_02001574();
  t203 = Value1((s32 (*)())Object_GetById, 9);
  Call2((void (*)())Func_0200158c, t203, 2);
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
  t221 = Value1((s32 (*)())Object_GetById, 8);
  t222 = FIELD_AT(t221, u8, 90);
  FIELD_AT(t221, u8, 90) = (254 & t222);
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 8, 0x118, 160);
  Call2((void (*)())Object_SetModeById, 8, 17);
  Call1((void (*)())Func_02001594, 134);
  Call1((void (*)())FieldEvent_WaitFrames, 2);
  Call2((void (*)())Func_02000478, 128, 8);
  Call1((void (*)())FieldEvent_WaitFrames, 2);
  Call2((void (*)())Func_02000478, 128, 8);
  Call1((void (*)())FieldEvent_WaitFrames, 56);
  Call1((void (*)())Func_02001594, 178);
  t232 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t232, u8, 85) = 0;
  v_r5 = 0;
L_02009034:
  if (v_r5 != 16)
    goto L_02009040;
  Call2((void (*)())Object_SetModeById, 8, 18);
L_02009040:
  t234 = Value1((s32 (*)())Object_GetById, 8);
  t235 = FIELD_AT(t234, s32, 12);
  FIELD_AT(t234, s32, 12) = (t235 + 0x2666);
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 64)
    goto L_02009034;
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call3((void (*)())Func_020014ec, 0x2008, 0, 5);
  v_r5 = 0;
L_0200907c:
  t240 = Value1((s32 (*)())Object_GetById, 8);
  t241 = FIELD_AT(t240, s32, 12);
  FIELD_AT(t240, s32, 12) = (t241 + 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  v_r5 = (v_r5 + 1);
  if (v_r5 != 32)
    goto L_0200907c;
  t243 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t243, u8, 85) = 1;
  v_r5 = 0;
L_020090a6:
  t244 = Value1((s32 (*)())Object_GetById, 8);
  t245 = FIELD_AT(t244, s32, 12);
  FIELD_AT(t244, s32, 12) = (t245 - 0x851eb);
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
  t253 = Value1((s32 (*)())Object_GetById, 9);
  Call2((void (*)())Func_0200158c, t253, 0);
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
  t294 = FIELD_AT(0x3000000, s32, 32);
  Call3((void (*)())Func_0200147c, 0x100000, 0x100000, 0x10000);
  v_r7 = 0xfffe2000;
  v_r8 = t294;
  v_r6 = 0;
L_0200928c:
  if (v_r6 != 20)
    goto L_02009298;
  Call2((void (*)())Object_SetModeById, 9, 7);
L_02009298:
  if ((u32)(v_r6 - 30) > (u32)16)
    goto L_020092c6;
  t297 = Value1((s32 (*)())Object_GetById, 9);
  FIELD_AT(t297, s32, 24) = (0x10000 - v_r7);
  t298 = Value1((s32 (*)())Object_GetById, 9);
  v_r5 = (v_r7 + 0x10000);
  FIELD_AT(t298, s32, 28) = v_r5;
  t299 = Value1((s32 (*)())Object_GetById, 9);
  t300 = FIELD_AT(t299, s32, 16);
  FIELD_AT(t299, s32, 16) = (t300 - 0x20000);
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
  t304 = FIELD_AT(v_r8, s32, 332);
  FIELD_AT(v_r8, s32, 332) = (t304 - 0x10000);
  t305 = Value1((s32 (*)())Object_GetById, 8);
  t306 = FIELD_AT(t305, s32, 16);
  FIELD_AT(t305, s32, 16) = (t306 + 0x10000);
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
  t321 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t321, s32, 428) = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  v_r6 = 0;
  goto L_020093ae;
L_02009394:
  if (v_r6 != 0)
    goto L_020093ac;
  t324 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t324, 0);
  Call2((void (*)())Object_SetModeById, 8, 12);
L_020093ac:
  v_r6 = (v_r6 + 1);
L_020093ae:
  if (v_r6 == 0x140)
    goto L_020093ce;
  t327 = *(s32 *)(s32)gInput;
  v_r1 = (s32)gInput;
  if ((t327 & 2) == 0)
    goto L_020093c4;
  v_r6 = 0;
L_020093c4:
  t328 = *(s32 *)v_r1;
  if ((t328 & 1) == 0)
    goto L_02009394;
L_020093ce:
  Call2((void (*)())Func_02001534, 3, 0);
L_020093d6:
  Call4((void (*)())MemFree, v_r9, v_r1, v_r2, v_r3);
  return 0;
}
