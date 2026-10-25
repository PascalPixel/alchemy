// Draft of overlay 6b5's Func_0200039c, translated from its listing: differ
// 300 of 185. Left: its shape: 237 instructions against the listing's 185.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Djinn_GetRecoveryTable();
s32 Func_0200193c();
s32 MemFree();
s32 Runtime_BumpAllocateAlternatePool();
s32 SerialRuntime_StartReceive();
s32 Ui_AdjustValueWithoutLimit();
s32 Unit_Get();
s32 WaitFrames();
extern u8 gLinkStatus[];
extern u8 gSerialReceivedSize[];

s32 Func_0200039c(s32 p0, s32 p1, s32 p2, s32 p3) {
  s32 sp_[1];
  s32 v_r8, v_r9, v_r3, v_r10, v_r7, v_r11, v_r5, v_r6, v_r1, v_r2;
  s32 v_r4, v_r0, t0, t1, t3, t4, t5, t6, t9, t10;
  s32 t11, t12, t13, t14, t15, t16, t18, t19, t20, t23;
  s32 t24, t25;

  t0 = Value4((s32 (*)())Runtime_BumpAllocateAlternatePool, 0x154, p1, 0x154,
              p3);
  v_r11 = 0x154;
  v_r7 = 0;
  v_r10 = 0x384;
  v_r3 = 0x384;
  v_r9 = t0;
  v_r8 = 0;
  goto L_0200847a;
L_020083c8:
  t1 = *(u16 *)v_r3;
  v_r3 = t1;
  if ((u32)v_r3 <= (u32)v_r11)
    goto L_020083d0;
  goto L_020084de;
L_020083d0:
  Call1((void (*)())WaitFrames, 1);
  v_r10 = (v_r10 - 1);
  v_r3 = -1;
  v_r2 = v_r10;
  if (v_r2 < 0)
    goto L_020083ee;
  t3 = *(u16 *)(s32)gLinkStatus;
  v_r3 = (3 & t3);
  v_r2 = t3;
  if (v_r3 == 3)
    goto L_020083f4;
L_020083ee:
  v_r5 = (v_r5 + 1);
  if (v_r5 > 24)
    goto L_020084de;
L_020083f4:
  t4 = Value0((s32 (*)())Func_0200193c);
  v_r3 = (s32)gSerialReceivedSize;
  if (t4 != 0)
    goto L_020083c8;
  t5 = *(u16 *)v_r3;
  v_r3 = t5;
  if (v_r3 != v_r11)
    goto L_020084de;
  t6 = FIELD_AT(v_r6, u8, 298);
  if (t6 == 0)
    goto L_02008414;
  v_r7 = (v_r7 + 1);
L_02008414:
  Call1((void (*)())WaitFrames, 2);
  Call2((void (*)())Ui_AdjustValueWithoutLimit, 0xc58, (s32)sp_);
  t9 = *(u16 *)sp_;
  v_r2 = (s32)sp_;
  v_r1 = 0;
  if (t9 == v_r1)
    goto L_0200843a;
L_0200842c:
  v_r1 = (v_r1 + 1);
  if (v_r1 > 4)
    goto L_0200843a;
  t10 = *(u16 *)(v_r2 + (v_r1 << 1));
  if (t10 != 0)
    goto L_0200842c;
L_0200843a:
  t11 = v_r1;
  v_r1 = 14;
  v_r4 = t11;
  if (v_r1 < v_r4)
    goto L_0200845a;
  v_r0 = (v_r6 + 14);
  v_r2 = ((v_r6 - v_r4) + 14);
L_0200844c:
  t12 = *(u8 *)v_r2;
  *(u8 *)v_r0 = t12;
  v_r0 = (v_r0 - 1);
  v_r1 = (v_r1 - 1);
  v_r2 = (v_r2 - 1);
  if (v_r1 >= v_r4)
    goto L_0200844c;
L_0200845a:
  v_r1 = 0;
  if (v_r1 >= v_r4)
    goto L_02008472;
  v_r0 = v_r6;
L_02008462:
  t13 = *(u16 *)((s32)sp_ + (v_r1 << 1));
  *(u8 *)v_r0 = t13;
  v_r0 = (v_r0 + 1);
  v_r1 = (v_r1 + 1);
  if (v_r1 < v_r4)
    goto L_02008462;
L_02008472:
  FIELD_AT(v_r6, u8, 14) = 0;
  v_r3 = 1;
  v_r8 = (v_r8 + 1);
L_0200847a:
  v_r2 = v_r8;
  if (v_r2 > 2)
    goto L_020084ec;
  t14 = Value4((s32 (*)())Unit_Get, (v_r8 + 128), v_r1, v_r2, v_r3);
  t15 = Value1((s32 (*)())SerialRuntime_StartReceive, t14);
  v_r3 = -1;
  v_r0 = t15;
  v_r6 = t14;
  v_r5 = 0;
  if (v_r0 != v_r3)
    goto L_020083f4;
  goto L_02008514;
L_0200849a:
  t16 = *(u16 *)v_r3;
  v_r3 = t16;
  v_r2 = 0x154;
  if (v_r3 > v_r2)
    goto L_020084de;
  Call1((void (*)())WaitFrames, 1);
  v_r10 = (v_r10 - 1);
  v_r3 = -1;
  v_r2 = v_r10;
  if (v_r2 < 0)
    goto L_020084c2;
  t18 = *(u16 *)(s32)gLinkStatus;
  v_r3 = (3 & t18);
  v_r2 = t18;
  if (v_r3 == 3)
    goto L_020084c8;
L_020084c2:
  v_r5 = (v_r5 + 1);
  if (v_r5 > 24)
    goto L_020084de;
L_020084c8:
  t19 = Value0((s32 (*)())Func_0200193c);
  v_r3 = (s32)gSerialReceivedSize;
  if (t19 != 0)
    goto L_0200849a;
  t20 = *(u16 *)v_r3;
  v_r3 = 0x154;
  if (t20 == v_r3)
    goto L_020084e4;
L_020084de:
  v_r7 = -1;
  goto L_02008516;
L_020084e4:
  Call1((void (*)())WaitFrames, 2);
  goto L_02008516;
L_020084ec:
  Call4((void (*)())MemFree, v_r9, v_r1, v_r2, v_r3);
  t23 = Runtime_BumpAllocateAlternatePool(0x154, 0, 0x154, 0);
  t24 = Value1((s32 (*)())Djinn_GetRecoveryTable, 1);
  t25 = Value1((s32 (*)())SerialRuntime_StartReceive, t24);
  v_r3 = -1;
  v_r0 = t25;
  v_r5 = 0;
  v_r9 = t23;
  if (v_r0 != v_r3)
    goto L_020084c8;
L_02008514:
  v_r7 = v_r0;
L_02008516:
  Call4((void (*)())MemFree, v_r9, v_r1, v_r2, v_r3);
  return v_r7;
}
