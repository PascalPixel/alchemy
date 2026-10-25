// Draft of overlay 6ac's Func_020003c4, translated from its listing: differ
// 190 of 179. Left: its shape: 163 instructions against the listing's 179.
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
  s32 t2, t3, t7, t8, t9, t10, t11, t12, t13, t14;
  s32 t15, t17, t19, t21, t23, t24, t25;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = FIELD_AT(t0, s16, 358);
  v_r2 = t0;
  if (t1 == 0)
    goto L_020083e2;
  goto L_0200852e;
L_020083e2:
  t2 = FIELD_AT(v_r2, s8, 3236);
  v_r5 = t2;
  if (v_r5 == 0)
    goto L_020083f4;
  goto L_0200852e;
L_020083f4:
  t3 = *(s32 *)(s32)gData_02003704;
  v_r7 = (s32)gData_02003704;
  if (t3 != -1)
    goto L_0200842c;
  ((s32 (*)())(s32)IwramCopyWords)((s32)gData_02003a5c, 0x5000040, 32);
  ((s32 (*)())(s32)IwramCopyWords)((s32)Data_020039dc, 0x5000100, 64);
  ((s32 (*)())(s32)IwramCopyWords)((s32)Data_02003a1c, (s32)Data_020039dc, 64);
  *(s32 *)v_r7 = v_r5;
L_0200842c:
  t7 = *(s32 *)v_r7;
  v_r0 = t7;
  if (v_r0 >= 0)
    goto L_02008434;
  v_r0 = (v_r0 + 7);
L_02008434:
  t8 = __modsi3((v_r0 >> 3), 6);
  v_r7 = 8;
  v_r4 = 0;
  v_r8 = t8;
L_02008442:
  *(s32 *)sp_ = v_r4;
  t9 = __modsi3((v_r8 + v_r4), 6);
  t10 = *(u16 *)((s32)Data_020039dc + ((t9 << 1) + 8));
  *(u16 *)((s32)Data_02003a1c + v_r7) = t10;
  t11 = *(u16 *)((s32)Data_020039dc + ((t9 << 1) + 20));
  *(u16 *)((s32)Data_02003a1c + (v_r7 + 12)) = t11;
  t12 = *(u16 *)((s32)Data_020039dc + ((t9 << 1) + 40));
  *(u16 *)((s32)Data_02003a1c + (v_r7 + 32)) = t12;
  t13 = *(s32 *)sp_;
  t14 = *(u16 *)((s32)Data_020039dc + ((t9 << 1) + 52));
  *(u16 *)((s32)Data_02003a1c + (v_r7 + 44)) = t14;
  v_r6 = (s32)Data_02003a1c;
  v_r7 = (v_r7 + 2);
  v_r4 = (t13 + 1);
  if (v_r4 <= 5)
    goto L_02008442;
  t15 = GameFlag_Test(0xa32);
  if (t15 != 0)
    goto L_020084a4;
  ((s32 (*)())(s32)IwramCopyWords)((v_r6 + 20), (s32)Data_0200ba70, 12);
L_020084a4:
  t17 = GameFlag_Test(0xa33);
  if (t17 != 0)
    goto L_020084c0;
  ((s32 (*)())(s32)IwramCopyWords)((v_r6 + 40), (s32)Data_0200ba70, 12);
L_020084c0:
  t19 = GameFlag_Test(0xa34);
  if (t19 != 0)
    goto L_020084dc;
  ((s32 (*)())(s32)IwramCopyWords)((v_r6 + 52), (s32)Data_0200ba70, 12);
L_020084dc:
  t21 = GameFlag_Test(0xa35);
  if (t21 != 0)
    goto L_020084f8;
  ((s32 (*)())(s32)IwramCopyWords)((v_r6 + 8), (s32)Data_0200ba70, 12);
L_020084f8:
  t23 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t24 = *(u16 *)(s32)gIoWriteQueue;
  v_r1 = (s32)gIoWriteQueue;
  v_r2 = t24;
  v_r0 = 0x4000208;
  v_r4 = t23;
  if (v_r2 > 31)
    goto L_02008524;
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r1) + 4) = v_r6;
  *(u16 *)v_r1 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r1) + 8) = 0x5000100;
  FIELD_AT(((((v_r2 << 1) + v_r2) << 2) + v_r1), s32, 12) = 0x84000010;
L_02008524:
  *(u16 *)v_r0 = v_r4;
  t25 = *(s32 *)(s32)gData_02003704;
  *(s32 *)(s32)gData_02003704 = (t25 + 1);
L_0200852e:
  return;
}
