// Draft of overlay 6ab's Func_02002dc4, translated from its listing: differ
// 346 of 304. Left: its shape: 300 instructions against the listing's 304.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 ArcTan2();
s32 Func_02002d90();
s32 Func_020041cc();
s32 Func_02004204();
s32 Func_0200420c();
s32 Func_02004224();
s32 Func_0200428c();
s32 Map_GetTerrainHeight();
s32 ObjectDispatch_SetAnimationSpeed();
s32 ObjectTable_Get();
s32 Vector_AddPolarOffsetFar();
s32 WaitFrames();
extern u8 Func_02002d04[];
extern u8 IwramMulQ16[];
extern u8 gData_0200483c[];
extern u8 gInput[];
extern u8 gPartyState[];

void Func_02002dc4(void) {
  s32 sp_[1];
  s32 v_r7, v_r6, v_r5, v_r10, v_r9, v_r11, v_r8, t0, t1, t3;
  s32 t4, t5, t6, t7, t8, t9, t10, t11, t12, t13;
  s32 t14, t15, t16, t17, t18, t19, t20, t22, t23, t24;
  s32 t25, t26, t27, t28, t29, t30, t31, t32, t33, t34;
  s32 t35, t36, t37, t42, t43, t44, t45, t46, t47, t48;
  s32 t49, t50, t51, t52, t53, t54, t55, t56, t57, t58;
  s32 t59, t60, t61, t64, t66, t67, t68, t69, t70, t71;
  s32 t72, t73, t78;

  t0 = FIELD_AT((s32)gPartyState, s32, 532);
  t1 = Value1((s32 (*)())ObjectTable_Get, t0);
  v_r7 = t1;
L_0200ade4:
  Func_02002d90();
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
  t9 = Value3((s32 (*)())Func_02004224, t7, t6, t8);
  FIELD_AT(sp_, s32, 24) = t9;
  t10 = FIELD_AT(sp_, s32, 20);
  t11 = FIELD_AT(v_r7, s32, 8);
  t12 = FIELD_AT(sp_, s32, 16);
  t13 = FIELD_AT(v_r7, s32, 16);
  v_r6 = (t12 - t13);
  v_r5 = ((t10 - t11) + v_r6);
  v_r10 = ((s32)sp_ + 28);
  if (v_r5 >= 0x40000)
    goto L_0200ae9c;
  t14 = *(s32 *)(s32)gInput;
  t15 = *(s16 *)((s32)gData_0200483c + ((((u32)t14 >> 4) & 15) << 1));
  v_r9 = t15;
  if ((t15 << 16) != 0xffff0000)
    goto L_0200aec8;
  goto L_0200b05e;
L_0200ae9c:
  t16 = FIELD_AT(sp_, s32, 16);
  t17 = FIELD_AT(sp_, s32, 20);
  t18 = FIELD_AT(v_r7, s32, 16);
  t19 = FIELD_AT(v_r7, s32, 8);
  t20 = Value2((s32 (*)())ArcTan2, (t16 - t18), (t17 - t19));
  v_r9 = ((t20 << 16) >> 16);
  v_r9 = ((((v_r9 + 0x2000) & -0x4000) << 16) >> 16);
  goto L_0200aec8;
L_0200aec8:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, v_r9, ((s32)sp_ + 28));
  t22 = FIELD_AT(sp_, s32, 4);
  t23 = *(s32 *)v_r10;
  t24 = *(u8 *)t22;
  t25 = FIELD_AT(v_r10, s32, 8);
  t26 = Value3((s32 (*)())Func_02004224, t24, t23, t25);
  if (t26 == 255)
    goto L_0200af4a;
  t27 = FIELD_AT(sp_, s32, 4);
  t28 = *(u8 *)t27;
  t29 = *(s32 *)v_r10;
  t30 = FIELD_AT(v_r10, s32, 8);
  t31 = Value3((s32 (*)())Map_GetTerrainHeight, t28, t29, t30);
  t32 = FIELD_AT(v_r7, s32, 12);
  if ((t31 - t32) > 0x80000)
    goto L_0200af4a;
  t33 = FIELD_AT(sp_, s32, 12);
  *(s32 *)v_r10 = t33;
  t34 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r10, s32, 8) = t34;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x1999;
  FIELD_AT(v_r7, u16, 100) = 0;
  t35 = FIELD_AT(sp_, s32, 12);
  t36 = FIELD_AT(sp_, s32, 8);
  t37 = FIELD_AT(v_r7, s32, 12);
  Call4((void (*)())Func_02004204, v_r7, t35, t37, t36);
  Call2((void (*)())Func_020041cc, v_r7, 2);
  Call2((void (*)())ObjectDispatch_SetAnimationSpeed, v_r7, 48);
  Call1((void (*)())Func_0200420c, v_r7);
  FIELD_AT(v_r7, s32, 108) = (s32)Func_02002d04;
  goto L_0200aff4;
