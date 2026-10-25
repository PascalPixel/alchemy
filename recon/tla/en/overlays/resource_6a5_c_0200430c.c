// Draft of overlay 6a5's Func_0200430c, translated from its listing: differ
// 172 of 197. Left: its shape: 183 instructions against the listing's 197.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020042e0();
s32 Func_02005764();
s32 Func_02005994();
s32 Func_020059e4();
s32 Func_020059f4();
s32 GameFlag_Test();
s32 Math_Cosine();
s32 Math_Sine();
s32 ObjectDispatch_SetAnimationSpeed();
s32 Object_GetById();
extern u8 Data_02006a28[];
extern u8 IwramMulQ16[];
extern u8 gData_020069fc[];
extern u8 gPartyState[];

void Func_0200430c(s32 p0) {
  s32 sp_[1];
  s32 v_r6, v_r10, v_r7, v_r5, v_r9, v_r3, v_r2, v_r8, v_r0, t0;
  s32 t1, t2, t3, t4, t5, t6, t9, t11, t12, t13;
  s32 t14, t15, t16, t17, t18, t19, t20, t22, t23, t24;
  s32 t25, t26, t27, t28, t29, t30, t31, t32, t33, t34;
  s32 t35, t36, t37;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = *(s32 *)((s32)gPartyState + 0x214);
  t2 = Object_GetById(t1);
  t3 = *(s16 *)(0x166 + t0);
  v_r7 = t2;
  v_r10 = t0;
  v_r6 = p0;
  if (t3 != 0)
    goto L_0200c36a;
  t4 = *(s16 *)(0x15a + v_r10);
  if (t4 != 0)
    goto L_0200c36a;
  t5 = *(s16 *)(0x15e + v_r10);
  if (t5 != 0)
    goto L_0200c36a;
  t6 = *(s16 *)(0x168 + v_r10);
  if (t6 == 0)
    goto L_0200c378;
L_0200c36a:
  ObjectDispatch_SetAnimationSpeed(v_r6, 0);
  goto L_0200c4b6;
L_0200c378:
  ObjectDispatch_SetAnimationSpeed(v_r6, 16);
  t9 = *(u16 *)(v_r6 + 100);
  *(u16 *)(v_r6 + 100) = (t9 + 1);
  if ((31 & (t9 + 1)) != 31)
    goto L_0200c398;
  Func_020059f4(231);
L_0200c398:
  t11 = FIELD_AT(v_r7, s32, 80);
  t12 = FIELD_AT(v_r6, s32, 80);
  t13 = FIELD_AT(t11, u8, 9);
  t14 = FIELD_AT(t12, u8, 9);
  FIELD_AT(t12, u8, 9) = ((-13 & t14) | (12 & t13));
  t15 = FIELD_AT(v_r6, s32, 8);
  t16 = FIELD_AT(v_r6, s32, 16);
  t17 = Func_020059e4(t15, t16, 2);
  if (t17 == 255)
    goto L_0200c49a;
  t18 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(sp_, s32, 0) = t18;
  t19 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(sp_, s32, 4) = t19;
  t20 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(sp_, s32, 8) = t20;
  Func_02005994(((s32)sp_ + 0));
  t22 = FIELD_AT(sp_, s32, 0);
  v_r5 = t22;
  if (v_r5 > 0x1100000)
    goto L_0200c49a;
  if (v_r5 < 0xffe00000)
    goto L_0200c49a;
  t23 = *(u8 *)(98 + v_r6);
  v_r9 = (98 + v_r6);
  if (t23 != 0)
    goto L_0200c460;
  t24 = FIELD_AT(v_r7, s32, 12);
  t25 = FIELD_AT(v_r6, s32, 12);
  v_r5 = (t24 - t25);
  v_r2 = t24;
  v_r3 = t25;
  if (v_r5 >= 0)
    goto L_0200c3f8;
  v_r5 = (v_r3 - v_r2);
L_0200c3f8:
  t26 = Func_020042e0((v_r7 + 8), (v_r6 + 8));
  v_r8 = 0;
  if (t26 > 12)
    goto L_0200c418;
  if (v_r5 >= 0xc0000)
    goto L_0200c418;
  v_r8 = 1;
L_0200c418:
  if (v_r8 == 0)
    goto L_0200c460;
  t27 = GameFlag_Test(0x104);
  if (t27 != 0)
    goto L_0200c460;
  t28 = FIELD_AT(v_r6, u16, 6);
  FIELD_AT(v_r7, s32, 104) = v_r6;
  FIELD_AT(v_r7, u16, 6) = t28;
  t29 = *(u8 *)(v_r7 + 35);
  *(u8 *)(v_r7 + 35) = (254 & t29);
  *(u16 *)(0x16a + v_r10) = 200;
  *(u8 *)((s32)gPartyState + 0x212) = 0;
  *(u8 *)v_r9 = 1;
  *(u8 *)(v_r6 + 99) = 1;
L_0200c460:
  t30 = FIELD_AT(v_r6, u16, 6);
  t31 = Math_Cosine(t30);
  t32 = FIELD_AT(v_r6, s32, 48);
  t33 = FIELD_AT(v_r6, s32, 8);
  t34 = FIELD_AT(v_r6, u16, 6);
  FIELD_AT(v_r6, s32, 8) = (t33 + t31);
  t35 = Math_Sine(t34, t32);
  t36 = FIELD_AT(v_r6, s32, 48);
  t37 = FIELD_AT(v_r6, s32, 16);
  v_r0 = t35;
  v_r3 = t37;
  goto L_0200c494;
L_0200c494:
  FIELD_AT(v_r6, s32, 16) = (v_r3 + v_r0);
  goto L_0200c4b6;
L_0200c49a:
  *(u8 *)(v_r6 + 99) = 0;
  FIELD_AT(v_r6, s32, 108) = 0;
  Func_02005764(v_r6, (s32)gData_020069fc);
  Func_020059f4(228);
  *(s32 *)(s32)Data_02006a28 = 0;
L_0200c4b6:
  return;
}
