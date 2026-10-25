// Draft of overlay 6ab's Func_0200274c, translated from its listing: differ
// 359 of 362. Left: its shape: 381 instructions against the listing's 362.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020024d4();
s32 Func_02002658();
s32 Func_02004204();
s32 Func_0200422c();
s32 Func_0200423c();
s32 Func_02004244();
s32 Func_02004264();
s32 Func_0200446c();
s32 GameFlag_Test();
s32 Object_GetById();
s32 Vector_AddPolarOffsetFar();
extern u8 gData_020023c4[];
extern u8 gPartyState[];

void Func_0200274c(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4, s32 p5, s32 p6) {
  s32 sp_[1];
  s32 spo_[2];
  s32 v_r6, v_r5, v_r8, v_r3, v_r0, v_r7, v_r11, v_r9, v_r10, v_r2;
  s32 v_r4, v_r1, t0, t1, t2, t3, t4, t5, t6, t7;
  s32 t9, t10, t11, t15, t17, t18, t19, t20, t21, t22;
  s32 t23, t24, t25, t26, t27, t28, t29, t33, t34, t36;
  s32 t37, t38, t39, t43, t44, t46, t47, t48, t49, t50;
  s32 t51, t53, t54, t55, t56, t57, t58, t59, t60, t61;
  s32 t62, t63, t64;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = FIELD_AT(p0, s16, 100);
  v_r3 = t1;
  v_r8 = (p0 + 100);
  v_r5 = p0;
  v_r6 = t0;
  if (v_r3 == 0)
    goto L_0200a774;
  goto L_0200aa00;
L_0200a774:
  t2 = Value1((s32 (*)())GameFlag_Test, 0x106);
  t3 = FIELD_AT(v_r6, s16, 358);
  v_r0 = t2;
  if (t3 == 0)
    goto L_0200a78e;
  v_r0 = (v_r0 | 1);
L_0200a78e:
  FIELD_AT(v_r5, u8, 91) = v_r0;
  t4 = FIELD_AT(v_r5, s32, 8);
  FIELD_AT(sp_, s32, 36) = t4;
  t5 = FIELD_AT(v_r5, s32, 12);
  FIELD_AT(sp_, s32, 40) = t5;
  t6 = FIELD_AT(v_r5, s32, 16);
  FIELD_AT(sp_, s32, 44) = t6;
  t7 = FIELD_AT(v_r5, u16, 6);
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x80000, t7, ((s32)sp_ + 36));
  t9 = FIELD_AT(sp_, s32, 36);
  t10 = FIELD_AT(sp_, s32, 44);
  t11 = Value3((s32 (*)())Func_0200422c, 0, t9, t10);
  v_r7 = ((s32)sp_ + 36);
  if (t11 != 7)
    goto L_0200a804;
  FIELD_AT(v_r5, s32, 64) = 0x80000000;
  FIELD_AT(v_r5, s32, 60) = 0x80000000;
  FIELD_AT(v_r5, s32, 56) = 0x80000000;
  FIELD_AT(v_r5, s32, 52) = 0x4000;
  *(u16 *)v_r8 = 1;
  Call1((void (*)())Func_0200446c, 145);
  Call3((void (*)())Func_02004264, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02004264, -1, -1, 0xe666);
  t15 = FIELD_AT(v_r5, s32, 104);
  v_r3 = t15;
  if (v_r3 == 0)
    goto L_0200a804;
  ((s32 (*)())v_r3)(v_r5);
L_0200a804:
  t17 = FIELD_AT(0x3000000, s32, 32);
  t18 = FIELD_AT(v_r5, s32, 8);
  t19 = FIELD_AT(v_r5, s32, 16);
  p4 = (t18 >> 20);
  t20 = FIELD_AT(t17, s32, 368);
  FIELD_AT(sp_, s32, 20) = (t20 + ((p4 + ((t19 >> 20) << 7)) << 2));
  t21 = FIELD_AT(t17, s32, 424);
  FIELD_AT(sp_, s32, 16) = (t21 + ((p4 + ((t19 >> 20) << 7)) << 2));
  t22 = FIELD_AT(t17, s32, 328);
  FIELD_AT(sp_, s32, 12) = (t22 >> 20);
  t23 = FIELD_AT(t17, s32, 332);
  t24 = FIELD_AT(t17, s32, 384);
  FIELD_AT(sp_, s32, 8) = (t24 >> 20);
  t25 = FIELD_AT(t17, s32, 388);
  *(s32 *)sp_ = (((t19 >> 20) + (t25 >> 20)) - 2);
  p6 = -1;
  v_r11 = (((t19 >> 20) + (t23 >> 20)) - 1);
  v_r8 = ((t19 >> 20) - 1);
L_0200a870:
  t26 = FIELD_AT(sp_, s32, 8);
  t27 = FIELD_AT(sp_, s32, 12);
  p5 = -1;
  v_r10 = ((p4 + t27) - 1);
  v_r6 = (p4 - 1);
  v_r9 = ((p4 + t26) - 1);
L_0200a88c:
  t28 = FIELD_AT(sp_, s32, 20);
  FIELD_AT(sp_, s32, 4) = (((p6 << 7) + p5) << 2);
  t29 = FIELD_AT(((((p6 << 7) + p5) << 2) + t28), u8, 2);
  v_r2 = ((((p6 << 7) + p5) << 2) + t28);
  if (t29 != 77)
    goto L_0200a8ec;
  FIELD_AT(v_r2, u8, 2) = 0;
  Call6((void (*)())Func_02004244, 64, 64, 1, 1, v_r10, v_r11);
  Call6((void (*)())Func_0200423c, 64, 64, 1, 1, v_r6, v_r8);
  Call1((void (*)())Func_0200446c, 0x23d);
  t33 = FIELD_AT(v_r5, s32, 12);
  t34 = FIELD_AT(v_r5, u16, 6);
  Call4((void (*)())Func_02002658, ((v_r6 << 20) + 0x80000), t33,
        ((v_r8 << 20) + 0x80000), t34);
