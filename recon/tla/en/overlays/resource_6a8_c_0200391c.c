// Draft of overlay 6a8's Func_0200391c, translated from its listing: differ
// 300 of 304. Left: its shape: 322 instructions against the listing's 304.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 ArcTan2();
s32 Func_020038e8();
s32 Func_020043c4();
s32 Func_020043f4();
s32 Func_020043fc();
s32 Func_02004424();
s32 Func_02004454();
s32 Map_GetTerrainHeight();
s32 Actor_SetAnimationSpeed();
s32 ObjectTable_Get();
s32 Vector_AddPolarOffsetFar();
s32 wait();
extern u8 Func_0200385c[];
extern u8 IwramMulQ16[];
extern u8 gData_02004780[];
extern u8 gInput[];
extern u8 gMainRoutineWork[];

void Func_0200391c(void) {
  s32 sp_[1];
  s32 v_r7, v_r6, v_r5, v_r10, v_r9, v_r11, v_r8, t0, t1, t3;
  s32 t4, t5, t6, t7, t8, t9, t10, t11, t12, t13;
  s32 t14, t15, t16, t17, t18, t19, t20, t21, t22, t24;
  s32 t25, t26, t27, t28, t29, t30, t31, t32, t33, t34;
  s32 t35, t36, t37, t38, t39, t44, t45, t46, t47, t48;
  s32 t49, t50, t51, t52, t53, t54, t55, t56, t57, t58;
  s32 t59, t60, t61, t62, t63, t66, t68, t69, t70, t71;
  s32 t72, t73, t74, t75, t80;

  t0 = FIELD_AT((s32)gMainRoutineWork, s32, 532);
  t1 = Value1((s32 (*)())ObjectTable_Get, t0);
  v_r7 = t1;
L_0200b93c:
  Func_020038e8();
  FIELD_AT(v_r7, s32, 48) = 0x10000;
  FIELD_AT(v_r7, s32, 52) = 0x8000;
  FIELD_AT(v_r7, s32, 56) = 0x80000000;
  FIELD_AT(v_r7, s32, 64) = 0x80000000;
  FIELD_AT(v_r7, s32, 36) = 0;
  FIELD_AT(v_r7, s32, 44) = 0;
  t3 = FIELD_AT(v_r7, s32, 8);
  FIELD_AT(sp_, s32, 12) = ((t3 & 0xfff00000) + 0x80000);
  FIELD_AT(sp_, s32, 28) = ((t3 & 0xfff00000) + 0x80000);
  t4 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(sp_, s32, 32) = t4;
  t5 = FIELD_AT(v_r7, s32, 16);
  FIELD_AT(sp_, s32, 8) = ((t5 & 0xfff00000) + 0x80000);
  FIELD_AT(sp_, s32, 36) = ((t5 & 0xfff00000) + 0x80000);
  t6 = FIELD_AT(sp_, s32, 12);
  FIELD_AT(sp_, s32, 16) = ((t5 & 0xfff00000) + 0x80000);
  FIELD_AT(sp_, s32, 20) = t6;
  FIELD_AT(sp_, s32, 4) = (v_r7 + 34);
  t7 = FIELD_AT(v_r7, u8, 34);
  t8 = FIELD_AT(sp_, s32, 16);
  t9 = Value3((s32 (*)())Func_02004424, t7, t6, t8);
  FIELD_AT(sp_, s32, 24) = t9;
  t10 = FIELD_AT(sp_, s32, 20);
  t11 = FIELD_AT(v_r7, s32, 8);
  t12 = FIELD_AT(sp_, s32, 16);
  t13 = FIELD_AT(v_r7, s32, 16);
  t14 = ((s32 (*)())(s32)IwramMulQ16)((t10 - t11));
  v_r6 = (t12 - t13);
  t15 = ((s32 (*)())(s32)IwramMulQ16)(v_r6, v_r6);
  v_r5 = (t14 + t15);
  v_r10 = ((s32)sp_ + 28);
  if (v_r5 >= 0x40000)
    goto L_0200b9f4;
  t16 = *(s32 *)(s32)gInput;
  t17 = *(s16 *)((s32)gData_02004780 + ((((u32)t16 >> 4) & 15) << 1));
  v_r9 = t17;
  if ((t17 << 16) != 0xffff0000)
    goto L_0200ba20;
  goto L_0200bbb6;
L_0200b9f4:
  t18 = FIELD_AT(sp_, s32, 16);
  t19 = FIELD_AT(sp_, s32, 20);
  t20 = FIELD_AT(v_r7, s32, 16);
  t21 = FIELD_AT(v_r7, s32, 8);
  t22 = Value2((s32 (*)())ArcTan2, (t18 - t20), (t19 - t21));
  v_r9 = ((t22 << 16) >> 16);
  v_r9 = ((((v_r9 + 0x2000) & -0x4000) << 16) >> 16);
  goto L_0200ba20;
L_0200ba20:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, v_r9, ((s32)sp_ + 28));
  t24 = FIELD_AT(sp_, s32, 4);
  t25 = *(s32 *)v_r10;
  t26 = *(u8 *)t24;
  t27 = FIELD_AT(v_r10, s32, 8);
  t28 = Value3((s32 (*)())Func_02004424, t26, t25, t27);
  if (t28 == 255)
    goto L_0200baa2;
  t29 = FIELD_AT(sp_, s32, 4);
  t30 = *(u8 *)t29;
  t31 = *(s32 *)v_r10;
  t32 = FIELD_AT(v_r10, s32, 8);
  t33 = Value3((s32 (*)())Map_GetTerrainHeight, t30, t31, t32);
  t34 = FIELD_AT(v_r7, s32, 12);
  if ((t33 - t34) > 0x80000)
    goto L_0200baa2;
  t35 = FIELD_AT(sp_, s32, 12);
  *(s32 *)v_r10 = t35;
  t36 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r10, s32, 8) = t36;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x1999;
  FIELD_AT(v_r7, u16, 100) = 0;
  t37 = FIELD_AT(sp_, s32, 12);
  t38 = FIELD_AT(sp_, s32, 8);
  t39 = FIELD_AT(v_r7, s32, 12);
  Call4((void (*)())Func_020043f4, v_r7, t37, t39, t38);
  Call2((void (*)())Func_020043c4, v_r7, 2);
  Call2((void (*)())Actor_SetAnimationSpeed, v_r7, 48);
  Call1((void (*)())Func_020043fc, v_r7);
  FIELD_AT(v_r7, s32, 108) = (s32)Func_0200385c;
  goto L_0200bb4c;
