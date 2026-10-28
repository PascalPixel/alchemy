// Draft of overlay 6ab's Func_02003390, translated from its listing: differ
// 324 of 246. Left: its shape: 266 instructions against the listing's 246.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02002d90();
s32 Func_020041cc();
s32 Func_02004204();
s32 Func_0200420c();
s32 Func_02004224();
s32 Func_0200428c();
s32 Map_GetTerrainHeight();
s32 Actor_SetAnimationSpeed();
s32 ObjectTable_Get();
s32 Vector_AddPolarOffsetFar();
s32 wait();
extern u8 Func_02002d04[];
extern u8 gMainRoutineWork[];

void Func_02003390(void) {
  s32 sp_[1];
  s32 v_r5, v_r6, v_r2, v_r1, v_r8, v_r9, v_r7, v_r11, v_r10, t0;
  s32 t1, t2, t4, t5, t6, t7, t8, t9, t11, t12;
  s32 t13, t14, t15, t16, t17, t18, t19, t20, t21, t24;
  s32 t25, t26, t27, t28, t29, t30, t31, t32, t33, t34;
  s32 t35, t36, t37, t38, t39, t40, t41, t44, t45, t47;
  s32 t48, t49, t50, t51, t52, t53, t58;

  t0 = *(s32 *)((s32)gMainRoutineWork + 0x214);
  t1 = ObjectTable_Get(t0);
  t2 = FIELD_AT(t1, u16, 6);
  FIELD_AT(sp_, s32, 16) = ((((t2 + 0x2000) & -0x4000) << 16) >> 16);
  v_r5 = t1;
L_0200b3c2:
  Func_02002d90();
  FIELD_AT(v_r5, s32, 48) = 0x10000;
  FIELD_AT(v_r5, s32, 52) = 0x8000;
  FIELD_AT(v_r5, s32, 56) = 0x80000000;
  FIELD_AT(v_r5, s32, 64) = 0x80000000;
  FIELD_AT(v_r5, s32, 36) = 0;
  FIELD_AT(v_r5, s32, 44) = 0;
  t4 = FIELD_AT(v_r5, s32, 8);
  FIELD_AT(sp_, s32, 20) = ((t4 & 0xfff00000) + 0x80000);
  v_r9 = 0x80000;
  v_r8 = ((t4 & 0xfff00000) + 0x80000);
  v_r1 = 0x80000;
  v_r2 = 0xfff00000;
  v_r6 = ((s32)sp_ + 20);
  goto L_0200b404;
L_0200b404:
  t5 = FIELD_AT(v_r5, s32, 12);
  FIELD_AT(v_r6, s32, 4) = t5;
  t6 = FIELD_AT(v_r5, s32, 16);
  v_r7 = ((t6 & v_r2) + v_r1);
  FIELD_AT(v_r6, s32, 8) = v_r7;
  FIELD_AT(sp_, s32, 8) = v_r8;
  FIELD_AT(sp_, s32, 4) = v_r7;
  t7 = *(u8 *)(34 + v_r5);
  t8 = Func_02004224(t7, v_r8, v_r7, (34 + v_r5));
  FIELD_AT(sp_, s32, 12) = t8;
  t9 = FIELD_AT(sp_, s32, 16);
  Vector_AddPolarOffsetFar(0x100000, t9, v_r6);
  v_r11 = (34 + v_r5);
  t11 = *(u8 *)v_r11;
  t12 = FIELD_AT(v_r6, s32, 8);
  t13 = *(s32 *)v_r6;
  t14 = Func_02004224(t11, t13, t12, 0);
  if (t14 == 255)
    goto L_0200b498;
  t15 = *(u8 *)v_r11;
  t16 = *(s32 *)v_r6;
  t17 = FIELD_AT(v_r6, s32, 8);
  t18 = Map_GetTerrainHeight(t15, t16, t17);
  t19 = FIELD_AT(v_r5, s32, 12);
  if ((t18 - t19) > v_r9)
    goto L_0200b498;
  t20 = FIELD_AT(sp_, s32, 8);
  *(s32 *)v_r6 = t20;
  t21 = FIELD_AT(sp_, s32, 4);
  FIELD_AT(v_r6, s32, 8) = t21;
  FIELD_AT(v_r5, s32, 48) = 0x20000;
  FIELD_AT(v_r5, s32, 52) = 0x1999;
  *(u16 *)(v_r5 + 100) = 0;
  Func_020041cc(v_r5, 2);
  Actor_SetAnimationSpeed(v_r5, 48);
  FIELD_AT(v_r5, s32, 108) = (s32)Func_02002d04;
  goto L_0200b542;
L_0200b498:
  t24 = FIELD_AT(sp_, u16, 16);
  FIELD_AT(v_r5, u16, 6) = t24;
  FIELD_AT(v_r5, s32, 36) = 0;
  FIELD_AT(v_r5, s32, 44) = 0;
  FIELD_AT(v_r5, s32, 8) = v_r8;
  FIELD_AT(v_r5, s32, 16) = v_r7;
  goto L_0200b58e;
L_0200b4ac:
  t25 = *(u8 *)v_r11;
  t26 = *(s32 *)v_r7;
  t27 = FIELD_AT(v_r7, s32, 8);
  t28 = Map_GetTerrainHeight(t25, t26, t27);
  t29 = FIELD_AT(v_r5, s32, 12);
  if ((t28 - t29) > 0x80000)
    goto L_0200b562;
  t30 = FIELD_AT(v_r5, u16, 32);
  FIELD_AT(sp_, s32, 0) = (t30 - 2);
  t31 = FIELD_AT(0x3000000, s32, 20);
  v_r8 = (89 + t31);
  v_r9 = 0;
  v_r6 = t31;
L_0200b4da:
  t32 = *(s32 *)v_r6;
  if (t32 == 0)
    goto L_0200b504;
  t33 = *(u8 *)v_r8;
  if ((1 & t33) == 0)
    goto L_0200b504;
  if (v_r6 == v_r5)
    goto L_0200b504;
  t34 = FIELD_AT(v_r6, u16, 32);
  t35 = FIELD_AT(sp_, s32, 0);
  t36 = Func_0200428c((v_r6 + 8), t35, ((s32)sp_ + 20), (t34 - 2));
  if (t36 >= 0)
    goto L_0200b562;
L_0200b504:
  v_r9 = (v_r9 + 1);
  v_r8 = (v_r8 + 128);
  v_r6 = (v_r6 + 128);
  if (v_r9 <= 63)
    goto L_0200b4da;
  t37 = *(s32 *)v_r7;
  FIELD_AT(sp_, s32, 8) = t37;
  t38 = FIELD_AT(v_r7, s32, 8);
  FIELD_AT(sp_, s32, 4) = t38;
  FIELD_AT(v_r5, s32, 48) = 0x20000;
  FIELD_AT(v_r5, s32, 52) = 0x1999;
  t39 = *(s32 *)v_r7;
  t40 = FIELD_AT(v_r7, s32, 4);
  t41 = FIELD_AT(v_r7, s32, 8);
  Func_02004204(v_r5, t39, t40, t41);
  Func_0200420c(v_r5);
  t44 = FIELD_AT(sp_, s32, 12);
  if (v_r10 != t44)
    goto L_0200b588;
L_0200b542:
  t45 = FIELD_AT(sp_, s32, 16);
  Vector_AddPolarOffsetFar(0x100000, t45, ((s32)sp_ + 20));
  t47 = *(u8 *)v_r11;
  t48 = FIELD_AT(sp_, s32, 20);
  t49 = FIELD_AT(sp_, s32, 28);
  t50 = Func_02004224(t47, t48, t49, 0);
  v_r10 = t50;
  v_r7 = ((s32)sp_ + 20);
  if (v_r10 != 255)
    goto L_0200b4ac;
L_0200b562:
  FIELD_AT(v_r5, s32, 48) = 0x20000;
  FIELD_AT(v_r5, s32, 52) = 0x10000;
  t51 = FIELD_AT(v_r5, s32, 12);
  t52 = FIELD_AT(sp_, s32, 8);
  t53 = FIELD_AT(sp_, s32, 4);
  Func_02004204(v_r5, t52, t51, t53);
  Func_0200420c(v_r5);
  wait(2);
  goto L_0200b3c2;
L_0200b588:
  wait(10);
L_0200b58e:
  FIELD_AT(v_r5, s32, 108) = 0;
  t58 = *(u8 *)(v_r5 + 90);
  *(u8 *)(v_r5 + 90) = (1 | t58);
  FIELD_AT(v_r5, s32, 52) = 0x4000;
  Func_020041cc(v_r5, 1);
  return;
}
