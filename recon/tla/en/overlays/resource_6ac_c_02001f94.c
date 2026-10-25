// Draft of overlay 6ac's Func_02001f94, translated from its listing: differ
// 200 of 182. Left: its shape: 170 instructions against the listing's 182.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02001b1c();
s32 Func_02001d08();
s32 Func_02001d50();
s32 Func_02001d98();
s32 Func_02001f14();
s32 GameFlag_Test();
extern u8 gData_020023c4[];

void Func_02001f94(void) {
  s32 sp_[1];
  s32 v_r8, v_r7, v_r9, v_r10, v_r0, v_r3, v_r11, v_r5, v_r6, v_r4;
  s32 v_r2, v_r1, t0, t1, t2, t3, t4, t5, t6, t7;
  s32 t8, t9, t10, t11, t12, t13, t14, t15, t16, t17;
  s32 t18, t19, t20, t21, t22, t23, t24, t25, t27, t28;
  s32 t30, t31, t33, t34, t36, t37;

  t0 = FIELD_AT(0x3000000, s32, 32);
  v_r8 = ((s32)gData_020023c4 + 0x188);
  t1 = *(s32 *)(t0 + 228);
  t2 = FIELD_AT((t0 + 228), s32, 4);
  t3 = *(s32 *)t0;
  t4 = FIELD_AT(t3, s32, 4);
  FIELD_AT(sp_, s32, 4) = t4;
  t5 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(sp_, s32, 0) = t5;
  t6 = FIELD_AT(v_r8, s16, 6);
  v_r0 = v_r8;
  v_r10 = (t2 & 0xffff0000);
  v_r9 = (t1 & 0xffff0000);
  v_r7 = (v_r8 + 20);
  if (t6 == 0)
    goto L_02009fee;
  t7 = FIELD_AT(v_r0, s16, 8);
  if (t7 != 0)
    goto L_02009fee;
  t8 = FIELD_AT(v_r0, s32, 16);
  v_r3 = t8;
  if (v_r3 == 0)
    goto L_02009fee;
L_02009fee:
  t9 = FIELD_AT(v_r8, u16, 6);
  FIELD_AT(v_r8, u16, 8) = t9;
  FIELD_AT(v_r8, u16, 6) = 0;
  v_r11 = 3;
L_0200a000:
  t10 = *(s16 *)v_r7;
  if (t10 == 0)
    goto L_0200a0f0;
  t11 = FIELD_AT(v_r7, s32, 8);
  v_r5 = t11;
  if (v_r5 == 0)
    goto L_0200a0f0;
  t12 = Value1((s32 (*)())GameFlag_Test, 0x106);
  t13 = FIELD_AT(sp_, s32, 0);
  t14 = *(s16 *)(t13 + 0x166);
  v_r0 = t12;
  if (t14 == 0)
    goto L_0200a02a;
  v_r0 = (v_r0 | 1);
L_0200a02a:
  v_r6 = (v_r5 + 91);
  *(u8 *)v_r6 = v_r0;
  t15 = FIELD_AT(v_r8, s16, 14);
  if (t15 == 0)
    goto L_0200a044;
  t16 = Value1((s32 (*)())GameFlag_Test, 0x106);
  *(u8 *)v_r6 = t16;
L_0200a044:
  t17 = FIELD_AT(v_r5, s32, 8);
  t18 = FIELD_AT(v_r5, s32, 16);
  t19 = FIELD_AT(v_r5, s32, 12);
  t20 = FIELD_AT(v_r7, s16, 6);
  v_r1 = ((((t18 - v_r10) - t19) >> 16) - 8);
  v_r2 = ((t17 - v_r9) >> 16);
  v_r4 = t20;
  if (v_r4 != 0)
    goto L_0200a07a;
  if ((u32)(v_r2 + 7) > (u32)0x14e)
    goto L_0200a0f0;
  if (v_r1 <= -0x30)
    goto L_0200a0f0;
  if (v_r1 > 239)
    goto L_0200a0f0;
L_0200a07a:
  t21 = FIELD_AT(v_r7, s16, 2);
  t22 = FIELD_AT(v_r7, u16, 2);
  v_r1 = t22;
  if (t21 > 0)
    goto L_0200a0ec;
  t23 = FIELD_AT(v_r7, u16, 4);
  v_r1 = (240 & t23);
  if (v_r1 == 32)
    goto L_0200a0c0;
  if (v_r1 > 32)
    goto L_0200a09c;
  if (v_r1 == 0)
    goto L_0200a0dc;
  if (v_r1 == 16)
    goto L_0200a0ce;
  goto L_0200a0e8;
L_0200a09c:
  if (v_r1 == 48)
    goto L_0200a0b2;
  if (v_r1 != 128)
    goto L_0200a0e8;
  t24 = FIELD_AT(v_r7, s32, 8);
  t25 = FIELD_AT(v_r7, s32, 12);
  Call3((void (*)())Func_02001f14, (t24 + 8), t25, v_r4);
  goto L_0200a0e8;
L_0200a0b2:
  t27 = FIELD_AT(v_r7, s32, 8);
  t28 = FIELD_AT(v_r7, s32, 12);
  Call3((void (*)())Func_02001d08, (t27 + 8), t28, v_r4);
  goto L_0200a0e8;
L_0200a0c0:
  t30 = FIELD_AT(v_r7, s32, 8);
  t31 = FIELD_AT(v_r7, s32, 12);
  Call3((void (*)())Func_02001d98, (t30 + 8), t31, v_r4);
  goto L_0200a0e8;
L_0200a0ce:
  t33 = FIELD_AT(v_r7, s32, 8);
  t34 = FIELD_AT(v_r7, s32, 12);
  Call3((void (*)())Func_02001d50, (t33 + 8), t34, v_r4);
  goto L_0200a0e8;
L_0200a0dc:
  t36 = FIELD_AT(v_r7, s32, 8);
  t37 = FIELD_AT(v_r7, s32, 12);
  Call3((void (*)())Func_02001b1c, (t36 + 8), t37, v_r4);
L_0200a0e8:
  v_r3 = 8;
  goto L_0200a0ee;
L_0200a0ec:
  v_r3 = (v_r1 - 1);
L_0200a0ee:
  FIELD_AT(v_r7, u16, 2) = v_r3;
L_0200a0f0:
  v_r11 = (v_r11 + -1);
  v_r7 = (v_r7 + 16);
  if (v_r11 < 0)
    goto L_0200a100;
  goto L_0200a000;
L_0200a100:
  return;
}
