// Draft of overlay 6a5's Func_02003c58, translated from its listing: differ
// 159 of 149. Left: its shape: 152 instructions against the listing's 149.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 Func_0200577c();
s32 Func_020059f4();
s32 Object_GetById();
s32 WaitTicks();
extern u8 Data_02006a1c[];
extern u8 gData_02005a20[];

void Func_02003c58(s32 p0) {
  s32 sp_[1];
  s32 v_r10, v_r11, v_r9, v_r2, v_r8, v_r5, v_r3, v_r7, v_r6, v_r0;
  s32 t0, t1, t2, t3, t4, t5, t7, t8, t9, t10;
  s32 t11, t12, t13, t14, t15, t16, t17, t18, t21, t22;
  s32 t23;

  FIELD_AT(sp_, s32, 0) = p0;
  t0 = FIELD_AT(0x3000000, s32, 32);
  t1 = *(s32 *)(s32)gData_02005a20;
  v_r8 = 0;
  v_r2 = (s32)gData_02005a20;
  v_r9 = 0x18000;
  v_r11 = 0;
  v_r10 = (0x178 + t0);
  if (t1 == 64)
    goto L_0200bcac;
  v_r5 = v_r2;
L_0200bc8e:
  t2 = *(s32 *)v_r5;
  t3 = Object_GetById(t2);
  *(u8 *)(t3 + 85) = 0;
  v_r8 = (v_r8 + 1);
  v_r5 = (v_r5 + 4);
  v_r11 = (v_r11 + 1);
  if ((u32)v_r8 > (u32)4)
    goto L_0200bcac;
  t4 = *(s32 *)v_r5;
  if (t4 != 64)
    goto L_0200bc8e;
L_0200bcac:
  t5 = FIELD_AT(sp_, s32, 0);
  if (t5 == 0)
    goto L_0200bd74;
  Func_020059f4(223);
  t7 = FIELD_AT(v_r10, s32, 12);
  FIELD_AT(v_r10, s32, 12) = (t7 + 0x18000);
  v_r8 = 0;
  v_r3 = (t7 + 0x18000);
  goto L_0200bd6c;
L_0200bcca:
  v_r7 = 0;
  if ((u32)v_r7 >= (u32)v_r11)
    goto L_0200bcf8;
  v_r6 = (s32)gData_02005a20;
L_0200bcd2:
  t8 = *(s32 *)v_r6;
  t9 = Object_GetById(t8);
  t10 = FIELD_AT(t9, s32, 16);
  FIELD_AT(t9, s32, 16) = (t10 - v_r9);
  t11 = *(s32 *)v_r6;
  t12 = Object_GetById(t11);
  t13 = *(s32 *)v_r6;
  t14 = Object_GetById(t13);
  t15 = FIELD_AT(t14, s32, 16);
  FIELD_AT(t12, s32, 64) = t15;
  v_r7 = (v_r7 + 1);
  v_r6 = (v_r6 + 4);
  if ((u32)v_r7 < (u32)v_r11)
    goto L_0200bcd2;
L_0200bcf8:
  if ((3 & v_r8) != 3)
    goto L_0200bd24;
  t16 = FIELD_AT(v_r10, s32, 12);
  t17 = *(s32 *)(s32)Data_02006a1c;
  v_r0 = ((v_r9 << 4) - v_r9);
  if (t17 >= ((((v_r9 << 4) - v_r9) << 1) + t16))
    goto L_0200bd24;
  v_r3 = v_r0;
  if (v_r3 >= 0)
    goto L_0200bd20;
  v_r3 = (v_r3 + 15);
L_0200bd20:
  v_r9 = (v_r3 >> 4);
L_0200bd24:
  if (v_r9 > 0xccb)
    goto L_0200bd36;
  v_r9 = 0xccc;
L_0200bd36:
  t18 = FIELD_AT(sp_, s32, 0);
  if (t18 != 1)
    goto L_0200bd54;
  if (v_r8 != 40)
    goto L_0200bd54;
  Camera_MoveWithinMapBounds(0x1200000, 0x680000, 0x1700000, 1);
L_0200bd54:
  WaitTicks(1);
  v_r8 = (v_r8 + 1);
  if ((u32)v_r8 > (u32)119)
    goto L_0200bd74;
  t21 = FIELD_AT(v_r10, s32, 12);
  FIELD_AT(v_r10, s32, 12) = (t21 + v_r9);
  v_r3 = (t21 + v_r9);
L_0200bd6c:
  t22 = *(s32 *)(s32)Data_02006a1c;
  if (v_r3 < t22)
    goto L_0200bcca;
L_0200bd74:
  t23 = *(s32 *)(s32)Data_02006a1c;
  FIELD_AT(v_r10, s32, 12) = t23;
  Func_0200577c();
  WaitTicks(2);
  return;
}
