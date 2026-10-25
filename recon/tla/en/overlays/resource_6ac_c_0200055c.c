// Draft of overlay 6ac's Func_0200055c, translated from its listing: differ
// 151 of 164. Left: its shape: 177 instructions against the listing's 164.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02003170();
extern u8 Data_02003a7c[];
extern u8 gData_02003a7e[];
extern u8 gData_02003a80[];
extern u8 gIoWriteQueue[];

void Func_0200055c(void) {
  s32 v_r3, v_r2, v_r5, v_r1, v_r6, v_r4, v_r0, t0, t1, t2;
  s32 t3, t4, t5, t6, t8, t10, t12, t14, t15, t17;
  s32 t19, t21;

  t0 = *(s16 *)(s32)Data_02003a7c;
  t1 = *(s16 *)(s32)gData_02003a7e;
  v_r5 = (s32)Data_02003a7c;
  v_r2 = t0;
  v_r3 = t1;
  if (v_r2 != v_r3)
    goto L_02008572;
  goto L_020086b0;
L_02008572:
  if ((v_r3 - v_r2) >= 0)
    goto L_0200857e;
  v_r1 = -1;
  goto L_02008580;
L_0200857e:
  v_r1 = 1;
L_02008580:
  t2 = *(u16 *)v_r5;
  *(u16 *)v_r5 = (t2 + v_r1);
  t3 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t4 = *(u16 *)(s32)gIoWriteQueue;
  v_r0 = (s32)gIoWriteQueue;
  v_r1 = (t2 + v_r1);
  v_r3 = t4;
  v_r4 = 0x4000208;
  v_r6 = t3;
  if (v_r3 > 31)
    goto L_020085c0;
  *(u16 *)v_r0 = (v_r3 + 1);
  *(s32 *)(((((v_r3 << 1) + v_r3) << 2) + v_r0) + 4) =
      (((16 - ((v_r1 << 16) >> 16)) << 8) | ((v_r1 << 16) >> 16));
  *(s32 *)(((((v_r3 << 1) + v_r3) << 2) + v_r0) + 8) = 0x4000052;
  FIELD_AT(((((v_r3 << 1) + v_r3) << 2) + v_r0), s32, 12) = 0x20000;
L_020085c0:
  *(u16 *)v_r4 = v_r6;
  t5 = *(s16 *)v_r5;
  if (t5 != 0)
    goto L_02008638;
  t6 = *(s16 *)(s32)gData_02003a80;
  v_r5 = (s32)gData_02003a80;
  if (t6 != 0)
    goto L_020085e8;
  Call6((void (*)())Func_02003170, 34, 8, 3, 3, 17, 8);
L_020085e8:
  t8 = *(s16 *)v_r5;
  if (t8 != 1)
    goto L_02008602;
  Call6((void (*)())Func_02003170, 30, 8, 3, 3, 8, 8);
L_02008602:
  t10 = *(s16 *)v_r5;
  if (t10 != 2)
    goto L_0200861e;
  Call6((void (*)())Func_02003170, 34, 8, 3, 3, 17, 8);
L_0200861e:
  t12 = *(s16 *)v_r5;
  if (t12 != 3)
    goto L_02008638;
  Call6((void (*)())Func_02003170, 30, 8, 3, 3, 8, 8);
L_02008638:
  t14 = *(s16 *)(s32)Data_02003a7c;
  if (t14 != 16)
    goto L_020086b0;
  t15 = *(s16 *)(s32)gData_02003a80;
  v_r5 = (s32)gData_02003a80;
  if (t15 != 0)
    goto L_02008660;
  Call6((void (*)())Func_02003170, 34, 0, 3, 3, 17, 8);
L_02008660:
  t17 = *(s16 *)v_r5;
  if (t17 != 1)
    goto L_0200867a;
  Call6((void (*)())Func_02003170, 30, 4, 3, 3, 8, 8);
L_0200867a:
  t19 = *(s16 *)v_r5;
  if (t19 != 2)
    goto L_02008696;
  Call6((void (*)())Func_02003170, 34, 4, 3, 3, 17, 8);
L_02008696:
  t21 = *(s16 *)v_r5;
  if (t21 != 3)
    goto L_020086b0;
  Call6((void (*)())Func_02003170, 30, 0, 3, 3, 8, 8);
L_020086b0:
  return;
}
