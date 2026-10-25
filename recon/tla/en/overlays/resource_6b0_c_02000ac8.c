// Draft of overlay 6b0's Func_02000ac8, translated from its listing: differ
// 549 of 630. Left: its shape: 639 instructions against the listing's 630.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_SetStateFlags();
s32 Camera_FollowActor();
s32 Camera_MoveWithinMapBounds();
s32 Engine_ObjectCreate();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_WaitFrames();
s32 Func_0200088c();
s32 Func_020009b8();
s32 Func_02000a78();
s32 Func_020032e8();
s32 Func_020032f8();
s32 Func_02003328();
s32 Func_02003330();
s32 Func_02003338();
s32 Func_02003380();
s32 Func_02003398();
s32 Func_020033a0();
s32 Func_02003400();
s32 Func_02003420();
s32 Func_02003430();
s32 Func_02003458();
s32 Func_02003460();
s32 Func_02003468();
s32 Func_02003488();
s32 Func_020034b8();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 ObjectDispatch_SetSingleChildFlags();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 ObjectMotion_ResetAndSetPositionInMode2();
s32 ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
s32 Object_SetModeById();
s32 Random16Far();
s32 WaitFrames();
extern u8 gData_020035cc[];
extern u8 gData_02003620[];
extern u8 gData_020036fc[];
extern u8 gPartyState[];

