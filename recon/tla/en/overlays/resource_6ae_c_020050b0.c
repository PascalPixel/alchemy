// Draft of overlay 6ae's Func_020050b0, translated from its listing: differ
// 151 of 120. Left: its shape: 117 instructions against the listing's 120.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
extern u8 Data_02006cd4[];
extern u8 Data_02006ce4[];
extern u8 Data_02006ce8[];
extern u8 gData_02006cd0[];
extern u8 gData_02006cd8[];
extern u8 gData_02006cdc[];
extern u8 gData_02006ce0[];
extern u8 gData_02006cec[];
extern u8 gData_02006cf0[];

s32 Func_020050b0(s32 p0) {
  s32 v_r4, v_r0, v_r2, v_r1, v_r6, v_r5, v_r14, v_r7, v_r12, v_r3;
  s32 t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
  s32 t10, t11, t12, t13, t14, t15, t16, t17, t18, t19;
  s32 t20, t21, t22;

  t0 = *(u16 *)(s32)gData_02006cd8;
  v_r1 = (s32)gData_02006cd8;
  v_r2 = t0;
  v_r0 = p0;
  v_r4 = 0;
  if (v_r2 == v_r4)
    goto L_0200d0ea;
  *(u16 *)v_r1 = (v_r2 + 0xffff);
  v_r0 = 0xffff;
  if (((v_r2 + 0xffff) << 16) != 0)
    goto L_0200d0ea;
  t1 = *(u16 *)(s32)gData_02006cdc;
  *(u16 *)v_r1 = t1;
  v_r2 = t1;
  v_r0 = (s32)gData_02006cdc;
  v_r4 = 1;
  if ((u32)(t1 << 16) <= (u32)0x20000)
    goto L_0200d0ea;
  *(u16 *)v_r0 = (v_r2 + 0xffff);
L_0200d0ea:
  if (v_r4 != 0)
    goto L_0200d0f0;
  goto L_0200d1dc;
L_0200d0f0:
  t2 = *(s32 *)(s32)Data_02006ce8;
  t3 = *(u16 *)(s32)gData_02006cec;
  v_r1 = t2;
  v_r5 = 0;
  v_r6 = (s32)gData_02006cec;
  if ((u32)v_r5 >= (u32)t3)
    goto L_0200d13e;
  t4 = *(s32 *)(s32)Data_02006cd4;
  v_r12 = v_r6;
  v_r7 = t4;
  v_r14 = (s32)gData_02006cf0;
L_0200d108:
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
    goto L_0200d108;
L_0200d13e:
  t12 = *(u16 *)(s32)gData_02006cec;
  t13 = *(s32 *)(s32)gData_02006ce0;
  t14 = *(u16 *)((t12 << 1) + t13);
  t15 = *(u16 *)(s32)gData_02006cd0;
  v_r4 = ((t14 << 1) + 0x5000000);
  v_r2 = ((t12 << 1) + t12);
  if (t15 == 0)
    goto L_0200d184;
  v_r3 = (s32)Data_02006ce4;
  goto L_0200d186;
L_0200d184:
  v_r3 = (s32)Data_02006ce8;
L_0200d186:
  t16 = *(s32 *)v_r3;
  t17 = *(u16 *)(t16 + (v_r2 << 1));
  t18 = *(u16 *)((t16 + (v_r2 << 1)) + 2);
  t19 = FIELD_AT(((t16 + (v_r2 << 1)) + 2), u16, 2);
  *(u16 *)v_r4 = (((t19 << 10) | (t18 << 5)) | t17);
  t20 = *(u16 *)(s32)gData_02006cd0;
  *(u16 *)(s32)gData_02006cd0 = (t20 ^ 1);
  t21 = *(u16 *)(s32)gData_02006cf0;
  *(u16 *)(s32)gData_02006cf0 = (t21 + 1);
  t22 = *(u16 *)(s32)gData_02006cec;
  v_r1 = (s32)gData_02006cf0;
  v_r0 = t17;
  if (((u32)((t21 + 1) << 16) >> 16) != t22)
    goto L_0200d1dc;
  *(u16 *)v_r1 = 0;
  goto L_0200d1dc;
L_0200d1dc:
  return v_r0;
}
