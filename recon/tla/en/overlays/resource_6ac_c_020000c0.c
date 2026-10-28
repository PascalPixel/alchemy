// Draft of overlay 6ac's Func_020000c0, translated from its listing: differ
// 270 of 230. Left: its shape: 188 instructions against the listing's 230.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 GameFlag_Test();
s32 __modsi3();
extern u8 Data_020039bc[];
extern u8 IwramCopyWords[];
extern u8 gData_020036c4[];
extern u8 gData_0200393c[];
extern u8 gData_0200397c[];
extern u8 gIoWriteQueue[];
extern u8 gSequenceParentWork[];

void Func_020000c0(void) {
  s32 v_r2, v_r11, v_r10, v_r7, v_r0, v_r9, v_r8, v_r5, v_r6, v_r4;
  s32 v_r1, t0, t1, t2, t3, t4, t8, t9, t11, t12;
  s32 t13, t14, t15, t16, t17, t18, t19, t20, t21, t22;
  s32 t23, t24, t25;

  t0 = FIELD_AT((s32)gSequenceParentWork, s16, 482);
  t1 = FIELD_AT(0x3000000, s32, 108);
  t2 = FIELD_AT(t1, s16, 358);
  v_r11 = t0;
  v_r2 = t1;
  if (t2 == 0)
    goto L_020080f2;
  goto L_02008292;
L_020080f2:
  t3 = FIELD_AT(v_r2, u8, 3236);
  v_r10 = ((t3 << 24) >> 24);
  if (v_r10 == 0)
    goto L_02008108;
  goto L_02008292;
L_02008108:
  t4 = *(s32 *)(s32)gData_020036c4;
  v_r7 = (s32)gData_020036c4;
  if (t4 != -1)
    goto L_02008140;
  ((s32 (*)())(s32)IwramCopyWords)((s32)Data_020039bc, 0x50001a0, 32);
  ((s32 (*)())(s32)IwramCopyWords)((s32)gData_0200393c, 0x5000100, 64);
  ((s32 (*)())(s32)IwramCopyWords)((s32)gData_0200397c, (s32)gData_0200393c,
                                   64);
  *(s32 *)v_r7 = v_r10;
L_02008140:
  t8 = *(s32 *)v_r7;
  v_r0 = t8;
  if (v_r0 >= 0)
    goto L_02008148;
  v_r0 = (v_r0 + 7);
L_02008148:
  t9 = __modsi3((v_r0 >> 3), 6);
  ((s32 (*)())(s32)IwramCopyWords)((s32)gData_0200397c, (s32)Data_020039bc, 32);
  v_r9 = t9;
  if (v_r11 != 1)
    goto L_0200819e;
  t11 = GameFlag_Test(0xa32);
  if (t11 == 0)
    goto L_0200819e;
  v_r8 = 20;
  v_r7 = 0;
L_02008178:
  t12 = __modsi3((v_r9 + v_r7), 6);
  t13 = *(u16 *)((s32)gData_0200393c + ((t12 << 1) + 20));
  *(u16 *)((s32)gData_0200397c + v_r8) = t13;
  v_r7 = (v_r7 + 1);
  v_r8 = (v_r8 + 2);
  if (v_r7 <= 5)
    goto L_02008178;
L_0200819e:
  if (v_r11 != 2)
    goto L_020081de;
  t14 = GameFlag_Test(0xa33);
  if (t14 == 0)
    goto L_020081de;
  v_r8 = 20;
  v_r7 = 0;
L_020081b8:
  t15 = __modsi3((v_r9 + v_r7), 6);
  t16 = *(u16 *)((s32)gData_0200393c + ((t15 << 1) + 40));
  *(u16 *)((s32)gData_0200397c + v_r8) = t16;
  v_r7 = (v_r7 + 1);
  v_r8 = (v_r8 + 2);
  if (v_r7 <= 5)
    goto L_020081b8;
L_020081de:
  if (v_r11 != 3)
    goto L_0200821c;
  t17 = GameFlag_Test(0xa34);
  if (t17 == 0)
    goto L_0200821c;
  v_r5 = 20;
  v_r7 = 0;
L_020081f6:
  t18 = __modsi3((v_r9 + v_r7), 6);
  t19 = *(u16 *)((s32)gData_0200393c + ((t18 << 1) + 52));
  *(u16 *)((s32)gData_0200397c + v_r5) = t19;
  v_r7 = (v_r7 + 1);
  v_r5 = (v_r5 + 2);
  if (v_r7 <= 5)
    goto L_020081f6;
L_0200821c:
  if (v_r11 != 4)
    goto L_0200825a;
  t20 = GameFlag_Test(0xa35);
  if (t20 == 0)
    goto L_0200825a;
  v_r6 = 20;
  v_r7 = 0;
L_02008234:
  t21 = __modsi3((v_r9 + v_r7), 6);
  t22 = *(u16 *)((s32)gData_0200393c + ((t21 << 1) + 8));
  *(u16 *)((s32)gData_0200397c + v_r6) = t22;
  v_r7 = (v_r7 + 1);
  v_r6 = (v_r6 + 2);
  if (v_r7 <= 5)
    goto L_02008234;
L_0200825a:
  t23 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t24 = *(u16 *)(s32)gIoWriteQueue;
  v_r1 = (s32)gIoWriteQueue;
  v_r2 = t24;
  v_r0 = 0x4000208;
  v_r4 = t23;
  if (v_r2 > 31)
    goto L_02008288;
  *(u16 *)v_r1 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r1) + 4) = (s32)gData_0200397c;
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r1) + 8) = 0x50001a0;
  FIELD_AT(((((v_r2 << 1) + v_r2) << 2) + v_r1), s32, 12) = 0x84000008;
L_02008288:
  *(u16 *)v_r0 = v_r4;
  t25 = *(s32 *)(s32)gData_020036c4;
  *(s32 *)(s32)gData_020036c4 = (t25 + 1);
L_02008292:
  return;
}