void Func_02000ac8(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4, s32 p5) {
  s32 sp_[1];
  s32 spo_[2];
  s32 v_r9, v_r11, v_r10, v_r8, v_r0, v_r6, v_r5, v_r7, v_r3, v_r2;
  s32 v_r4, v_r1, t0, t1, t2, t3, t4, t5, t7, t10;
  s32 t13, t14, t16, t18, t20, t22, t24, t25, t26, t27;
  s32 t35, t36, t38, t40, t44, t45, t46, t47, t48, t49;
  s32 t50, t51, t52, t53, t54, t55, t56, t57, t58, t59;
  s32 t66, t69, t71, t72, t73, t74, t75, t77, t78, t82;
  s32 t90, t91, t92, t95, t98, t108, t109, t110, t113, t114;
  s32 t115, t116, t126;

  t0 = FIELD_AT(0x3000000, s32, 108);
  p5 = t0;
  t1 = FIELD_AT((s32)gPartyState, s32, 532);
  t2 = Value1((s32 (*)())Object_GetById, t1);
  t3 = *(u16 *)0x4000052;
  v_r9 = ((s32)spo_ + 106);
  *(u16 *)v_r9 = t3;
  FIELD_AT(sp_, s32, 16) = 0;
  FIELD_AT(sp_, s32, 12) = 0;
  t4 = FIELD_AT(p5, u16, 340);
  p4 = (((1 & (((t4 << 16) >> 16) - 50)) << 8) + 0x198);
  FIELD_AT(sp_, s32, 8) = 0;
  FIELD_AT(sp_, s32, 20) = (((((2 & (((t4 << 16) >> 16) - 50)) << 1) +
                              (2 & (((t4 << 16) >> 16) - 50)))
                             << 5) +
                            168);
  v_r6 = 0;
  v_r0 = ((t4 << 16) >> 16);
  v_r8 = t2;
  v_r10 = (((t4 + 193) << 16) >> 16);
  v_r11 = 0;
  if (v_r0 == 51)
    goto L_02008b70;
  if (v_r0 > 51)
    goto L_02008b54;
  if (v_r0 == 50)
    goto L_02008b5e;
  goto L_02008bcc;
L_02008b54:
  if (v_r0 == 52)
    goto L_02008b86;
  if (v_r0 == 53)
    goto L_02008b9e;
  goto L_02008bcc;
L_02008b5e:
  FIELD_AT(sp_, s32, 16) = 0x208;
  FIELD_AT(sp_, s32, 12) = 48;
  v_r6 = 48;
  v_r0 = 0xa87;
  goto L_02008bb4;
L_02008b70:
  FIELD_AT(sp_, s32, 12) = 48;
  FIELD_AT(sp_, s32, 8) = 3;
  v_r6 = 68;
  v_r0 = 0xa88;
  v_r5 = 0x228;
  goto L_02008bb2;
L_02008b86:
  FIELD_AT(sp_, s32, 8) = 10;
  FIELD_AT(sp_, s32, 16) = 0x1f8;
  FIELD_AT(sp_, s32, 12) = 40;
  v_r6 = 88;
  v_r0 = 0xa89;
  goto L_02008bb4;
L_02008b9e:
  FIELD_AT(sp_, s32, 12) = 40;
  FIELD_AT(sp_, s32, 8) = 2;
  v_r6 = 108;
  v_r0 = 0xa8a;
  v_r5 = 0x238;
L_02008bb2:
  FIELD_AT(sp_, s32, 16) = v_r5;
L_02008bb4:
  t5 = Value1((s32 (*)())GameFlag_Test, v_r0);
  if (t5 == 0)
    goto L_02008bcc;
  Call1((void (*)())GameFlag_SetBit, 0x200);
  goto L_02009078;
L_02008bcc:
  v_r5 = (v_r6 << 16);
  v_r7 = ((u32)v_r5 >> 16);
  if (v_r7 == (v_r7 + 18))
    goto L_02008bfe;
L_02008bd8:
  t7 = Value1((s32 (*)())GameFlag_Test, v_r7);
  if (t7 == 0)
    goto L_02008bf4;
  *(u8 *)(((s32)sp_ + 76) + v_r11) = (v_r7 - v_r6);
  v_r11 = ((u32)((v_r11 + 1) << 24) >> 24);
L_02008bf4:
  v_r7 = (v_r7 + 1);
  if (v_r7 != (((u32)v_r5 >> 16) + 18))
    goto L_02008bd8;
L_02008bfe:
  if (v_r11 != 0)
    goto L_02008c06;
  goto L_02009078;
L_02008c06:
  Func_02003398();
  Call1((void (*)())Func_02003488, 0);
  v_r5 = ((s32)gPartyState + 0x214);
  t10 = *(s32 *)v_r5;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, t10, 0xcccc, 0x6666);
  Call2((void (*)())Func_02003420, 0xcccc, 0x1999);
  t13 = *(s32 *)v_r5;
  t14 = FIELD_AT(sp_, s32, 20);
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, t13, p4, t14);
  t16 = FIELD_AT(sp_, s32, 20);
  Call4((void (*)())Camera_MoveWithinMapBounds, (p4 << 16), -1, (t16 << 16), 1);
  t18 = *(s32 *)v_r5;
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, t18);
  t20 = *(s32 *)v_r5;
  Call2((void (*)())Func_02003400, t20, 0x4000);
  t22 = *(s32 *)v_r5;
  Call2((void (*)())Object_SetModeById, t22, 28);
  t24 = FIELD_AT(v_r8, s32, 12);
  t25 = FIELD_AT(v_r8, s32, 8);
  t26 = FIELD_AT(v_r8, s32, 16);
  t27 = Value4((s32 (*)())Engine_ObjectCreate, ((u32)(v_r10 << 16) >> 16), t25,
               (t24 + 0x80000), t26);
  FIELD_AT(t27, u8, 85) = 0;
  FIELD_AT(t27, s32, 28) = 0x1999;
  FIELD_AT(t27, s32, 24) = 0x1999;
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t27, 0);
  Call2((void (*)())Func_020032e8, t27, (s32)gData_020035cc);
  Call2((void (*)())Animation_SetStateFlags, t27, 1);
  Call2((void (*)())Func_02003460, 0x10000, 0);
  Call2((void (*)())Func_02003458, 0x10003, 1);
  Call1((void (*)())Func_02003468, 60);
  Func_02003338();
  t35 = *(u8 *)v_r9;
  *(u16 *)v_r9 = t35;
  *(u16 *)0x4000052 = 0x1000;
  t36 = FIELD_AT(sp_, s32, 20);
  FIELD_AT(sp_, s32, 4) = (p4 >> 4);
  *(s32 *)sp_ = (t36 >> 4);
  Call6((void (*)())Func_02003328, ((p4 >> 4) + 63), ((t36 >> 4) + 23),
        ((p4 >> 4) + 63), ((t36 >> 4) - 3), 3, 5);
  t38 = *(u16 *)v_r9;
  v_r7 = 0;
  v_r10 = t27;
  v_r3 = t38;
  goto L_02008d1c;
L_02008d1c:
  if (v_r3 == 0)
    goto L_02008d44;
L_02008d20:
  *(u16 *)0x4000052 = (v_r7 | 0x1000);
  Call1((void (*)())WaitFrames, 8);
  t40 = *(u16 *)v_r9;
  v_r7 = (v_r7 + 1);
  if (v_r7 != t40)
    goto L_02008d20;
  goto L_02008d44;
