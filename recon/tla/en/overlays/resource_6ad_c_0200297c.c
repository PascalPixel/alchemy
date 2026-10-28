// Draft of overlay 6ad's Func_0200297c, translated from its listing: differ
// 269 of 236. Left: its shape: 249 instructions against the listing's 236.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 Func_02002800();
s32 Func_02005480();
s32 Func_020054d8();
s32 Func_020054f8();
s32 Func_02005538();
s32 Func_02005560();
s32 Func_020055a8();
s32 Func_02005648();
s32 Object_GetById();
s32 WaitFrames();
extern u8 Func_02002954[];
extern u8 gData_020023c4[];
extern u8 gAdventure[];

void Func_0200297c(void) {
  s32 sp_[1];
  s32 v_r5, v_r6, v_r10, v_r1, v_r9, v_r3, v_r0, v_r8, v_r7, v_r11;
  s32 v_r2, v_r4, t0, t2, t3, t7, t8, t9, t10, t11;
  s32 t12, t13, t14, t15, t16, t17, t18, t19, t20, t22;
  s32 t23, t24, t25, t26, t27, t28, t30, t31, t32, t33;
  s32 t34, t35, t37, t38, t39, t40, t41, t44;

  t0 = FIELD_AT((s32)gAdventure, u8, 530);
  v_r5 = (s32)gAdventure;
  if (t0 == 0)
    goto L_0200a99e;
  goto L_0200ab5c;
L_0200a99e:
  Func_02002800(-10, 0);
  t2 = FIELD_AT(v_r5, s32, 532);
  t3 = Object_GetById(t2);
  *(s32 *)sp_ = ((s32)gData_020023c4 + 0x188);
  Func_02005560(t3, 0, ((s32)gData_020023c4 + 0x188));
  Func_02005648(0);
  FIELD_AT(t3, s32, 108) = (s32)Func_02002954;
  FIELD_AT(t3, u16, 6) = 0x4000;
  Func_02005480(t3, 49);
  v_r6 = t3;
L_0200a9d6:
  t7 = FIELD_AT(v_r6, u8, 34);
  t8 = FIELD_AT(v_r6, s32, 8);
  t9 = FIELD_AT(v_r6, s32, 16);
  t10 = Func_020054d8(t7, t8, t9);
  t11 = *(s32 *)sp_;
  t12 = *(s32 *)sp_;
  v_r3 = (t11 + 104);
  v_r9 = (t12 + 106);
  v_r1 = ((s32)sp_ + 4);
  v_r10 = (t11 + 104);
  if (t10 != 7)
    goto L_0200aa52;
  t13 = *(s16 *)v_r3;
  FIELD_AT(v_r6, s32, 36) = (t13 << 17);
  t14 = *(s16 *)v_r9;
  FIELD_AT(v_r6, s32, 44) = (t14 << 17);
  FIELD_AT(v_r6, s32, 52) = 0x1000;
  v_r5 = 0;
L_0200aa10:
  t15 = *(s16 *)v_r10;
  t16 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(sp_, s32, 4) = (t16 + (t15 << 19));
  t17 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(sp_, s32, 8) = t17;
  t18 = *(s16 *)v_r9;
  t19 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(sp_, s32, 12) = (t19 + (t18 << 19));
  t20 = Func_020054f8(v_r6, ((s32)sp_ + 4));
  if (t20 == 0)
    goto L_0200aa44;
  FIELD_AT(v_r6, s32, 36) = 0;
  FIELD_AT(v_r6, s32, 44) = 0;
  goto L_0200ab50;
L_0200aa44:
  WaitFrames(1);
  v_r5 = (v_r5 + 1);
  if (v_r5 <= 9)
    goto L_0200aa10;
  goto L_0200ab50;
L_0200aa52:
  t22 = *(s16 *)v_r10;
  t23 = FIELD_AT(v_r6, s32, 8);
  *(s32 *)v_r1 = (t23 + (t22 << 19));
  t24 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(v_r1, s32, 4) = t24;
  t25 = *(s16 *)v_r9;
  t26 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(v_r1, s32, 8) = (t26 + (t25 << 19));
  t27 = Func_020054f8(v_r6, v_r1);
  v_r0 = t27;
  if (v_r0 > 0)
    goto L_0200ab50;
  if (v_r0 >= 0)
    goto L_0200aa9c;
  FIELD_AT(v_r6, u16, 6) = 0x4000;
  t28 = FIELD_AT((s32)gAdventure, s32, 532);
  Call3((void (*)())Func_020055a8, t28, 6, -1);
  goto L_0200ab50;
L_0200aa9c:
  t30 = FIELD_AT(v_r6, u16, 32);
  t31 = FIELD_AT(0x3000000, s32, 20);
  v_r11 = (t30 - 2);
  v_r7 = (t31 + 89);
  v_r8 = 0;
  v_r5 = t31;
L_0200aab0:
  t32 = *(s32 *)v_r5;
  if (t32 == 0)
    goto L_0200aae0;
  t33 = *(u8 *)v_r7;
  if ((1 & t33) == 0)
    goto L_0200aae0;
  if (v_r5 == v_r6)
    goto L_0200aae0;
  t34 = FIELD_AT(v_r5, u16, 32);
  t35 = Func_02005538((v_r5 + 8), v_r11, ((s32)sp_ + 4), (t34 - 2));
  if (t35 < 0)
    goto L_0200aae0;
  WaitFrames(1);
  goto L_0200ab50;
L_0200aae0:
  v_r8 = (v_r8 + 1);
  v_r7 = (v_r7 + 128);
  v_r5 = (v_r5 + 128);
  if (v_r8 <= 63)
    goto L_0200aab0;
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
  v_r2 = (((t38 + (t37 << 17)) & 0xfffff) - 0x80000);
  if (v_r2 <= v_r4)
    goto L_0200ab1e;
  v_r2 = v_r4;
L_0200ab1e:
  v_r0 = 0xffff0000;
  if (v_r2 >= v_r0)
    goto L_0200ab26;
  v_r2 = v_r0;
L_0200ab26:
  FIELD_AT(v_r6, s32, 8) = (v_r1 - v_r2);
  v_r2 = ((v_r5 & v_r7) - 0x80000);
  if (v_r2 <= v_r4)
    goto L_0200ab38;
  v_r2 = v_r4;
L_0200ab38:
  if (v_r2 >= v_r0)
    goto L_0200ab3e;
  v_r2 = v_r0;
L_0200ab3e:
  FIELD_AT(v_r6, s32, 16) = (v_r5 - v_r2);
  t41 = *(s32 *)sp_;
  FIELD_AT(t41, u16, 4) = 0;
  WaitFrames(1);
  goto L_0200a9d6;
L_0200ab50:
  FIELD_AT(v_r6, s32, 108) = 0;
  Animation_ApplyChildValues(v_r6, 0);
L_0200ab5c:
  t44 = *(s32 *)sp_;
  FIELD_AT(t44, u16, 4) = 0;
  return;
}
