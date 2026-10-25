// Draft of overlay 6b0's Func_020001f8, translated from its listing: differ
// 151 of 120. Left: its shape: 117 instructions against the listing's 120.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
extern u8 Data_02004438[];
extern u8 Data_02004448[];
extern u8 Data_0200444c[];
extern u8 gData_02004434[];
extern u8 gData_0200443c[];
extern u8 gData_02004440[];
extern u8 gData_02004444[];
extern u8 gData_02004450[];
extern u8 gData_02004454[];

s32 Func_020001f8(s32 p0) {
  s32 v_r4, v_r0, v_r2, v_r1, v_r6, v_r5, v_r14, v_r7, v_r12, v_r3;
  s32 t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
  s32 t10, t11, t12, t13, t14, t15, t16, t17, t18, t19;
  s32 t20, t21, t22;

  t0 = *(u16 *)(s32)gData_0200443c;
  v_r1 = (s32)gData_0200443c;
  v_r2 = t0;
  v_r0 = p0;
  v_r4 = 0;
  if (v_r2 == v_r4)
    goto L_02008232;
  *(u16 *)v_r1 = (v_r2 + 0xffff);
  v_r0 = 0xffff;
  if (((v_r2 + 0xffff) << 16) != 0)
    goto L_02008232;
  t1 = *(u16 *)(s32)gData_02004440;
  *(u16 *)v_r1 = t1;
  v_r2 = t1;
  v_r0 = (s32)gData_02004440;
  v_r4 = 1;
  if ((u32)(t1 << 16) <= (u32)0x20000)
    goto L_02008232;
  *(u16 *)v_r0 = (v_r2 + 0xffff);
L_02008232:
  if (v_r4 != 0)
    goto L_02008238;
  goto L_02008324;
L_02008238:
  t2 = *(s32 *)(s32)Data_0200444c;
  t3 = *(u16 *)(s32)gData_02004450;
  v_r1 = t2;
  v_r5 = 0;
  v_r6 = (s32)gData_02004450;
  if ((u32)v_r5 >= (u32)t3)
    goto L_02008286;
  t4 = *(s32 *)(s32)Data_02004438;
  v_r12 = v_r6;
  v_r7 = t4;
  v_r14 = (s32)gData_02004454;
L_02008250:
  t5 = *(u16 *)v_r14;
  t6 = *(u16 *)v_r6;
  t7 = *(u16 *)((((t6 * t5) + v_r5) << 1) + v_r7);
  t8 = *(u16 *)v_r1;
  t9 = *(u16 *)(v_r1 + 2);
  t10 = *(u16 *)((v_r1 + 2) + 2);
  *(u16 *)((t7 << 1) + 0x5000000) = (((t10 << 10) | (t9 << 5)) | t8);
  t11 = *(u16 *)v_r12;
  v_r5 = (v_r5 + 1);
  v_r1 = (((v_r1 + 2) + 2) + 2);
  if ((u32)v_r5 < (u32)t11)
    goto L_02008250;
L_02008286:
  t12 = *(u16 *)(s32)gData_02004450;
  t13 = *(s32 *)(s32)gData_02004444;
  t14 = *(u16 *)((t12 << 1) + t13);
  t15 = *(u16 *)(s32)gData_02004434;
  v_r4 = ((t14 << 1) + 0x5000000);
  v_r2 = ((t12 << 1) + t12);
  if (t15 == 0)
    goto L_020082cc;
  v_r3 = (s32)Data_02004448;
  goto L_020082ce;
L_020082cc:
  v_r3 = (s32)Data_0200444c;
L_020082ce:
  t16 = *(s32 *)v_r3;
  t17 = *(u16 *)(t16 + (v_r2 << 1));
  t18 = *(u16 *)((t16 + (v_r2 << 1)) + 2);
  t19 = FIELD_AT(((t16 + (v_r2 << 1)) + 2), u16, 2);
  *(u16 *)v_r4 = (((t19 << 10) | (t18 << 5)) | t17);
  t20 = *(u16 *)(s32)gData_02004434;
  *(u16 *)(s32)gData_02004434 = (t20 ^ 1);
  t21 = *(u16 *)(s32)gData_02004454;
  *(u16 *)(s32)gData_02004454 = (t21 + 1);
  t22 = *(u16 *)(s32)gData_02004450;
  v_r1 = (s32)gData_02004454;
  v_r0 = t17;
  if (((u32)((t21 + 1) << 16) >> 16) != t22)
    goto L_02008324;
  *(u16 *)v_r1 = 0;
  goto L_02008324;
L_02008324:
  return v_r0;
}