L_0200af4a:
  FIELD_AT(v_r7, u16, 6) = v_r9;
  FIELD_AT(v_r7, s32, 36) = 0;
  FIELD_AT(v_r7, s32, 44) = 0;
  t42 = FIELD_AT(sp_, s32, 12);
  FIELD_AT(v_r7, s32, 8) = t42;
  t43 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r7, s32, 16) = t43;
  goto L_0200b040;
L_0200af5e:
  t44 = FIELD_AT(sp_, s32, 4);
  t45 = *(s32 *)v_r6;
  t46 = *(u8 *)t44;
  t47 = FIELD_AT(v_r6, s32, 8);
  t48 = Value3((s32 (*)())Map_GetTerrainHeight, t46, t45, t47);
  t49 = FIELD_AT(v_r7, s32, 12);
  if ((t48 - t49) > 0x80000)
    goto L_0200b014;
  t50 = FIELD_AT(v_r7, u16, 32);
  *(s32 *)sp_ = (t50 - 2);
  t51 = FIELD_AT(0x3000000, s32, 20);
  v_r8 = (t51 + 89);
  v_r10 = 0;
  v_r5 = t51;
L_0200af8c:
  t52 = *(s32 *)v_r5;
  if (t52 == 0)
    goto L_0200afb6;
  t53 = *(u8 *)v_r8;
  if ((1 & t53) == 0)
    goto L_0200afb6;
  if (v_r5 == v_r7)
    goto L_0200afb6;
  t54 = FIELD_AT(v_r5, u16, 32);
  t55 = *(s32 *)sp_;
  t56 = Value4((s32 (*)())Func_0200428c, (v_r5 + 8), t55, ((s32)sp_ + 28),
               (t54 - 2));
  if (t56 >= 0)
    goto L_0200b014;
L_0200afb6:
  v_r10 = (v_r10 + 1);
  v_r8 = (v_r8 + 128);
  v_r5 = (v_r5 + 128);
  if (v_r10 <= 63)
    goto L_0200af8c;
  t57 = *(s32 *)v_r6;
  FIELD_AT(sp_, s32, 20) = t57;
  t58 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(sp_, s32, 16) = t58;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x1999;
  t59 = FIELD_AT(v_r6, s32, 8);
  t60 = *(s32 *)v_r6;
  t61 = FIELD_AT(v_r6, s32, 4);
  Call4((void (*)())Func_02004204, v_r7, t60, t61, t59);
  Call1((void (*)())Func_0200420c, v_r7);
  t64 = FIELD_AT(sp_, s32, 24);
  if (v_r11 != t64)
    goto L_0200b03a;
L_0200aff4:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, v_r9, ((s32)sp_ + 28));
  t66 = FIELD_AT(sp_, s32, 4);
  t67 = *(u8 *)t66;
  t68 = FIELD_AT(sp_, s32, 28);
  t69 = FIELD_AT(sp_, s32, 36);
  t70 = Value3((s32 (*)())Func_02004224, t67, t68, t69);
  v_r11 = t70;
  v_r6 = ((s32)sp_ + 28);
  if (v_r11 != 255)
    goto L_0200af5e;
L_0200b014:
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x10000;
  t71 = FIELD_AT(v_r7, s32, 12);
  t72 = FIELD_AT(sp_, s32, 20);
  t73 = FIELD_AT(sp_, s32, 16);
  Call4((void (*)())Func_02004204, v_r7, t72, t71, t73);
  Call1((void (*)())Func_0200420c, v_r7);
  Call1((void (*)())WaitFrames, 2);
  goto L_0200ade4;
L_0200b03a:
  Call1((void (*)())WaitFrames, 10);
L_0200b040:
  FIELD_AT(v_r7, s32, 108) = 0;
  t78 = FIELD_AT(v_r7, u8, 90);
  FIELD_AT(v_r7, u8, 90) = (1 | t78);
  FIELD_AT(v_r7, s32, 52) = 0x4000;
  Call2((void (*)())Func_020041cc, v_r7, 1);
L_0200b05e:
  return;
}
