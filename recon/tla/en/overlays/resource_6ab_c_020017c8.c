// Draft of overlay 6ab's Func_020017c8, translated from its listing: differ
// 169 of 167. Left: its shape: 164 instructions against the listing's 167.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02001528();
s32 Func_0200158c();
s32 Actor_GetById();
s32 __divsi3();
extern u8 gData_020023c4[];
extern u8 gSequenceParentWork[];

void Func_020017c8(s32 p0) {
  s32 sp_[1];
  s32 v_r5, v_r3, v_r7, v_r6, v_r4, v_r8, v_r10, v_r9, v_r2, v_r1;
  s32 v_r0, t0, t1, t2, t3, t4, t5, t6, t7, t8;
  s32 t9, t10, t11, t12, t13, t14, t15, t16, t17, t18;
  s32 t19, t20, t21, t22, t23, t24, t25, t26, t27, t28;
  s32 t29, t30, t31, t32, t33, t34, t35, t36;

  t0 = FIELD_AT((s32)gSequenceParentWork, s32, 532);
  t1 = Actor_GetById(t0);
  t2 = FIELD_AT(0x3000000, s32, 108);
  t3 = FIELD_AT(p0, s32, 68);
  t4 = FIELD_AT(p0, s32, 16);
  t5 = FIELD_AT(p0, s32, 72);
  FIELD_AT(p0, s32, 16) = (t4 + t3);
  t6 = FIELD_AT(p0, s32, 12);
  t7 = FIELD_AT(p0, s32, 76);
  FIELD_AT(p0, s32, 12) = (t6 + t5);
  t8 = FIELD_AT(p0, s32, 8);
  FIELD_AT(p0, s32, 8) = (t8 + t7);
  *(s32 *)sp_ = t1;
  t9 = __divsi3(t3, 18);
  v_r5 = (t3 - t9);
  FIELD_AT(p0, s32, 68) = v_r5;
  t10 = *(s32 *)sp_;
  v_r9 = t2;
  v_r10 = (s32)gSequenceParentWork;
  v_r8 = ((s32)gData_020023c4 + 0x188);
  v_r4 = t10;
  v_r6 = p0;
  v_r7 = t7;
  v_r3 = t7;
  if (v_r7 >= 0)
    goto L_02009824;
  v_r3 = (v_r3 + 15);
L_02009824:
  FIELD_AT(v_r6, s32, 76) = (v_r7 - (v_r3 >> 4));
  t11 = FIELD_AT(v_r6, s32, 48);
  t12 = FIELD_AT(v_r6, s32, 24);
  t13 = FIELD_AT(v_r6, s32, 80);
  FIELD_AT(v_r6, s32, 24) = (t12 + t11);
  t14 = FIELD_AT(v_r6, s32, 52);
  t15 = FIELD_AT(v_r6, s32, 28);
  FIELD_AT(v_r6, s32, 28) = (t15 + t14);
  t16 = FIELD_AT(t13, u16, 18);
  t17 = FIELD_AT(v_r6, u16, 100);
  FIELD_AT(t13, u16, 18) = (t16 + t17);
  t18 = FIELD_AT(v_r4, s32, 12);
  t19 = FIELD_AT(v_r4, s32, 20);
  if (t18 != t19)
    goto L_020098b4;
  t20 = Func_02001528(v_r6, v_r4);
  if (t20 == 0)
    goto L_020098b4;
  t21 = FIELD_AT(v_r10, u8, 530);
  v_r3 = t21;
  if (v_r3 != 0)
    goto L_020098b4;
  t22 = FIELD_AT(v_r6, s32, 76);
  v_r2 = t22;
  if (v_r2 == 0)
    goto L_02009888;
  FIELD_AT(v_r8, u16, 106) = v_r3;
  v_r1 = (v_r8 + 104);
  if (v_r2 <= 0)
    goto L_02009880;
  v_r3 = 1;
  goto L_02009886;
L_02009880:
  v_r3 = 0xffff;
L_02009886:
  *(u16 *)v_r1 = v_r3;
L_02009888:
  FIELD_AT(v_r6, u8, 35) = 2;
  t23 = FIELD_AT(v_r6, s32, 16);
  t24 = FIELD_AT(v_r6, s32, 68);
  FIELD_AT(v_r6, s32, 76) = 0;
  FIELD_AT(v_r6, s32, 16) = (t23 - t24);
  t25 = FIELD_AT(v_r6, s32, 72);
  FIELD_AT(v_r6, s32, 72) = (t25 + (t24 >> 2));
  FIELD_AT(v_r8, u16, 4) = 1;
  t26 = FIELD_AT(v_r8, u16, 10);
  FIELD_AT(v_r9, u16, 340) = t26;
L_020098b4:
  t27 = FIELD_AT(v_r8, u16, 84);
  v_r7 = 0;
  v_r0 = t27;
  if (v_r0 == v_r7)
    goto L_02009914;
  v_r5 = (v_r8 + 84);
L_020098c4:
  t28 = Actor_GetById(v_r0);
  t29 = FIELD_AT(t28, s32, 12);
  t30 = FIELD_AT(t28, s32, 20);
  v_r4 = t28;
  if (t29 != t30)
    goto L_02009906;
  t31 = Func_0200158c(v_r6, v_r4);
  if (t31 == 0)
    goto L_02009906;
  t32 = FIELD_AT(v_r5, u16, 2);
  v_r1 = t32;
  if (v_r1 != 0)
    goto L_02009900;
  FIELD_AT(v_r6, u8, 35) = 2;
  t33 = FIELD_AT(v_r6, s32, 76);
  t34 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(v_r6, s32, 76) = v_r1;
  FIELD_AT(v_r6, s32, 16) = (t34 - t33);
  t35 = FIELD_AT(v_r6, s32, 72);
  FIELD_AT(v_r6, s32, 72) = (t35 + (t33 >> 2));
  goto L_02009906;
L_02009900:
  FIELD_AT(v_r8, u16, 6) = 1;
L_02009906:
  v_r7 = (v_r7 + 1);
  v_r5 = (v_r5 + 4);
  if (v_r7 > 3)
    goto L_02009914;
  t36 = *(u16 *)v_r5;
  v_r0 = t36;
  if (v_r0 != 0)
    goto L_020098c4;
L_02009914:
  return;
}
