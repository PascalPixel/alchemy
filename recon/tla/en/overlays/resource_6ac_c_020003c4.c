// Draft of overlay 6ac's Func_020003c4, translated from its listing: differ
// 206 of 179. Left: its shape: 119 instructions against the listing's 179.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 GameFlag_Test();
s32 __modsi3();
extern u8 Data_020039dc[];
extern u8 Data_02003a1c[];
extern u8 Data_0200ba70[];
extern u8 IwramCopyWords[];
extern u8 gData_02003704[];
extern u8 gData_02003a5c[];
extern u8 gIoWriteQueue[];

void Func_020003c4(void) {
  s32 sp_[1];
  s32 v_r2, v_r5, v_r7, v_r0, v_r8, v_r4, v_r6, v_r1, t0, t1;
  s32 t2, t3, t4, t5, t6, t7, t8, t9, t10, t11;
  s32 t12, t13, t14, t15, t16, t17, t18;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = *(s16 *)(t0 + 0x166);
  v_r2 = t0;
  if (t1 == 0)
    goto L_020083e2;
  goto L_0200852e;
L_020083e2:
  t2 = *(s8 *)(v_r2 + 0xca4);
  v_r5 = t2;
  if (v_r5 == 0)
    goto L_020083f4;
  goto L_0200852e;
L_020083f4:
  t3 = *(s32 *)(s32)gData_02003704;
  v_r7 = (s32)gData_02003704;
  if (t3 != -1)
    goto L_0200842c;
  *(s32 *)v_r7 = v_r5;
L_0200842c:
  t4 = *(s32 *)v_r7;
  v_r0 = t4;
  if (v_r0 >= 0)
    goto L_02008434;
  v_r0 = (v_r0 + 7);
L_02008434:
  t5 = __modsi3((v_r0 >> 3), 6);
  v_r7 = 8;
  v_r4 = 0;
  v_r8 = t5;
L_02008442:
  FIELD_AT(sp_, s32, 0) = v_r4;
  t6 = __modsi3((v_r8 + v_r4), 6);
  t7 = *(u16 *)((s32)Data_020039dc + ((t6 << 1) + 8));
  *(u16 *)((s32)Data_02003a1c + v_r7) = t7;
  t8 = *(u16 *)((s32)Data_020039dc + ((t6 << 1) + 20));
  *(u16 *)((s32)Data_02003a1c + (v_r7 + 12)) = t8;
  t9 = *(u16 *)((s32)Data_020039dc + ((t6 << 1) + 40));
  *(u16 *)((s32)Data_02003a1c + ((v_r7 + 12) + 20)) = t9;
  t10 = FIELD_AT(sp_, s32, 0);
  t11 = *(u16 *)((s32)Data_020039dc + ((t6 << 1) + 52));
  *(u16 *)((s32)Data_02003a1c + (((v_r7 + 12) + 20) + 12)) = t11;
  v_r6 = (s32)Data_02003a1c;
  v_r7 = (v_r7 + 2);
  v_r4 = (t10 + 1);
  if (v_r4 <= 5)
    goto L_02008442;
  t12 = GameFlag_Test(0xa32);
  if (t12 != 0)
    goto L_020084a4;
L_020084a4:
  t13 = GameFlag_Test(0xa33);
  if (t13 != 0)
    goto L_020084c0;
L_020084c0:
  t14 = GameFlag_Test(0xa34);
  if (t14 != 0)
    goto L_020084dc;
L_020084dc:
  t15 = GameFlag_Test(0xa35);
  if (t15 != 0)
    goto L_020084f8;
L_020084f8:
  t16 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t17 = *(u16 *)(s32)gIoWriteQueue;
  v_r1 = (s32)gIoWriteQueue;
  v_r2 = t17;
  v_r0 = 0x4000208;
  v_r4 = t16;
  if (v_r2 > 31)
    goto L_02008524;
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r1) + 4) = v_r6;
  *(u16 *)v_r1 = (v_r2 + 1);
  *(s32 *)((((((v_r2 << 1) + v_r2) << 2) + v_r1) + 4) + 4) = 0x5000100;
  *(s32 *)(((((((v_r2 << 1) + v_r2) << 2) + v_r1) + 4) + 4) + 4) = 0x84000010;
L_02008524:
  *(u16 *)v_r0 = v_r4;
  t18 = *(s32 *)(s32)gData_02003704;
  *(s32 *)(s32)gData_02003704 = (t18 + 1);
L_0200852e:
  return;
}