L_02008d44:
  Func_02003330();
  Call1((void (*)())Func_020034b8, 246);
  Call1((void (*)())FieldEvent_WaitFrames, 48);
  v_r7 = 0;
  v_r5 = ((u32)((v_r11 + 255) << 24) >> 24);
  if (v_r11 == v_r7)
    goto L_02008e18;
L_02008d64:
  t44 = Value0((s32 (*)())Random16Far);
  t45 = *(u8 *)(((s32)sp_ + 76) + (t44 & v_r5));
  *(u8 *)(((s32)sp_ + 56) + v_r7) = t45;
  t46 = *(u8 *)(((s32)sp_ + 76) + v_r5);
  *(u8 *)(((s32)sp_ + 76) + (t44 & v_r5)) = t46;
  v_r7 = (v_r7 + 1);
  v_r5 = ((u32)((v_r5 + 255) << 24) >> 24);
  if (v_r7 != v_r11)
    goto L_02008d64;
  v_r7 = 0;
  if (v_r11 == v_r7)
    goto L_02008e18;
L_02008d8c:
  t47 = Value0((s32 (*)())Random16Far);
  t48 = FIELD_AT(v_r8, s32, 8);
  FIELD_AT(sp_, s32, 44) = ((t48 + ((31 & t47) << 16)) - 0x100000);
  t49 = Value0((s32 (*)())Random16Far);
  t50 = FIELD_AT(v_r8, s32, 12);
  FIELD_AT(sp_, s32, 48) = ((t50 + ((t49 & 15) << 16)) + 0x80000);
  t51 = Value0((s32 (*)())Random16Far);
  t52 = FIELD_AT(v_r8, s32, 16);
  FIELD_AT(sp_, s32, 52) = ((t52 + ((t51 & 15) << 16)) - 0x80000);
  t53 = *(u8 *)(((s32)sp_ + 56) + v_r7);
  t54 = *(s32 *)((s32)gData_020036fc + (t53 << 3));
  t55 = FIELD_AT(v_r8, s32, 8);
  FIELD_AT(sp_, s32, 32) = (t55 + (t54 << 16));
  t56 = FIELD_AT(v_r8, s32, 12);
  FIELD_AT(sp_, s32, 36) = t56;
  t57 = *(s32 *)((s32)gData_020036fc + ((t53 << 3) + 4));
  t58 = FIELD_AT(v_r8, s32, 16);
  FIELD_AT(sp_, s32, 40) = (t58 + (t57 << 16));
  t59 = FIELD_AT(sp_, s32, 8);
  Call3((void (*)())Func_0200088c, ((s32)sp_ + 44), ((s32)sp_ + 32), t59);
  if ((1 & v_r7) == 0)
    goto L_02008e0c;
  Call1((void (*)())Func_020034b8, 195);
L_02008e0c:
  Call1((void (*)())FieldEvent_WaitFrames, 8);
  v_r7 = (v_r7 + 1);
  if (v_r7 != v_r11)
    goto L_02008d8c;
L_02008e18:
  if (v_r11 == 18)
    goto L_02008eea;
  Call1((void (*)())FieldEvent_WaitFrames, 96);
  Call2((void (*)())Func_020032e8, v_r10, (s32)gData_02003620);
  Func_02003338();
  t66 = *(u16 *)0x4000052;
  FIELD_AT(sp_, u16, 98) = (255 & t66);
  Call2((void (*)())Func_02003458, 0x10000, 1);
  Call1((void (*)())Func_02003468, 60);
  t69 = FIELD_AT(sp_, u16, 98);
  v_r7 = t69;
  if (v_r7 == 0)
    goto L_02008e72;
L_02008e58:
  *(u16 *)0x4000052 = (v_r7 | 0x1000);
  Call1((void (*)())WaitFrames, 8);
  v_r7 = (v_r7 - 1);
  if (v_r7 != 0)
    goto L_02008e58;
L_02008e72:
  v_r7 = 0;
  if (v_r11 == v_r7)
    goto L_02008ec8;
