// Draft of overlay 6aa's Func_020047b8, translated from its listing: differ
// 151 of 120. Left: its shape: 117 instructions against the listing's 120.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
extern u8 Data_02006880[];
extern u8 Data_02006894[];
extern u8 gData_0200687c[];
extern u8 gData_02006884[];
extern u8 gData_02006890[];
extern u8 gData_02006898[];
extern u8 gData_0200689c[];
extern u8 gData_0200e888[];
extern u8 gData_0200e88c[];

s32 Func_020047b8(s32 p0) {
  s32 v_r4, v_r0, v_r2, v_r1, v_r6, v_r5, v_r14, v_r7, v_r12, v_r3;
  s32 t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
  s32 t10, t11, t12, t13, t14, t15, t16, t17, t18, t19;
  s32 t20, t21, t22;

  t0 = *(u16 *)(s32)gData_02006884;
  v_r1 = (s32)gData_02006884;
  v_r2 = t0;
  v_r0 = p0;
  v_r4 = 0;
  if (v_r2 == v_r4)
    goto L_0200c7f2;
  *(u16 *)v_r1 = (v_r2 + 0xffff);
  v_r0 = 0xffff;
  if (((v_r2 + 0xffff) << 16) != 0)
    goto L_0200c7f2;
  t1 = *(u16 *)(s32)gData_0200e888;
  *(u16 *)v_r1 = t1;
  v_r2 = t1;
  v_r0 = (s32)gData_0200e888;
  v_r4 = 1;
  if ((u32)(t1 << 16) <= (u32)0x20000)
    goto L_0200c7f2;
  *(u16 *)v_r0 = (v_r2 + 0xffff);
L_0200c7f2:
  if (v_r4 != 0)
    goto L_0200c7f8;
  goto L_0200c8e4;
L_0200c7f8:
  t2 = *(s32 *)(s32)Data_02006894;
  t3 = *(u16 *)(s32)gData_02006898;
  v_r1 = t2;
  v_r5 = 0;
  v_r6 = (s32)gData_02006898;
  if ((u32)v_r5 >= (u32)t3)
    goto L_0200c846;
  t4 = *(s32 *)(s32)Data_02006880;
  v_r12 = v_r6;
  v_r7 = t4;
  v_r14 = (s32)gData_0200689c;
L_0200c810:
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
    goto L_0200c810;
L_0200c846:
  t12 = *(u16 *)(s32)gData_02006898;
  t13 = *(s32 *)(s32)gData_0200e88c;
  t14 = *(u16 *)((t12 << 1) + t13);
  t15 = *(u16 *)(s32)gData_0200687c;
  v_r4 = ((t14 << 1) + 0x5000000);
  v_r2 = ((t12 << 1) + t12);
  if (t15 == 0)
    goto L_0200c88c;
  v_r3 = (s32)gData_02006890;
  goto L_0200c88e;
L_0200c88c:
  v_r3 = (s32)Data_02006894;
L_0200c88e:
  t16 = *(s32 *)v_r3;
  t17 = *(u16 *)(t16 + (v_r2 << 1));
  t18 = *(u16 *)((t16 + (v_r2 << 1)) + 2);
  t19 = FIELD_AT(((t16 + (v_r2 << 1)) + 2), u16, 2);
  *(u16 *)v_r4 = (((t19 << 10) | (t18 << 5)) | t17);
  t20 = *(u16 *)(s32)gData_0200687c;
  *(u16 *)(s32)gData_0200687c = (t20 ^ 1);
  t21 = *(u16 *)(s32)gData_0200689c;
  *(u16 *)(s32)gData_0200689c = (t21 + 1);
  t22 = *(u16 *)(s32)gData_02006898;
  v_r1 = (s32)gData_0200689c;
  v_r0 = t17;
  if (((u32)((t21 + 1) << 16) >> 16) != t22)
    goto L_0200c8e4;
  *(u16 *)v_r1 = 0;
  goto L_0200c8e4;
L_0200c8e4:
  return v_r0;
}