L_0200baa2:
  FIELD_AT(v_r7, u16, 6) = v_r9;
  FIELD_AT(v_r7, s32, 36) = 0;
  FIELD_AT(v_r7, s32, 44) = 0;
  t44 = FIELD_AT(sp_, s32, 12);
  FIELD_AT(v_r7, s32, 8) = t44;
  t45 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r7, s32, 16) = t45;
  goto L_0200bb98;
L_0200bab6:
  t46 = FIELD_AT(sp_, s32, 4);
  t47 = *(s32 *)v_r6;
  t48 = *(u8 *)t46;
  t49 = FIELD_AT(v_r6, s32, 8);
  t50 = Value3((s32 (*)())Map_GetTerrainHeight, t48, t47, t49);
  t51 = FIELD_AT(v_r7, s32, 12);
  if ((t50 - t51) > 0x80000)
    goto L_0200bb6c;
  t52 = FIELD_AT(v_r7, u16, 32);
  *(s32 *)sp_ = (t52 - 2);
  t53 = FIELD_AT(0x3000000, s32, 20);
  v_r8 = (t53 + 89);
  v_r10 = 0;
  v_r5 = t53;
L_0200bae4:
  t54 = *(s32 *)v_r5;
  if (t54 == 0)
    goto L_0200bb0e;
  t55 = *(u8 *)v_r8;
  if ((1 & t55) == 0)
    goto L_0200bb0e;
  if (v_r5 == v_r7)
    goto L_0200bb0e;
  t56 = FIELD_AT(v_r5, u16, 32);
  t57 = *(s32 *)sp_;
  t58 = Value4((s32 (*)())Func_02004454, (v_r5 + 8), t57, ((s32)sp_ + 28),
               (t56 - 2));
  if (t58 >= 0)
    goto L_0200bb6c;
L_0200bb0e:
  v_r10 = (v_r10 + 1);
  v_r8 = (v_r8 + 128);
  v_r5 = (v_r5 + 128);
  if (v_r10 <= 63)
    goto L_0200bae4;
  t59 = *(s32 *)v_r6;
  FIELD_AT(sp_, s32, 20) = t59;
  t60 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(sp_, s32, 16) = t60;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x1999;
  t61 = FIELD_AT(v_r6, s32, 8);
  t62 = *(s32 *)v_r6;
  t63 = FIELD_AT(v_r6, s32, 4);
  Call4((void (*)())Func_020043f4, v_r7, t62, t63, t61);
  Call1((void (*)())Func_020043fc, v_r7);
  t66 = FIELD_AT(sp_, s32, 24);
  if (v_r11 != t66)
    goto L_0200bb92;
L_0200bb4c:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, v_r9, ((s32)sp_ + 28));
  t68 = FIELD_AT(sp_, s32, 4);
  t69 = *(u8 *)t68;
  t70 = FIELD_AT(sp_, s32, 28);
  t71 = FIELD_AT(sp_, s32, 36);
  t72 = Value3((s32 (*)())Func_02004424, t69, t70, t71);
  v_r11 = t72;
  v_r6 = ((s32)sp_ + 28);
  if (v_r11 != 255)
    goto L_0200bab6;
L_0200bb6c:
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x10000;
  t73 = FIELD_AT(v_r7, s32, 12);
  t74 = FIELD_AT(sp_, s32, 20);
  t75 = FIELD_AT(sp_, s32, 16);
  Call4((void (*)())Func_020043f4, v_r7, t74, t73, t75);
  Call1((void (*)())Func_020043fc, v_r7);
  Call1((void (*)())wait, 2);
  goto L_0200b93c;
L_0200bb92:
  Call1((void (*)())wait, 10);
L_0200bb98:
  FIELD_AT(v_r7, s32, 108) = 0;
  t80 = FIELD_AT(v_r7, u8, 90);
  FIELD_AT(v_r7, u8, 90) = (1 | t80);
  FIELD_AT(v_r7, s32, 52) = 0x4000;
  Call2((void (*)())Func_020043c4, v_r7, 1);
L_0200bbb6:
  return;
}
