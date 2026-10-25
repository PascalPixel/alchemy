// Draft of overlay 6ad's Func_02002f80, translated from its listing: differ
// 374 of 362. Left: its shape: 376 instructions against the listing's 362.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02002d08();
s32 Func_02002e8c();
s32 Func_020054b8();
s32 Func_020054d8();
s32 Func_020054e8();
s32 Func_020054f0();
s32 Func_02005510();
s32 Func_020056c0();
s32 GameFlag_Test();
s32 Object_GetById();
s32 Vector_AddPolarOffsetFar();
extern u8 gData_020023c4[];
extern u8 gPartyState[];

void Func_02002f80(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4, s32 p5, s32 p6) {
  s32 sp_[1];
  s32 spo_[2];
  s32 v_r6, v_r5, v_r8, v_r3, v_r0, v_r7, v_r11, v_r9, v_r10, v_r2;
  s32 v_r4, v_r1, t0, t1, t2, t3, t4, t5, t6, t7;
  s32 t9, t10, t11, t15, t16, t17, t18, t19, t20, t21;
  s32 t22, t23, t24, t25, t26, t27, t28, t32, t33, t35;
  s32 t36, t37, t38, t42, t43, t45, t46, t47, t48, t49;
  s32 t50, t52, t53, t54, t55, t56, t57, t58, t59, t60;
  s32 t61, t62, t63;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = FIELD_AT(p0, s16, 100);
  v_r3 = t1;
  v_r8 = (p0 + 100);
  v_r5 = p0;
  v_r6 = t0;
  if (v_r3 == 0)
    goto L_0200afa8;
  goto L_0200b234;
L_0200afa8:
  t2 = Value1((s32 (*)())GameFlag_Test, 0x106);
  t3 = FIELD_AT(v_r6, s16, 358);
  v_r0 = t2;
  if (t3 == 0)
    goto L_0200afc2;
  v_r0 = (v_r0 | 1);
L_0200afc2:
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
  t11 = Value3((s32 (*)())Func_020054d8, 0, t9, t10);
  v_r7 = ((s32)sp_ + 36);
  if (t11 != 7)
    goto L_0200b038;
  FIELD_AT(v_r5, s32, 64) = 0x80000000;
  FIELD_AT(v_r5, s32, 60) = 0x80000000;
  FIELD_AT(v_r5, s32, 56) = 0x80000000;
  FIELD_AT(v_r5, s32, 52) = 0x4000;
  *(u16 *)v_r8 = 1;
  Call1((void (*)())Func_020056c0, 145);
  Call3((void (*)())Func_02005510, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02005510, -1, -1, 0xe666);
  t15 = FIELD_AT(v_r5, s32, 104);
  v_r3 = t15;
  if (v_r3 == 0)
    goto L_0200b038;
L_0200b038:
  t16 = FIELD_AT(0x3000000, s32, 32);
  t17 = FIELD_AT(v_r5, s32, 8);
  t18 = FIELD_AT(v_r5, s32, 16);
  p4 = (t17 >> 20);
  t19 = FIELD_AT(t16, s32, 368);
  FIELD_AT(sp_, s32, 20) = (t19 + ((p4 + ((t18 >> 20) << 7)) << 2));
  t20 = FIELD_AT(t16, s32, 424);
  FIELD_AT(sp_, s32, 16) = (t20 + ((p4 + ((t18 >> 20) << 7)) << 2));
  t21 = FIELD_AT(t16, s32, 328);
  FIELD_AT(sp_, s32, 12) = (t21 >> 20);
  t22 = FIELD_AT(t16, s32, 332);
  t23 = FIELD_AT(t16, s32, 384);
  FIELD_AT(sp_, s32, 8) = (t23 >> 20);
  t24 = FIELD_AT(t16, s32, 388);
  *(s32 *)sp_ = (((t18 >> 20) + (t24 >> 20)) - 2);
  p6 = -1;
  v_r11 = (((t18 >> 20) + (t22 >> 20)) - 1);
  v_r8 = ((t18 >> 20) - 1);
L_0200b0a4:
  t25 = FIELD_AT(sp_, s32, 8);
  t26 = FIELD_AT(sp_, s32, 12);
  p5 = -1;
  v_r10 = ((p4 + t26) - 1);
  v_r6 = (p4 - 1);
  v_r9 = ((p4 + t25) - 1);
L_0200b0c0:
  t27 = FIELD_AT(sp_, s32, 20);
  FIELD_AT(sp_, s32, 4) = (((p6 << 7) + p5) << 2);
  t28 = FIELD_AT(((((p6 << 7) + p5) << 2) + t27), u8, 2);
  v_r2 = ((((p6 << 7) + p5) << 2) + t27);
  if (t28 != 77)
    goto L_0200b120;
  FIELD_AT(v_r2, u8, 2) = 0;
  Call6((void (*)())Func_020054f0, 64, 64, 1, 1, v_r10, v_r11);
  Call6((void (*)())Func_020054e8, 64, 64, 1, 1, v_r6, v_r8);
  Call1((void (*)())Func_020056c0, 0x23d);
  t32 = FIELD_AT(v_r5, s32, 12);
  t33 = FIELD_AT(v_r5, u16, 6);
  Call4((void (*)())Func_02002e8c, ((v_r6 << 20) + 0x80000), t32,
        ((v_r8 << 20) + 0x80000), t33);
L_0200b120:
  t35 = FIELD_AT(sp_, s32, 4);
  t36 = FIELD_AT(sp_, s32, 16);
  t37 = FIELD_AT((t35 + t36), u8, 2);
  v_r2 = (t35 + t36);
  if (t37 != 77)
    goto L_0200b174;
  FIELD_AT(v_r2, u8, 2) = 0;
  t38 = *(s32 *)sp_;
  Call6((void (*)())Func_020054f0, 64, 64, 1, 2, v_r9, t38);
  Call6((void (*)())Func_020054e8, 64, 64, 1, 1, v_r6, v_r8);
  Call1((void (*)())Func_020056c0, 0x23c);
  t42 = FIELD_AT(v_r5, s32, 12);
  t43 = FIELD_AT(v_r5, u16, 6);
  Func_02002e8c(((v_r6 << 20) + 0x80000), t42, ((v_r8 << 20) + 0x80000), t43);
L_0200b174:
  p5 = (p5 + 1);
  v_r4 = 1;
  v_r10 = (v_r10 + 1);
  v_r6 = (v_r6 + 1);
  v_r9 = (v_r9 + 1);
  if ((p5 + 1) <= v_r4)
    goto L_0200b0c0;
  t45 = *(s32 *)sp_;
  *(s32 *)sp_ = (t45 + 1);
  p6 = (p6 + 1);
  v_r11 = (v_r11 + v_r4);
  v_r8 = (v_r8 + v_r4);
  if ((p6 + 1) <= 1)
    goto L_0200b0a4;
  t46 = FIELD_AT(v_r5, s32, 24);
  v_r3 = t46;
  if (v_r3 >= 0x10000)
    goto L_0200b1b2;
  FIELD_AT(v_r5, s32, 24) = (v_r3 + 0x1000);
  t47 = FIELD_AT(v_r5, s32, 28);
  FIELD_AT(v_r5, s32, 28) = (t47 + 0x1000);
L_0200b1b2:
  t48 = *(s32 *)v_r7;
  t49 = FIELD_AT(v_r7, s32, 4);
  t50 = FIELD_AT(v_r7, s32, 8);
  Call4((void (*)())Func_020054b8, v_r5, t48, t49, t50);
  t52 = FIELD_AT(0x3000000, s32, 108);
  t53 = FIELD_AT((s32)gPartyState, s32, 532);
  t54 = Value1((s32 (*)())Object_GetById, t53);
  t55 = FIELD_AT(v_r5, s32, 8);
  t56 = FIELD_AT(t54, s32, 8);
  v_r2 = (t55 - t56);
  v_r7 = t52;
  v_r6 = ((s32)gData_020023c4 + 0x188);
  v_r0 = t54;
  v_r1 = t55;
  v_r3 = t56;
  if (v_r2 < 0)
    goto L_0200b1e8;
  if (v_r2 < 0x140000)
    goto L_0200b1f2;
  goto L_0200b268;
L_0200b1e8:
  if ((v_r3 - v_r1) >= 0x140000)
    goto L_0200b268;
L_0200b1f2:
  t57 = FIELD_AT(v_r5, s32, 12);
  t58 = FIELD_AT(v_r0, s32, 12);
  if ((u32)((t57 - t58) + 0x7ffff) > (u32)0x1ffffe)
    goto L_0200b268;
  t59 = FIELD_AT(v_r5, s32, 16);
  t60 = FIELD_AT(v_r0, s32, 16);
  v_r2 = (t59 - t60);
  v_r3 = t59;
  v_r0 = t60;
  if (v_r2 < 0)
    goto L_0200b216;
  if (v_r2 < 0x140000)
    goto L_0200b220;
  goto L_0200b268;
L_0200b216:
  if ((v_r0 - v_r3) >= 0x140000)
    goto L_0200b268;
L_0200b220:
  FIELD_AT(v_r6, u16, 4) = 2;
  t61 = FIELD_AT(v_r6, u16, 10);
  FIELD_AT(v_r6, s32, 108) = v_r5;
  FIELD_AT(v_r7, u16, 340) = (t61 + 2);
  goto L_0200b268;
L_0200b234:
  if (v_r3 != 1)
    goto L_0200b268;
  FIELD_AT(v_r5, u8, 91) = 0;
  t62 = FIELD_AT(v_r5, s32, 24);
  v_r3 = t62;
  v_r2 = 0;
  if (v_r3 <= v_r2)
    goto L_0200b25a;
  FIELD_AT(v_r5, s32, 24) = (v_r3 - 0x1000);
  t63 = FIELD_AT(v_r5, s32, 28);
  FIELD_AT(v_r5, s32, 28) = (t63 - 0x1000);
  Call1((void (*)())Func_02002d08, v_r5);
  goto L_0200b268;
L_0200b25a:
  FIELD_AT(v_r5, s32, 16) = v_r2;
  FIELD_AT(v_r5, s32, 12) = v_r2;
  FIELD_AT(v_r5, s32, 8) = v_r2;
  FIELD_AT(v_r5, s32, 44) = v_r2;
  FIELD_AT(v_r5, s32, 40) = v_r2;
  FIELD_AT(v_r5, s32, 36) = v_r2;
  FIELD_AT(v_r5, s32, 108) = v_r2;
L_0200b268:
  return;
}
