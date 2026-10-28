// Draft of overlay 6a5's Func_02003a6c, translated from its listing: differ
// 228 of 216. Left: its shape: 228 instructions against the listing's 216.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_FollowActor();
s32 Camera_MoveWithinMapBounds();
s32 Func_02005884();
s32 Func_020059f4();
s32 Actor_SetSpritePriority();
s32 Actor_GetById();
s32 wait();
extern u8 Data_02006a1c[];
extern u8 gData_020059fc[];
extern u8 gData_02005a0c[];
extern u8 gData_02005a14[];
extern u8 gData_02005a1c[];
extern u8 gData_020069dc[];

void Func_02003a6c(s32 p0) {
  s32 sp_[1];
  s32 v_r11, v_r8, v_r0, v_r1, v_r2, v_r9, v_r7, v_r5, v_r10, v_r6;
  s32 t0, t1, t2, t3, t4, t5, t7, t8, t9, t10;
  s32 t11, t12, t15, t16, t17, t18, t19, t20, t21, t22;
  s32 t23, t24, t25, t26, t27, t28, t29, t33, t34, t35;
  s32 t36, t38, t39, t41, t42, t43;

  t0 = FIELD_AT(0x3000000, s32, 32);
  FIELD_AT(sp_, s32, 4) = 0x1999;
  *(s32 *)sp_ = 0;
  t1 = FIELD_AT(t0, s32, 388);
  *(s32 *)(s32)Data_02006a1c = t1;
  v_r0 = p0;
  v_r8 = p0;
  v_r11 = (t0 + 0x178);
  if (v_r0 == 1)
    goto L_0200baba;
  if (v_r0 > 1)
    goto L_0200baac;
  if (v_r0 == 0)
    goto L_0200bab6;
  goto L_0200bac6;
L_0200baac:
  v_r1 = v_r8;
  if (v_r1 == 2)
    goto L_0200bac0;
  if (v_r1 != 3)
    goto L_0200bac6;
L_0200bab6:
  v_r2 = (s32)gData_020059fc;
  goto L_0200bac8;
L_0200baba:
  v_r9 = (s32)gData_02005a0c;
  goto L_0200baca;
L_0200bac0:
  v_r9 = (s32)gData_02005a14;
  goto L_0200baca;
L_0200bac6:
  v_r2 = (s32)gData_02005a1c;
L_0200bac8:
  v_r9 = v_r2;
L_0200baca:
  t2 = *(s32 *)v_r9;
  v_r7 = 0;
  if (t2 == 64)
    goto L_0200bafc;
  v_r5 = v_r9;
L_0200bad6:
  t3 = *(s32 *)v_r5;
  t4 = Actor_GetById(t3);
  FIELD_AT(t4, u8, 85) = 0;
  t5 = *(s32 *)v_r5;
  Actor_SetSpritePriority(t5, 1);
  t7 = *(s32 *)sp_;
  *(s32 *)sp_ = (t7 + 1);
  v_r7 = (v_r7 + 1);
  v_r5 = (v_r5 + 4);
  if ((u32)v_r7 > (u32)4)
    goto L_0200bafc;
  t8 = *(s32 *)v_r5;
  if (t8 != 64)
    goto L_0200bad6;
L_0200bafc:
  v_r7 = 0;
L_0200bafe:
  v_r5 = (v_r7 << 2);
  t9 = *(s32 *)((s32)gData_020069dc + v_r5);
  t10 = Actor_GetById(t9);
  t11 = FIELD_AT(t10, u8, 35);
  FIELD_AT(t10, u8, 35) = (32 | t11);
  t12 = *(s32 *)((s32)gData_020069dc + v_r5);
  Actor_SetSpritePriority(t12, 0);
  v_r7 = (v_r7 + 1);
  if ((u32)v_r7 <= (u32)5)
    goto L_0200bafe;
  Func_020059f4(223);
  v_r7 = 0;
L_0200bb28:
  t15 = FIELD_AT(v_r11, s32, 12);
  t16 = FIELD_AT(sp_, s32, 4);
  FIELD_AT(v_r11, s32, 12) = (t15 - t16);
  t17 = *(s32 *)sp_;
  v_r10 = 0;
  if ((u32)v_r10 >= (u32)t17)
    goto L_0200bb68;
  v_r6 = v_r9;
L_0200bb3e:
  t18 = *(s32 *)v_r6;
  t19 = Actor_GetById(t18);
  t20 = FIELD_AT(t19, s32, 16);
  t21 = FIELD_AT(sp_, s32, 4);
  FIELD_AT(t19, s32, 16) = (t20 + t21);
  t22 = *(s32 *)v_r6;
  t23 = Actor_GetById(t22);
  t24 = *(s32 *)v_r6;
  t25 = Actor_GetById(t24);
  t26 = FIELD_AT(t25, s32, 16);
  FIELD_AT(t23, s32, 64) = t26;
  t27 = *(s32 *)sp_;
  v_r10 = (v_r10 + 1);
  v_r6 = (v_r6 + 4);
  if ((u32)v_r10 < (u32)t27)
    goto L_0200bb3e;
L_0200bb68:
  if ((3 & v_r7) != 3)
    goto L_0200bb7c;
  t28 = FIELD_AT(sp_, s32, 4);
  FIELD_AT(sp_, s32, 4) = (t28 + 0x1999);
L_0200bb7c:
  t29 = FIELD_AT(sp_, s32, 4);
  if (t29 <= 0x17fff)
    goto L_0200bb8a;
  FIELD_AT(sp_, s32, 4) = 0x18000;
L_0200bb8a:
  wait(1);
  if (v_r8 != 0)
    goto L_0200bba2;
  if (v_r7 != 40)
    goto L_0200bbbe;
  v_r2 = 152;
  v_r1 = 208;
  v_r0 = 128;
  goto L_0200bbb2;
L_0200bba2:
  if (v_r8 != 1)
    goto L_0200bbc4;
  if (v_r7 != 40)
    goto L_0200bbbe;
  v_r2 = 176;
  v_r1 = 208;
  v_r0 = 128;
L_0200bbb2:
  Camera_MoveWithinMapBounds((v_r0 << 18), (v_r1 << 15), (v_r2 << 17), 1);
L_0200bbbe:
  if (v_r7 != 120)
    goto L_0200bbda;
  goto L_0200bbe0;
L_0200bbc4:
  if (v_r8 != 3)
    goto L_0200bbda;
  if (v_r7 != 40)
    goto L_0200bbd6;
  Camera_FollowActor(0, 1);
L_0200bbd6:
  if (v_r7 == 120)
    goto L_0200bbe0;
L_0200bbda:
  v_r7 = (v_r7 + 1);
  if ((u32)v_r7 <= (u32)227)
    goto L_0200bb28;
L_0200bbe0:
  v_r7 = 0;
L_0200bbe2:
  v_r5 = (v_r7 << 2);
  t33 = *(s32 *)((s32)gData_020069dc + v_r5);
  t34 = Actor_GetById(t33);
  t35 = FIELD_AT(t34, u8, 35);
  FIELD_AT(t34, u8, 35) = (223 & t35);
  t36 = *(s32 *)((s32)gData_020069dc + v_r5);
  Actor_SetSpritePriority(t36, 1);
  v_r7 = (v_r7 + 1);
  if ((u32)v_r7 <= (u32)5)
    goto L_0200bbe2;
  t38 = *(s32 *)sp_;
  v_r7 = 0;
  if ((u32)v_r7 >= (u32)t38)
    goto L_0200bc2c;
  v_r5 = v_r9;
L_0200bc0e:
  t39 = *(s32 *)v_r5;
  Func_02005884(t39, 0, 0);
  t41 = *(s32 *)v_r5;
  t42 = Actor_GetById(t41);
  FIELD_AT(t42, u8, 85) = 0;
  t43 = *(s32 *)sp_;
  v_r7 = (v_r7 + 1);
  v_r5 = (v_r5 + 4);
  if ((u32)v_r7 < (u32)t43)
    goto L_0200bc0e;
L_0200bc2c:
  return;
}
