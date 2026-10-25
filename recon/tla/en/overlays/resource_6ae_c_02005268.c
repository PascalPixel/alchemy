// Draft of overlay 6ae's Func_02005268, translated from its listing: differ
// 200 of 174. Left: its shape: 186 instructions against the listing's 174.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 Func_0200579c();
s32 Func_020057ac();
s32 Func_020057bc();
s32 Func_020057dc();
s32 Math_Sine();
s32 Resource_ResetEntry();
s32 __umodsi3();

void Func_02005268(s32 p0) {
  s32 sp_[1];
  s32 v_r6, v_r7, v_r8, v_r2, v_r5, v_r11, v_r3, v_r9, v_r10, t0;
  s32 t1, t2, t3, t5, t6, t8, t9, t10, t11, t13;
  s32 t15, t16, t17, t18, t19, t20, t21, t22, t24, t25;
  s32 t26, t27, t28, t29, t30, t31, t32, t34, t35;

  t0 = FIELD_AT(p0, s32, 104);
  v_r6 = (p0 + 99);
  t1 = *(u8 *)v_r6;
  v_r2 = t1;
  v_r8 = t0;
  v_r7 = p0;
  if ((1 & t1) == 0)
    goto L_0200d2a2;
  t2 = *(u8 *)v_r6;
  t3 = __umodsi3(((u32)t2 >> 1), 6, v_r2);
  Animation_ApplyChildValues(v_r7, ((u32)(t3 << 24) >> 24));
L_0200d2a2:
  t5 = FIELD_AT(v_r7, u8, 98);
  v_r5 = t5;
  if (v_r5 != 0)
    goto L_0200d2e0;
  t6 = *(u8 *)v_r6;
  if ((15 & t6) != 0)
    goto L_0200d316;
  Func_020057dc(0x256);
  t8 = FIELD_AT(v_r8, s16, 166);
  *(u16 *)(v_r8 + ((t8 << 1) + 160)) = v_r5;
  t9 = FIELD_AT(v_r8, u16, 166);
  FIELD_AT(v_r8, u16, 166) = (t9 ^ 1);
  goto L_0200d316;
L_0200d2e0:
  if (v_r5 != 1)
    goto L_0200d316;
  t10 = FIELD_AT(v_r8, s16, 160);
  if (t10 <= 31)
    goto L_0200d316;
  t11 = FIELD_AT(v_r8, s16, 162);
  if (t11 <= 31)
    goto L_0200d316;
  Animation_ApplyChildValues(v_r7, 0);
  t13 = FIELD_AT(v_r8, s16, 164);
  Resource_ResetEntry(t13);
  FIELD_AT(v_r7, s32, 108) = 0;
  goto L_0200d3d0;
L_0200d316:
  t15 = *(u8 *)v_r6;
  *(u8 *)v_r6 = (t15 + 1);
  *(s32 *)sp_ = 0;
  v_r11 = 1;
  v_r6 = (v_r8 + 160);
L_0200d328:
  t16 = *(s16 *)v_r6;
  t17 = Math_Sine((t16 << 10));
  FIELD_AT(sp_, s32, 4) = t17;
  t18 = *(s16 *)v_r6;
  v_r3 = t18;
  if (v_r3 < 0)
    goto L_0200d3bc;
  if (v_r3 > 31)
    goto L_0200d3bc;
  t19 = FIELD_AT(v_r7, s32, 8);
  FIELD_AT(sp_, s32, 12) = t19;
  t20 = *(s16 *)v_r6;
  t21 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(sp_, s32, 16) = (t21 + (t20 << 16));
  t22 = FIELD_AT(v_r7, s32, 16);
  FIELD_AT(sp_, s32, 20) = t22;
  Func_0200579c(((s32)sp_ + 12));
  t24 = FIELD_AT(sp_, s32, 12);
  FIELD_AT(sp_, s32, 8) = t24;
  t25 = FIELD_AT(sp_, s32, 20);
  t26 = *(s32 *)sp_;
  v_r10 = 0;
  v_r5 = (t26 + v_r8);
  v_r9 = t25;
L_0200d36c:
  t27 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r5, s32, 16) = v_r9;
  FIELD_AT(v_r5, s32, 12) = t27;
  t28 = FIELD_AT(sp_, s32, 4);
  FIELD_AT(v_r5, s32, 20) = t28;
  FIELD_AT(v_r5, s32, 24) = t28;
  if (v_r10 != 0)
    goto L_0200d38c;
  t29 = Func_020057bc(v_r7);
  FIELD_AT(v_r5, u16, 30) = (t29 - 1);
  goto L_0200d3a4;
L_0200d38c:
  t30 = Func_020057bc(v_r7);
  t31 = FIELD_AT(v_r5, s32, 16);
  FIELD_AT(v_r5, s32, 16) = (t31 - 0x10000);
  t32 = FIELD_AT(v_r5, s32, 24);
  FIELD_AT(v_r5, u16, 30) = (t30 + 1);
  FIELD_AT(v_r5, s32, 24) = -t32;
L_0200d3a4:
  Func_020057ac(v_r5);
  v_r10 = (v_r10 + 1);
  v_r5 = (v_r5 + 40);
  if (v_r10 <= 1)
    goto L_0200d36c;
  t34 = *(u16 *)v_r6;
  *(u16 *)v_r6 = (t34 + 1);
L_0200d3bc:
  t35 = *(s32 *)sp_;
  *(s32 *)sp_ = (t35 + 80);
  v_r11 = (v_r11 - 1);
  v_r6 = (v_r6 + 2);
  if (v_r11 >= 0)
    goto L_0200d328;
L_0200d3d0:
  return;
}