L_02008e7a:
  t71 = *(u8 *)(((s32)sp_ + 56) + v_r7);
  t72 = *(s32 *)((s32)gData_020036fc + (t71 << 3));
  t73 = FIELD_AT(v_r8, s32, 8);
  t74 = *(s32 *)((s32)gData_020036fc + ((t71 << 3) + 4));
  v_r1 = t74;
  v_r4 = v_r8;
  v_r2 = t73;
  v_r3 = t72;
  goto L_02008ea4;
L_02008ea4:
  t75 = FIELD_AT(v_r4, s32, 16);
  Call6((void (*)())Func_02003328, 64, 0, (((v_r2 + (v_r3 << 16)) >> 20) + 64),
        ((t75 + (v_r1 << 16)) >> 20), 1, 1);
  v_r7 = (v_r7 + 1);
  if (v_r7 != v_r11)
    goto L_02008e7a;
L_02008ec8:
  t77 = FIELD_AT(sp_, s32, 4);
  t78 = *(s32 *)sp_;
  Call6((void (*)())Func_02003328, 64, 0, (t77 + 63), (t78 - 3), 3, 5);
  Func_02003330();
  Event_WaitTransitionFrames();
  goto L_0200904c;
L_02008eea:
  t82 = FIELD_AT(p5, s16, 340);
  Call1((void (*)())GameFlag_SetBit, (t82 + 0xa55));
  Call2((void (*)())Func_020032e8, v_r10, (s32)gData_02003620);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call2((void (*)())Func_02003420, 0x40000, 0x8000);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x2180000, -1, 0x780000, 1);
  Func_02003430();
  Call1((void (*)())Func_020034b8, 220);
  t90 = FIELD_AT(sp_, s32, 16);
  t91 = FIELD_AT(sp_, s32, 12);
  t92 = FIELD_AT(sp_, s32, 8);
  v_r6 = (t91 << 16);
  v_r5 = (t90 << 16);
  Call4((void (*)())Func_020009b8, v_r5, v_r6, t92, 0x800);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  t95 = FIELD_AT(sp_, s32, 8);
  Call4((void (*)())Func_020009b8, v_r5, v_r6, t95, 0x900);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  t98 = FIELD_AT(sp_, s32, 8);
  Call4((void (*)())Func_020009b8, v_r5, v_r6, t98, 0xa00);
  Call1((void (*)())FieldEvent_WaitFrames, 12);
  Call1((void (*)())WaitFrames, 16);
  Call2((void (*)())Func_02003458, 0x7fff, 0);
  Call1((void (*)())Func_02003468, 1);
  Call1((void (*)())WaitFrames, 1);
  Call2((void (*)())Func_02003458, 0x10000, 0);
  Call1((void (*)())Func_02003468, 32);
  Call1((void (*)())Func_020034b8, 0x23e);
  t108 = FIELD_AT(sp_, s32, 16);
  t109 = FIELD_AT(sp_, s32, 12);
  t110 = FIELD_AT(sp_, s32, 8);
  Call3((void (*)())Func_02000a78, t108, t109, t110);
  Call1((void (*)())FieldEvent_WaitFrames, 80);
  t113 = Value1((s32 (*)())GameFlag_Test, 0xa87);
  if (t113 == 0)
    goto L_0200904c;
  t114 = Value1((s32 (*)())GameFlag_Test, 0xa88);
  if (t114 == 0)
    goto L_0200904c;
  t115 = Value1((s32 (*)())GameFlag_Test, 0xa89);
  if (t115 == 0)
    goto L_0200904c;
  t116 = Value1((s32 (*)())GameFlag_Test, 0xa8a);
  if (t116 == 0)
    goto L_0200904c;
  Call1((void (*)())GameFlag_SetBit, 0xa8b);
  Call1((void (*)())Func_020034b8, 188);
  Call6((void (*)())Func_02003328, 52, 7, 33, 7, 1, 2);
  Call1((void (*)())WaitFrames, 5);
  Call6((void (*)())Func_02003328, 54, 7, 33, 7, 1, 2);
  Call4((void (*)())Func_02003380, 0x2180000, 0x880000, 0, 0);
  Call1((void (*)())WaitFrames, 60);
L_0200904c:
  Call1((void (*)())Func_020032f8, v_r10);
  Call1((void (*)())GameFlag_SetBit, 0x200);
  t126 = FIELD_AT((s32)gPartyState, s32, 532);
  Call2((void (*)())Camera_FollowActor, t126, 1);
  Func_02003430();
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Func_020033a0();
L_02009078:
  return;
}
