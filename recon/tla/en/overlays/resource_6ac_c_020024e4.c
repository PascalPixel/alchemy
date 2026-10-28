// Draft of overlay 6ac's Func_020024e4, translated from its listing: differ
// 248 of 236. Left: its shape: 238 instructions against the listing's 236.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 Func_02002368();
s32 Func_02003118();
s32 Func_02003168();
s32 Func_02003180();
s32 Func_020031b0();
s32 Func_020031e0();
s32 Func_02003200();
s32 Func_02003280();
s32 Actor_GetById();
s32 WaitFrames();
extern u8 Func_020024bc[];
extern u8 gData_020023c4[];
extern u8 gAdventure[];

s32 Func_020024e4(void) {
  s32 sp_[1];
  s32 v_r5, v_r6, v_r10, v_r1, v_r9, v_r3, v_r0, v_r8, v_r7, v_r11;
  s32 v_r2, v_r4, t0, t2, t3, t7, t8, t9, t10, t11;
  s32 t12, t13, t14, t15, t16, t17, t18, t19, t20, t22;
  s32 t23, t24, t25, t26, t27, t28, t30, t31, t32, t33;
  s32 t34, t35, t37, t38, t39, t40, t41, t44;

  t0 = *(u8 *)((s32)gAdventure + 0x212);
  v_r5 = (s32)gAdventure;
  if (t0 == 0)
    goto L_0200a506;
  goto L_0200a6c4;
L_0200a506:
  Call2((void (*)())Func_02002368, -10, 0);
  t2 = *(s32 *)(v_r5 + 0x214);
  t3 = Value1((s32 (*)())Actor_GetById, t2);
  FIELD_AT(sp_, s32, 0) = ((s32)gData_020023c4 + 0x188);
  Func_020031e0();
  Call1((void (*)())Func_02003280, 0);
  FIELD_AT(t3, s32, 108) = (s32)Func_020024bc;
  FIELD_AT(t3, u16, 6) = 0x4000;
  Call2((void (*)())Func_02003118, t3, 49);
  v_r6 = t3;
L_0200a53e:
  t7 = *(u8 *)(v_r6 + 34);
  t8 = FIELD_AT(v_r6, s32, 8);
  t9 = FIELD_AT(v_r6, s32, 16);
  t10 = Value3((s32 (*)())Func_02003168, t7, t8, t9);
  t11 = FIELD_AT(sp_, s32, 0);
  t12 = FIELD_AT(sp_, s32, 0);
  v_r3 = (t11 + 104);
  v_r9 = (t12 + 106);
  v_r1 = ((s32)sp_ + 4);
  v_r10 = (t11 + 104);
  if (t10 != 7)
    goto L_0200a5ba;
  t13 = *(s16 *)v_r3;
  FIELD_AT(v_r6, s32, 36) = (t13 << 17);
  t14 = *(s16 *)v_r9;
  FIELD_AT(v_r6, s32, 44) = (t14 << 17);
  FIELD_AT(v_r6, s32, 52) = 0x1000;
  v_r5 = 0;
L_0200a578:
  t15 = *(s16 *)v_r10;
  t16 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(sp_, s32, 4) = (t16 + (t15 << 19));
  t17 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(sp_, s32, 8) = t17;
  t18 = *(s16 *)v_r9;
  t19 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(sp_, s32, 12) = (t19 + (t18 << 19));
  t20 = Value3((s32 (*)())Func_02003180, v_r6, ((s32)sp_ + 4), (t18 << 19));
  if (t20 == 0)
    goto L_0200a5ac;
  FIELD_AT(v_r6, s32, 36) = 0;
  FIELD_AT(v_r6, s32, 44) = 0;
  goto L_0200a6b8;
L_0200a5ac:
  Call1((void (*)())WaitFrames, 1);
  v_r5 = (v_r5 + 1);
  if (v_r5 <= 9)
    goto L_0200a578;
  goto L_0200a6b8;
L_0200a5ba:
  t22 = *(s16 *)v_r10;
  t23 = FIELD_AT(v_r6, s32, 8);
  *(s32 *)v_r1 = (t23 + (t22 << 19));
  t24 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(v_r1, s32, 4) = t24;
  t25 = *(s16 *)v_r9;
  t26 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(v_r1, s32, 8) = (t26 + (t25 << 19));
  t27 = Value3((s32 (*)())Func_02003180, v_r6, v_r1, (t25 << 19));
  v_r0 = t27;
  if (v_r0 > 0)
    goto L_0200a6b8;
  if (v_r0 >= 0)
    goto L_0200a604;
  FIELD_AT(v_r6, u16, 6) = 0x4000;
  t28 = *(s32 *)((s32)gAdventure + 0x214);
  Call3((void (*)())Func_02003200, t28, 6, -1);
  goto L_0200a6b8;
L_0200a604:
  t30 = FIELD_AT(v_r6, u16, 32);
  t31 = FIELD_AT(0x3000000, s32, 20);
  v_r11 = (t30 - 2);
  v_r7 = (t31 + 89);
  v_r8 = 0;
  v_r5 = t31;
L_0200a618:
  t32 = *(s32 *)v_r5;
  if (t32 == 0)
    goto L_0200a648;
  t33 = *(u8 *)v_r7;
  if ((1 & t33) == 0)
    goto L_0200a648;
  if (v_r5 == v_r6)
    goto L_0200a648;
  t34 = FIELD_AT(v_r5, u16, 32);
  t35 = Value4((s32 (*)())Func_020031b0, (v_r5 + 8), v_r11, ((s32)sp_ + 4),
               (t34 - 2));
  if (t35 < 0)
    goto L_0200a648;
  Call1((void (*)())WaitFrames, 1);
  goto L_0200a6b8;
L_0200a648:
  v_r8 = (v_r8 + 1);
  v_r7 = (v_r7 + 128);
  v_r5 = (v_r5 + 128);
  if (v_r8 <= 63)
    goto L_0200a618;
  t37 = *(s16 *)v_r10;
  t38 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(v_r6, s32, 8) = (t38 + (t37 << 17));
  t39 = *(s16 *)v_r9;
  t40 = FIELD_AT(v_r6, s32, 16);
  v_r5 = (t40 + (t39 << 17));
  FIELD_AT(v_r6, s32, 16) = v_r5;
  v_r4 = 0x10000;
  v_r7 = 0xfffff;
  v_r1 = (t38 + (t37 << 17));
  v_r2 = (((t38 + (t37 << 17)) & 0xfffff) + 0xfff80000);
  if (v_r2 <= v_r4)
    goto L_0200a686;
  v_r2 = v_r4;
L_0200a686:
  v_r0 = 0xffff0000;
  if (v_r2 >= v_r0)
    goto L_0200a68e;
  v_r2 = v_r0;
L_0200a68e:
  FIELD_AT(v_r6, s32, 8) = (v_r1 - v_r2);
  v_r2 = ((v_r5 & v_r7) + 0xfff80000);
  if (v_r2 <= v_r4)
    goto L_0200a6a0;
  v_r2 = v_r4;
L_0200a6a0:
  if (v_r2 >= v_r0)
    goto L_0200a6a6;
  v_r2 = v_r0;
L_0200a6a6:
  FIELD_AT(v_r6, s32, 16) = (v_r5 - v_r2);
  t41 = FIELD_AT(sp_, s32, 0);
  FIELD_AT(t41, u16, 4) = 0;
  Call1((void (*)())WaitFrames, 1);
  goto L_0200a53e;
L_0200a6b8:
  FIELD_AT(v_r6, s32, 108) = 0;
  Call2((void (*)())Animation_ApplyChildValues, v_r6, 0);
L_0200a6c4:
  t44 = FIELD_AT(sp_, s32, 0);
  FIELD_AT(t44, u16, 4) = 0;
  return t44;
}
