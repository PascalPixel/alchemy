// Draft of overlay 6ad's Func_02004a8c, translated from its listing: differ
// 246 of 171. Left: its shape: 181 instructions against the listing's 171.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02004bf4();
s32 Func_020054f8();
extern u8 gData_02005f64[];
extern u8 gData_02005f7c[];

s32 Func_02004a8c(s32 p0) {
  s32 sp_[1];
  s32 v_r5, v_r6, v_r4, v_r1, v_r0, v_r2, v_r3, v_r9, v_r10, v_r11;
  s32 v_r8, v_r7, t0, t1, t2, t3, t4, t5, t6, t7;
  s32 t8, t9, t10, t11, t12, t13, t14, t15, t16, t17;
  s32 t18, t19, t20, t21, t22, t23, t24, t25, t26, t27;
  s32 t28, t29, t30, t31;

  v_r5 = (p0 + 12);
  t0 = Func_02004bf4(((s32)sp_ + 24), (p0 + 16), v_r5);
  v_r4 = t0;
  v_r6 = p0;
  if (v_r4 != 0)
    goto L_0200cab6;
  goto L_0200cbd6;
L_0200cab6:
  t1 = *(s32 *)v_r5;
  FIELD_AT(sp_, s32, 20) = t1;
  t2 = *(s8 *)((s32)gData_02005f64 + (t1 << 2));
  v_r2 = t2;
  v_r0 = (s32)gData_02005f64;
  v_r1 = (t1 << 2);
  if (v_r2 >= 0)
    goto L_0200cac6;
  v_r2 = -v_r2;
L_0200cac6:
  t3 = *(s8 *)(v_r0 + (v_r1 + 2));
  v_r3 = t3;
  if (v_r3 >= 0)
    goto L_0200cad0;
  v_r3 = -v_r3;
L_0200cad0:
  FIELD_AT(sp_, s32, 16) = ((v_r2 + v_r3) >> 4);
  t4 = *(s8 *)(v_r0 + (v_r1 + 1));
  v_r2 = t4;
  if (v_r2 >= 0)
    goto L_0200cae0;
  v_r2 = -v_r2;
L_0200cae0:
  t5 = *(s8 *)(v_r0 + (v_r1 + 3));
  v_r3 = t5;
  if (v_r3 >= 0)
    goto L_0200caea;
  v_r3 = -v_r3;
L_0200caea:
  FIELD_AT(sp_, s32, 12) = ((v_r2 + v_r3) >> 4);
  t6 = FIELD_AT(sp_, s32, 24);
  t7 = *(s32 *)((s32)gData_02005f7c + (t6 << 2));
  FIELD_AT(v_r6, s32, 20) = 0;
  FIELD_AT(sp_, s32, 8) = (v_r4 + 34);
  t8 = FIELD_AT(sp_, s32, 8);
  *(u8 *)t8 = 2;
  t9 = FIELD_AT(v_r4, s32, 8);
  v_r9 = (0xffff0000 & t7);
  *(s32 *)v_r6 = (t9 + v_r9);
  t10 = FIELD_AT(v_r4, s32, 16);
  v_r10 = (t7 << 16);
  FIELD_AT(v_r6, s32, 8) = (t10 + v_r10);
  t11 = FIELD_AT(v_r4, s32, 12);
  FIELD_AT(sp_, s32, 32) = t11;
  v_r11 = 0;
L_0200cb28:
  t12 = FIELD_AT(sp_, s32, 20);
  FIELD_AT(sp_, s32, 4) = (t12 << 2);
  t13 = *(s8 *)((s32)gData_02005f64 + ((t12 << 2) + 1));
  t14 = FIELD_AT(v_r6, s32, 8);
  t15 = FIELD_AT(sp_, s32, 12);
  FIELD_AT(sp_, s32, 36) = (t14 + (t13 << 16));
  v_r8 = 0;
  if (v_r8 >= t15)
    goto L_0200cb96;
L_0200cb46:
  t16 = FIELD_AT(sp_, s32, 4);
  t17 = *(s8 *)((s32)gData_02005f64 + t16);
  t18 = *(s32 *)v_r6;
  FIELD_AT(sp_, s32, 28) = (t18 + (t17 << 16));
  t19 = FIELD_AT(sp_, s32, 16);
  v_r7 = 0;
  v_r5 = ((s32)sp_ + 28);
  if (v_r7 >= t19)
    goto L_0200cb80;
L_0200cb5e:
  FIELD_AT(sp_, s32, 0) = v_r4;
  t20 = Func_020054f8(v_r4, ((s32)sp_ + 28));
  t21 = FIELD_AT(sp_, s32, 0);
  v_r4 = t21;
  if (t20 == 2)
    goto L_0200cba8;
  t22 = *(s32 *)v_r5;
  *(s32 *)v_r5 = (t22 + 0x100000);
  t23 = FIELD_AT(sp_, s32, 16);
  v_r7 = (v_r7 + 1);
  if (v_r7 < t23)
    goto L_0200cb5e;
L_0200cb80:
  t24 = FIELD_AT(sp_, s32, 36);
  FIELD_AT(sp_, s32, 36) = (t24 + 0x100000);
  t25 = FIELD_AT(sp_, s32, 12);
  v_r8 = (v_r8 + 1);
  if (v_r8 < t25)
    goto L_0200cb46;
L_0200cb96:
  t26 = *(s32 *)v_r6;
  *(s32 *)v_r6 = (t26 + v_r9);
  t27 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(v_r6, s32, 8) = (t27 + v_r10);
  v_r11 = (v_r11 + 1);
  goto L_0200cb28;
L_0200cba8:
  t28 = FIELD_AT(sp_, s32, 8);
  *(u8 *)t28 = 0;
  v_r0 = 0;
  if (v_r11 == v_r0)
    goto L_0200cbd8;
  t29 = FIELD_AT(v_r4, s32, 8);
  *(s32 *)v_r6 = (t29 + (v_r11 * v_r9));
  t30 = FIELD_AT(v_r4, s32, 12);
  FIELD_AT(v_r6, s32, 4) = t30;
  t31 = FIELD_AT(v_r4, s32, 16);
  FIELD_AT(v_r6, s32, 8) = (t31 + (v_r11 * v_r10));
  v_r0 = 1;
  goto L_0200cbd8;
L_0200cbd6:
  v_r0 = 0;
L_0200cbd8:
  return v_r0;
}