L_0200a8ec:
  t36 = FIELD_AT(sp_, s32, 4);
  t37 = FIELD_AT(sp_, s32, 16);
  t38 = FIELD_AT((t36 + t37), u8, 2);
  v_r2 = (t36 + t37);
  if (t38 != 77)
    goto L_0200a940;
  FIELD_AT(v_r2, u8, 2) = 0;
  t39 = *(s32 *)sp_;
  Call6((void (*)())Func_02004244, 64, 64, 1, 2, v_r9, t39);
  Call6((void (*)())Func_0200423c, 64, 64, 1, 1, v_r6, v_r8);
  Call1((void (*)())Func_0200446c, 0x23c);
  t43 = FIELD_AT(v_r5, s32, 12);
  t44 = FIELD_AT(v_r5, u16, 6);
  Func_02002658(((v_r6 << 20) + 0x80000), t43, ((v_r8 << 20) + 0x80000), t44);
L_0200a940:
  p5 = (p5 + 1);
  v_r4 = 1;
  v_r10 = (v_r10 + 1);
  v_r6 = (v_r6 + 1);
  v_r9 = (v_r9 + 1);
  if ((p5 + 1) <= v_r4)
    goto L_0200a88c;
  t46 = *(s32 *)sp_;
  *(s32 *)sp_ = (t46 + 1);
  p6 = (p6 + 1);
  v_r11 = (v_r11 + v_r4);
  v_r8 = (v_r8 + v_r4);
  if ((p6 + 1) <= 1)
    goto L_0200a870;
  t47 = FIELD_AT(v_r5, s32, 24);
  v_r3 = t47;
  if (v_r3 >= 0x10000)
    goto L_0200a97e;
  FIELD_AT(v_r5, s32, 24) = (v_r3 + 0x1000);
  t48 = FIELD_AT(v_r5, s32, 28);
  FIELD_AT(v_r5, s32, 28) = (t48 + 0x1000);
L_0200a97e:
  t49 = *(s32 *)v_r7;
  t50 = FIELD_AT(v_r7, s32, 4);
  t51 = FIELD_AT(v_r7, s32, 8);
  Call4((void (*)())Func_02004204, v_r5, t49, t50, t51);
  t53 = FIELD_AT(0x3000000, s32, 108);
  t54 = FIELD_AT((s32)gPartyState, s32, 532);
  t55 = Value1((s32 (*)())Object_GetById, t54);
  t56 = FIELD_AT(v_r5, s32, 8);
  t57 = FIELD_AT(t55, s32, 8);
  v_r2 = (t56 - t57);
  v_r7 = t53;
  v_r6 = ((s32)gData_020023c4 + 0x188);
  v_r0 = t55;
  v_r1 = t56;
  v_r3 = t57;
  if (v_r2 < 0)
    goto L_0200a9b4;
  if (v_r2 < 0x140000)
    goto L_0200a9be;
  goto L_0200aa34;
L_0200a9b4:
  if ((v_r3 - v_r1) >= 0x140000)
    goto L_0200aa34;
L_0200a9be:
  t58 = FIELD_AT(v_r5, s32, 12);
  t59 = FIELD_AT(v_r0, s32, 12);
  if ((u32)((t58 - t59) + 0x7ffff) > (u32)0x1ffffe)
    goto L_0200aa34;
  t60 = FIELD_AT(v_r5, s32, 16);
  t61 = FIELD_AT(v_r0, s32, 16);
  v_r2 = (t60 - t61);
  v_r3 = t60;
  v_r0 = t61;
  if (v_r2 < 0)
    goto L_0200a9e2;
  if (v_r2 < 0x140000)
    goto L_0200a9ec;
  goto L_0200aa34;
L_0200a9e2:
  if ((v_r0 - v_r3) >= 0x140000)
    goto L_0200aa34;
L_0200a9ec:
  FIELD_AT(v_r6, u16, 4) = 2;
  t62 = FIELD_AT(v_r6, u16, 10);
  FIELD_AT(v_r6, s32, 108) = v_r5;
  FIELD_AT(v_r7, u16, 340) = (t62 + 2);
  goto L_0200aa34;
L_0200aa00:
  if (v_r3 != 1)
    goto L_0200aa34;
  FIELD_AT(v_r5, u8, 91) = 0;
  t63 = FIELD_AT(v_r5, s32, 24);
  v_r3 = t63;
  v_r2 = 0;
  if (v_r3 <= v_r2)
    goto L_0200aa26;
  FIELD_AT(v_r5, s32, 24) = (v_r3 - 0x1000);
  t64 = FIELD_AT(v_r5, s32, 28);
  FIELD_AT(v_r5, s32, 28) = (t64 - 0x1000);
  Call1((void (*)())Func_020024d4, v_r5);
  goto L_0200aa34;
L_0200aa26:
  FIELD_AT(v_r5, s32, 16) = v_r2;
  FIELD_AT(v_r5, s32, 12) = v_r2;
  FIELD_AT(v_r5, s32, 8) = v_r2;
  FIELD_AT(v_r5, s32, 44) = v_r2;
  FIELD_AT(v_r5, s32, 40) = v_r2;
  FIELD_AT(v_r5, s32, 36) = v_r2;
  FIELD_AT(v_r5, s32, 108) = v_r2;
L_0200aa34:
  return;
}
