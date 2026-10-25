// Draft of overlay 6aa's Func_020020ec, translated from its listing: differ
// 203 of 182. Left: its shape: 183 instructions against the listing's 182.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02001c74();
s32 Func_02001e60();
s32 Func_02001ea8();
s32 Func_02001ef0();
s32 Func_0200206c();
s32 GameFlag_Test();
extern u8 gData_020023c4[];

void Func_020020ec(void) {
  s32 sp_[1];
  s32 v_r8, v_r7, v_r9, v_r10, v_r0, v_r3, v_r11, v_r5, v_r6, v_r4;
  s32 v_r2, v_r1, t0, t1, t2, t3, t4, t5, t6, t7;
  s32 t8, t10, t11, t12, t13, t14, t15, t16, t17, t18;
  s32 t19, t20, t21, t22, t23, t24, t25, t26, t28, t29;
  s32 t31, t32, t34, t35, t37, t38;

  t0 = FIELD_AT(0x3000000, s32, 32);
  v_r8 = ((s32)gData_020023c4 + 0x188);
  t1 = FIELD_AT(t0, s32, 228);
  t2 = FIELD_AT(t0, s32, 232);
  t3 = *(s32 *)t0;
  t4 = FIELD_AT(t3, s32, 4);
  FIELD_AT(sp_, s32, 4) = t4;
  t5 = FIELD_AT(0x3000000, s32, 108);
  *(s32 *)sp_ = t5;
  t6 = FIELD_AT(v_r8, s16, 6);
  v_r0 = v_r8;
  v_r10 = (t2 & 0xffff0000);
  v_r9 = (t1 & 0xffff0000);
  v_r7 = (v_r8 + 20);
  if (t6 == 0)
    goto L_0200a146;
  t7 = FIELD_AT(v_r0, s16, 8);
  if (t7 != 0)
    goto L_0200a146;
  t8 = FIELD_AT(v_r0, s32, 16);
  v_r3 = t8;
  if (v_r3 == 0)
    goto L_0200a146;
  ((s32 (*)())v_r3)();
L_0200a146:
  t10 = FIELD_AT(v_r8, u16, 6);
  FIELD_AT(v_r8, u16, 8) = t10;
  FIELD_AT(v_r8, u16, 6) = 0;
  v_r11 = 3;
L_0200a158:
  t11 = *(s16 *)v_r7;
  if (t11 == 0)
    goto L_0200a248;
  t12 = FIELD_AT(v_r7, s32, 8);
  v_r5 = t12;
  if (v_r5 == 0)
    goto L_0200a248;
  t13 = Value1((s32 (*)())GameFlag_Test, 0x106);
  t14 = *(s32 *)sp_;
  t15 = FIELD_AT(t14, s16, 358);
  v_r0 = t13;
  if (t15 == 0)
    goto L_0200a182;
  v_r0 = (v_r0 | 1);
L_0200a182:
  v_r6 = (v_r5 + 91);
  *(u8 *)v_r6 = v_r0;
  t16 = FIELD_AT(v_r8, s16, 14);
  if (t16 == 0)
    goto L_0200a19c;
  t17 = Value1((s32 (*)())GameFlag_Test, 0x106);
  *(u8 *)v_r6 = t17;
L_0200a19c:
  t18 = FIELD_AT(v_r5, s32, 8);
  t19 = FIELD_AT(v_r5, s32, 16);
  t20 = FIELD_AT(v_r5, s32, 12);
  t21 = FIELD_AT(v_r7, s16, 6);
  v_r1 = ((((t19 - v_r10) - t20) >> 16) - 8);
  v_r2 = ((t18 - v_r9) >> 16);
  v_r4 = t21;
  if (v_r4 != 0)
    goto L_0200a1d2;
  if ((u32)(v_r2 + 7) > (u32)0x14e)
    goto L_0200a248;
  if (v_r1 <= -0x30)
    goto L_0200a248;
  if (v_r1 > 239)
    goto L_0200a248;
L_0200a1d2:
  t22 = FIELD_AT(v_r7, s16, 2);
  t23 = FIELD_AT(v_r7, u16, 2);
  v_r1 = t23;
  if (t22 > 0)
    goto L_0200a244;
  t24 = FIELD_AT(v_r7, u16, 4);
  v_r1 = (240 & t24);
  if (v_r1 == 32)
    goto L_0200a218;
  if (v_r1 > 32)
    goto L_0200a1f4;
  if (v_r1 == 0)
    goto L_0200a234;
  if (v_r1 == 16)
    goto L_0200a226;
  goto L_0200a240;
L_0200a1f4:
  if (v_r1 == 48)
    goto L_0200a20a;
  if (v_r1 != 128)
    goto L_0200a240;
  t25 = FIELD_AT(v_r7, s32, 8);
  t26 = FIELD_AT(v_r7, s32, 12);
  Call3((void (*)())Func_0200206c, (t25 + 8), t26, v_r4);
  goto L_0200a240;
L_0200a20a:
  t28 = FIELD_AT(v_r7, s32, 8);
  t29 = FIELD_AT(v_r7, s32, 12);
  Call3((void (*)())Func_02001e60, (t28 + 8), t29, v_r4);
  goto L_0200a240;
L_0200a218:
  t31 = FIELD_AT(v_r7, s32, 8);
  t32 = FIELD_AT(v_r7, s32, 12);
  Call3((void (*)())Func_02001ef0, (t31 + 8), t32, v_r4);
  goto L_0200a240;
L_0200a226:
  t34 = FIELD_AT(v_r7, s32, 8);
  t35 = FIELD_AT(v_r7, s32, 12);
  Call3((void (*)())Func_02001ea8, (t34 + 8), t35, v_r4);
  goto L_0200a240;
L_0200a234:
  t37 = FIELD_AT(v_r7, s32, 8);
  t38 = FIELD_AT(v_r7, s32, 12);
  Call3((void (*)())Func_02001c74, (t37 + 8), t38, v_r4);
L_0200a240:
  v_r3 = 8;
  goto L_0200a246;
L_0200a244:
  v_r3 = (v_r1 - 1);
L_0200a246:
  FIELD_AT(v_r7, u16, 2) = v_r3;
L_0200a248:
  v_r11 = (v_r11 - 1);
  v_r7 = (v_r7 + 16);
  if (v_r11 < 0)
    goto L_0200a258;
  goto L_0200a158;
L_0200a258:
  return;
}
