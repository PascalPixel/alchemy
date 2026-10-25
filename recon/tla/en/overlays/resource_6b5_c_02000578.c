// Draft of overlay 6b5's Func_02000578, translated from its listing: differ
// 382 of 264. Left: its shape: 294 instructions against the listing's 264.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Djinn_GetRecoveryTable();
s32 Func_02000538();
s32 Func_0200192c();
s32 Func_0200193c();
s32 MemFree();
s32 Runtime_BumpAllocateAlternatePool();
s32 Unit_Get();
s32 WaitFrames();
extern u8 IwramCopyWords[];
extern u8 gLinkStatus[];

s32 Func_02000578(void) {
  s32 sp_[1];
  s32 v_r6, v_r7, v_r11, v_r8, v_r9, v_r1, v_r3, v_r2, v_r5, v_r10;
  s32 v_r0, v_r4, t0, t1, t2, t3, t5, t6, t7, t9;
  s32 t10, t12, t14, t15, t17, t19, t21, t22, t23, t24;
  s32 t25, t26, t27, t28, t29, t31, t32;

  t0 = Runtime_BumpAllocateAlternatePool(0x154);
  t1 = Func_02000538(((s32)sp_ + 16));
  FIELD_AT(sp_, s32, 4) = t1;
  v_r9 = 0x258;
  v_r8 = ((s32)sp_ + 16);
  v_r11 = 0;
  v_r7 = t0;
  v_r6 = 0;
L_020085ac:
  FIELD_AT(sp_, s32, 0) = ((s32)sp_ + 8);
  *(u8 *)(((s32)sp_ + 8) + v_r6) = 0;
  v_r6 = (v_r6 + 1);
  v_r3 = 0;
  v_r1 = ((s32)sp_ + 8);
  if (v_r6 <= 7)
    goto L_020085ac;
  t2 = FIELD_AT(sp_, s32, 4);
  v_r2 = t2;
  v_r6 = 0;
  if (v_r6 >= v_r2)
    goto L_02008690;
L_020085c4:
  v_r5 = (v_r6 << 1);
  t3 = *(u16 *)(v_r8 + v_r5);
  Unit_Get(t3);
  *(u8 *)(v_r7 + 0x12a) = 2;
  t5 = *(u16 *)(v_r8 + v_r5);
  t6 = FIELD_AT(sp_, s32, 0);
  *(u8 *)(t6 + t5) = (v_r6 - 128);
  t7 = Func_0200192c(v_r7, 0x154);
  v_r2 = -1;
  v_r0 = t7;
  v_r10 = 0;
  if (v_r0 != v_r2)
    goto L_0200863c;
  v_r11 = v_r0;
  goto L_0200879a;
L_0200860c:
  WaitFrames(1);
  v_r9 = (v_r9 + -1);
  if (v_r9 < 0)
    goto L_0200862a;
  t9 = *(u16 *)(s32)gLinkStatus;
  if ((3 & t9) == 3)
    goto L_0200863c;
L_0200862a:
  v_r10 = (v_r10 + 1);
  v_r3 = v_r10;
  v_r2 = 1;
  if (v_r3 <= 24)
    goto L_0200863c;
  v_r11 = -1;
  v_r1 = -1;
  goto L_0200879a;
L_0200863c:
  t10 = Func_0200193c();
  if (t10 != 0)
    goto L_0200860c;
  WaitFrames(2);
  t12 = FIELD_AT(sp_, s32, 4);
  v_r6 = (v_r6 + 1);
  v_r2 = t12;
  if (v_r6 < v_r2)
    goto L_020085c4;
  goto L_02008690;
L_02008654:
  WaitFrames(1);
  v_r9 = (v_r9 + -1);
  v_r3 = -1;
  v_r1 = v_r9;
  if (v_r1 < 0)
    goto L_02008672;
  t14 = *(u16 *)(s32)gLinkStatus;
  v_r3 = (3 & t14);
  if (v_r3 == 3)
    goto L_02008680;
L_02008672:
  v_r5 = (v_r5 + 1);
  if (v_r5 <= 24)
    goto L_02008680;
  v_r11 = -1;
  v_r2 = -1;
  goto L_0200879a;
L_02008680:
  t15 = Func_0200193c();
  if (t15 != 0)
    goto L_02008654;
  WaitFrames(2);
  v_r6 = (v_r6 + 1);
L_02008690:
  if (v_r6 > 2)
    goto L_020086b2;
  *(u8 *)(v_r7 + 0x12a) = 0;
  t17 = Func_0200192c(v_r7, 0x154);
  v_r2 = -1;
  v_r0 = t17;
  v_r5 = 0;
  if (v_r0 != v_r2)
    goto L_02008680;
  v_r11 = v_r0;
  goto L_0200879a;
L_020086b2:
  MemFree(v_r7, v_r1, v_r2, v_r3);
  t19 = Runtime_BumpAllocateAlternatePool(0x154);
  Djinn_GetRecoveryTable(0);
  t21 = *(s32 *)(t19 + 0x128);
  v_r6 = 0x120;
  v_r5 = 0;
  v_r4 = (t19 + 8);
  v_r8 = 0;
  v_r10 = 0x258;
  v_r7 = t19;
  if (v_r5 >= t21)
    goto L_02008740;
  v_r0 = v_r4;
L_020086f8:
  t22 = FIELD_AT(v_r0, u8, 2);
  t23 = *(u8 *)(((s32)sp_ + 8) + t22);
  FIELD_AT(v_r0, u8, 2) = t23;
  if ((t23 << 24) != 0)
    goto L_02008732;
  t24 = *(s32 *)(v_r4 + v_r6);
  v_r1 = v_r5;
  if (v_r1 >= (t24 - 1))
    goto L_02008722;
  v_r2 = ((v_r5 << 2) + v_r4);
L_02008714:
  t25 = FIELD_AT(v_r2, s32, 4);
  *(s32 *)v_r2 = t25;
  t26 = *(s32 *)(v_r4 + v_r6);
  v_r1 = (v_r1 + 1);
  v_r2 = (v_r2 + 4);
  if (v_r1 < (t26 - 1))
    goto L_02008714;
L_02008722:
  t27 = *(s32 *)(v_r4 + 0x120);
  *(s32 *)(v_r4 + 0x120) = (t27 - 1);
  v_r0 = (v_r0 - 4);
  v_r5 = (v_r5 - 1);
L_02008732:
  t28 = FIELD_AT(v_r4, s32, 288);
  v_r5 = (v_r5 + 1);
  v_r6 = 0x120;
  v_r0 = (v_r0 + 4);
  if (v_r5 < t28)
    goto L_020086f8;
L_02008740:
  t29 = Func_0200192c(v_r7, 0x154);
  v_r1 = -1;
  v_r0 = t29;
  if (v_r0 != v_r1)
    goto L_02008786;
  v_r11 = v_r0;
  goto L_0200879a;
L_02008756:
  WaitFrames(1);
  v_r10 = (v_r10 + -1);
  if (v_r10 < 0)
    goto L_02008774;
  t31 = *(u16 *)(s32)gLinkStatus;
  if ((3 & t31) == 3)
    goto L_02008786;
L_02008774:
  v_r8 = (v_r8 + 1);
  v_r2 = v_r8;
  v_r1 = 1;
  if (v_r2 <= 24)
    goto L_02008786;
  v_r11 = -1;
  v_r3 = -1;
  goto L_0200879a;
L_02008786:
  t32 = Func_0200193c();
  if (t32 != 0)
    goto L_02008756;
  WaitFrames(1);
  WaitFrames(2);
L_0200879a:
  MemFree(v_r7, v_r1, v_r2, v_r3);
  return v_r11;
}
