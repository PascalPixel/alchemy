// Draft of overlay 6ab's Func_02002148, translated from its listing: differ
// 242 of 236. Left: instruction order and register choice.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 Func_02001fcc();
s32 Func_020041cc();
s32 Func_0200422c();
s32 Func_0200424c();
s32 Func_0200428c();
s32 Func_020042dc();
s32 Func_0200436c();
s32 Func_0200444c();
s32 Actor_GetById();
s32 wait();
extern u8 Func_02002120[];
extern u8 gData_020023c4[];
extern u8 gAdventure[];

s32 Func_02002148(s32 p0, s32 p1, s32 p2) {
  s32 sp_[1];
  s32 v_r2, v_r3, v_r5, v_r6, v_r10, v_r1, v_r9, v_r0, v_r8, v_r7;
  s32 v_r11, v_r4, t0, t2, t3, t7, t8, t9, t10, t11;
  s32 t12, t13, t14, t15, t16, t17, t18, t19, t20, t22;
  s32 t23, t24, t25, t26, t27, t28, t30, t31, t32, t33;
  s32 t34, t35, t37, t38, t39, t40, t41, t44;

  t0 = *(u8 *)((s32)gAdventure + 0x212);
  v_r5 = (s32)gAdventure;
  v_r3 = t0;
  v_r2 = p2;
  if (v_r3 == 0)
    goto L_0200a16a;
  goto L_0200a328;
L_0200a16a:
  Call4((void (*)())Func_02001fcc, -10, 0, v_r2, v_r3);
  t2 = *(s32 *)(v_r5 + 0x214);
  t3 = Value1((s32 (*)())Actor_GetById, t2);
  FIELD_AT(sp_, s32, 0) = ((s32)gData_020023c4 + 0x188);
  Func_020042dc();
  Call1((void (*)())Func_0200444c, 0);
  FIELD_AT(t3, s32, 108) = (s32)Func_02002120;
  FIELD_AT(t3, u16, 6) = 0x4000;
  Call2((void (*)())Func_020041cc, t3, 49);
  v_r6 = t3;
L_0200a1a2:
  t7 = *(u8 *)(v_r6 + 34);
  t8 = FIELD_AT(v_r6, s32, 8);
  t9 = FIELD_AT(v_r6, s32, 16);
  t10 = Value3((s32 (*)())Func_0200422c, t7, t8, t9);
  t11 = FIELD_AT(sp_, s32, 0);
  t12 = FIELD_AT(sp_, s32, 0);
  v_r3 = (t11 + 104);
  v_r9 = (t12 + 106);
  v_r1 = ((s32)sp_ + 4);
  v_r10 = (t11 + 104);
  if (t10 != 7)
    goto L_0200a21e;
  t13 = *(s16 *)v_r3;
  FIELD_AT(v_r6, s32, 36) = (t13 << 17);
  t14 = *(s16 *)v_r9;
  FIELD_AT(v_r6, s32, 44) = (t14 << 17);
  FIELD_AT(v_r6, s32, 52) = 0x1000;
  v_r5 = 0;
L_0200a1dc:
  t15 = *(s16 *)v_r10;
  t16 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(sp_, s32, 4) = (t16 + (t15 << 19));
  t17 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(sp_, s32, 8) = t17;
  t18 = *(s16 *)v_r9;
  t19 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(sp_, s32, 12) = (t19 + (t18 << 19));
  t20 = Value1((s32 (*)())Func_0200424c, v_r6);
  if (t20 == 0)
    goto L_0200a210;
  FIELD_AT(v_r6, s32, 36) = 0;
  FIELD_AT(v_r6, s32, 44) = 0;
  goto L_0200a31c;
L_0200a210:
  Call1((void (*)())wait, 1);
  v_r5 = (v_r5 + 1);
  if (v_r5 <= 9)
    goto L_0200a1dc;
  goto L_0200a31c;
L_0200a21e:
  t22 = *(s16 *)v_r10;
  t23 = FIELD_AT(v_r6, s32, 8);
  *(s32 *)v_r1 = (t23 + (t22 << 19));
  t24 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(v_r1, s32, 4) = t24;
  t25 = *(s16 *)v_r9;
  t26 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(v_r1, s32, 8) = (t26 + (t25 << 19));
  t27 = Value1((s32 (*)())Func_0200424c, v_r6);
  v_r0 = t27;
  if (v_r0 > 0)
    goto L_0200a31c;
  if (v_r0 >= 0)
    goto L_0200a268;
  FIELD_AT(v_r6, u16, 6) = 0x4000;
  t28 = *(s32 *)((s32)gAdventure + 0x214);
  Call3((void (*)())Func_0200436c, t28, 6, -1);
  goto L_0200a31c;
L_0200a268:
  t30 = FIELD_AT(v_r6, u16, 32);
  t31 = FIELD_AT(0x3000000, s32, 20);
  v_r11 = (t30 - 2);
  v_r7 = (t31 + 89);
  v_r8 = 0;
  v_r5 = t31;
L_0200a27c:
  t32 = *(s32 *)v_r5;
  if (t32 == 0)
    goto L_0200a2ac;
  t33 = *(u8 *)v_r7;
  if ((1 & t33) == 0)
    goto L_0200a2ac;
  if (v_r5 == v_r6)
    goto L_0200a2ac;
  t34 = FIELD_AT(v_r5, u16, 32);
  t35 = Value4((s32 (*)())Func_0200428c, (v_r5 + 8), v_r11, ((s32)sp_ + 4),
               (t34 - 2));
  if (t35 < 0)
    goto L_0200a2ac;
  Call1((void (*)())wait, 1);
  goto L_0200a31c;
L_0200a2ac:
  v_r8 = (v_r8 + 1);
  v_r7 = (v_r7 + 128);
  v_r5 = (v_r5 + 128);
  if (v_r8 <= 63)
    goto L_0200a27c;
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
    goto L_0200a2ea;
  v_r2 = v_r4;
L_0200a2ea:
  v_r0 = 0xffff0000;
  if (v_r2 >= v_r0)
    goto L_0200a2f2;
  v_r2 = v_r0;
L_0200a2f2:
  FIELD_AT(v_r6, s32, 8) = (v_r1 - v_r2);
  v_r2 = ((v_r5 & v_r7) + 0xfff80000);
  if (v_r2 <= v_r4)
    goto L_0200a304;
  v_r2 = v_r4;
L_0200a304:
  if (v_r2 >= v_r0)
    goto L_0200a30a;
  v_r2 = v_r0;
L_0200a30a:
  FIELD_AT(v_r6, s32, 16) = (v_r5 - v_r2);
  t41 = FIELD_AT(sp_, s32, 0);
  FIELD_AT(t41, u16, 4) = 0;
  Call1((void (*)())wait, 1);
  goto L_0200a1a2;
L_0200a31c:
  FIELD_AT(v_r6, s32, 108) = 0;
  Call2((void (*)())Animation_ApplyChildValues, v_r6, 0);
L_0200a328:
  t44 = FIELD_AT(sp_, s32, 0);
  FIELD_AT(t44, u16, 4) = 0;
  return t44;
}
