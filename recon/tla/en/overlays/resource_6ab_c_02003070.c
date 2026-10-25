// Draft of overlay 6ab's Func_02003070, translated from its listing: differ
// 401 of 347. Left: its shape: 354 instructions against the listing's 347.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 ArcTan2();
s32 Func_02002d90();
s32 Func_020041cc();
s32 Func_02004204();
s32 Func_0200420c();
s32 Func_02004224();
s32 Func_02004284();
s32 Func_0200428c();
s32 Map_GetTerrainHeight();
s32 ObjectDispatch_SetAnimationSpeed();
s32 ObjectTable_Get();
s32 Vector_AddPolarOffsetFar();
s32 WaitFrames();
extern u8 Func_02002d04[];
extern u8 IwramMulQ16[];
extern u8 gData_0200483c[];
extern u8 gData_02004cf0[];
extern u8 gInput[];
extern u8 gPartyState[];

void Func_02003070(void) {
  s32 sp_[1];
  s32 v_r7, v_r0, v_r1, v_r2, v_r10, v_r6, v_r5, v_r9, v_r11, v_r8;
  s32 t0, t1, t3, t4, t5, t6, t7, t8, t9, t10;
  s32 t11, t12, t13, t14, t15, t16, t17, t18, t19, t20;
  s32 t22, t23, t24, t25, t26, t27, t28, t29, t30, t31;
  s32 t32, t33, t34, t35, t36, t37, t41, t42, t43, t45;
  s32 t46, t47, t48, t49, t50, t51, t52, t53, t54, t55;
  s32 t56, t57, t58, t59, t60, t61, t62, t63, t64, t67;
  s32 t68, t69, t70, t72, t73, t74, t75, t76, t77, t78;
  s32 t79, t84;

  t0 = *(s32 *)((s32)gPartyState + 0x214);
  t1 = Value1((s32 (*)())ObjectTable_Get, t0);
  *(u16 *)(s32)gData_02004cf0 = 0;
  v_r7 = t1;
L_0200b096:
  Func_02002d90();
  FIELD_AT(v_r7, s32, 48) = 0x10000;
  FIELD_AT(v_r7, s32, 52) = 0x8000;
  FIELD_AT(v_r7, s32, 56) = 0x80000000;
  FIELD_AT(v_r7, s32, 64) = 0x80000000;
  FIELD_AT(v_r7, s32, 36) = 0;
  FIELD_AT(v_r7, s32, 44) = 0;
  t3 = FIELD_AT(v_r7, s32, 8);
  FIELD_AT(sp_, s32, 12) = ((t3 & 0xfff00000) + 0x80000);
  FIELD_AT(sp_, s32, 28) = ((t3 & 0xfff00000) + 0x80000);
  v_r10 = ((s32)sp_ + 28);
  v_r2 = 0x80000;
  v_r1 = 0xfff00000;
  v_r0 = ((s32)sp_ + 28);
  goto L_0200b0dc;
L_0200b0dc:
  t4 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(v_r0, s32, 4) = t4;
  t5 = FIELD_AT(v_r7, s32, 16);
  FIELD_AT(sp_, s32, 8) = ((t5 & v_r1) + v_r2);
  FIELD_AT(v_r0, s32, 8) = ((t5 & v_r1) + v_r2);
  t6 = FIELD_AT(sp_, s32, 12);
  FIELD_AT(sp_, s32, 16) = ((t5 & v_r1) + v_r2);
  FIELD_AT(sp_, s32, 20) = t6;
  FIELD_AT(sp_, s32, 4) = (v_r7 + 34);
  t7 = *(u8 *)(v_r7 + 34);
  t8 = FIELD_AT(sp_, s32, 16);
  t9 = Value4((s32 (*)())Func_02004224, t7, t6, t8, (v_r7 + 34));
  FIELD_AT(sp_, s32, 24) = t9;
  t10 = FIELD_AT(sp_, s32, 20);
  t11 = FIELD_AT(v_r7, s32, 8);
  t12 = FIELD_AT(sp_, s32, 16);
  t13 = FIELD_AT(v_r7, s32, 16);
  v_r6 = (t12 - t13);
  v_r5 = ((t10 - t11) + v_r6);
  if (v_r5 >= 0x40000)
    goto L_0200b158;
  t14 = *(s32 *)(s32)gInput;
  t15 = *(s16 *)((s32)gData_0200483c + ((((u32)t14 >> 4) & 15) << 1));
  v_r9 = t15;
  if ((t15 << 16) != 0xffff0000)
    goto L_0200b184;
  goto L_0200b34e;
L_0200b158:
  t16 = FIELD_AT(sp_, s32, 16);
  t17 = FIELD_AT(sp_, s32, 20);
  t18 = FIELD_AT(v_r7, s32, 16);
  t19 = FIELD_AT(v_r7, s32, 8);
  t20 = Value2((s32 (*)())ArcTan2, (t16 - t18), (t17 - t19));
  v_r9 = ((t20 << 16) >> 16);
  v_r9 = ((((0x2000 + v_r9) & -0x4000) << 16) >> 16);
  goto L_0200b184;
L_0200b184:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, v_r9, ((s32)sp_ + 28));
  t22 = FIELD_AT(sp_, s32, 4);
  t23 = *(s32 *)v_r10;
  t24 = *(u8 *)t22;
  t25 = FIELD_AT(v_r10, s32, 8);
  t26 = Value4((s32 (*)())Func_02004224, t24, t23, t25, t22);
  if (t26 == 255)
    goto L_0200b1fe;
  t27 = FIELD_AT(sp_, s32, 4);
  t28 = *(u8 *)t27;
  t29 = *(s32 *)v_r10;
  t30 = FIELD_AT(v_r10, s32, 8);
  t31 = Value3((s32 (*)())Map_GetTerrainHeight, t28, t29, t30);
  t32 = FIELD_AT(v_r7, s32, 12);
  if ((t31 - t32) > 0x80000)
    goto L_0200b1fe;
  t33 = FIELD_AT(sp_, s32, 12);
  *(s32 *)v_r10 = t33;
  t34 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r10, s32, 8) = t34;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x1999;
  *(u16 *)(v_r7 + 100) = 0;
  t35 = FIELD_AT(sp_, s32, 12);
  t36 = FIELD_AT(v_r7, s32, 12);
  t37 = FIELD_AT(sp_, s32, 8);
  Call4((void (*)())Func_02004204, v_r7, t35, t36, t37);
  Call2((void (*)())Func_020041cc, v_r7, 2);
  Call2((void (*)())ObjectDispatch_SetAnimationSpeed, v_r7, 48);
  v_r5 = 0;
  goto L_0200b226;
L_0200b1fe:
  FIELD_AT(v_r7, u16, 6) = v_r9;
  FIELD_AT(v_r7, s32, 36) = 0;
  FIELD_AT(v_r7, s32, 44) = 0;
  t41 = FIELD_AT(sp_, s32, 12);
  FIELD_AT(v_r7, s32, 8) = t41;
  t42 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r7, s32, 16) = t42;
  goto L_0200b34e;
L_0200b212:
  t43 = *(u16 *)(s32)gData_02004cf0;
  if ((t43 << 16) == 0)
    goto L_0200b21e;
  goto L_0200b34e;
L_0200b21e:
  Call1((void (*)())WaitFrames, 1);
  v_r5 = (v_r5 + 1);
L_0200b226:
  if (v_r5 > 179)
    goto L_0200b234;
  t45 = Value1((s32 (*)())Func_02004284, v_r7);
  if (t45 == 0)
    goto L_0200b212;
L_0200b234:
  FIELD_AT(v_r7, s32, 108) = (s32)Func_02002d04;
  goto L_0200b302;
L_0200b23a:
  t46 = FIELD_AT(sp_, s32, 4);
  t47 = *(s32 *)v_r6;
  t48 = *(u8 *)t46;
  t49 = FIELD_AT(v_r6, s32, 8);
  t50 = Value3((s32 (*)())Map_GetTerrainHeight, t48, t47, t49);
  t51 = FIELD_AT(v_r7, s32, 12);
  if ((t50 - t51) > 0x80000)
    goto L_0200b322;
  t52 = *(u16 *)(s32)gData_02004cf0;
  if ((t52 << 16) != 0)
    goto L_0200b34e;
  t53 = FIELD_AT(v_r7, u16, 32);
  FIELD_AT(sp_, s32, 0) = (t53 - 2);
  t54 = FIELD_AT(0x3000000, s32, 20);
  v_r8 = (89 + t54);
  v_r10 = 0;
  v_r5 = t54;
L_0200b272:
  t55 = *(s32 *)v_r5;
  if (t55 == 0)
    goto L_0200b29c;
  t56 = *(u8 *)v_r8;
  if ((1 & t56) == 0)
    goto L_0200b29c;
  if (v_r5 == v_r7)
    goto L_0200b29c;
  t57 = FIELD_AT(v_r5, u16, 32);
  t58 = FIELD_AT(sp_, s32, 0);
  t59 = Value4((s32 (*)())Func_0200428c, (v_r5 + 8), t58, ((s32)sp_ + 28),
               (t57 - 2));
  if (t59 >= 0)
    goto L_0200b322;
L_0200b29c:
  v_r10 = (v_r10 + 1);
  v_r8 = (v_r8 + 128);
  v_r5 = (v_r5 + 128);
  if (v_r10 <= 63)
    goto L_0200b272;
  t60 = *(s32 *)v_r6;
  FIELD_AT(sp_, s32, 20) = t60;
  t61 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(sp_, s32, 16) = t61;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x1999;
  t62 = *(s32 *)v_r6;
  t63 = FIELD_AT(v_r6, s32, 4);
  t64 = FIELD_AT(v_r6, s32, 8);
  Call4((void (*)())Func_02004204, v_r7, t62, t63, t64);
  v_r5 = 0;
  goto L_0200b2da;
L_0200b2d2:
  Call1((void (*)())WaitFrames, 1);
  v_r5 = (v_r5 + 1);
L_0200b2da:
  if (v_r5 > 179)
    goto L_0200b2f2;
  t67 = Value1((s32 (*)())Func_02004284, v_r7);
  if (t67 != 0)
    goto L_0200b2f2;
  t68 = *(u16 *)(s32)gData_02004cf0;
  if ((t68 << 16) == 0)
    goto L_0200b2d2;
L_0200b2f2:
  t69 = *(u16 *)(s32)gData_02004cf0;
  if ((t69 << 16) != 0)
    goto L_0200b34e;
  t70 = FIELD_AT(sp_, s32, 24);
  if (v_r11 != t70)
    goto L_0200b348;
L_0200b302:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, v_r9, ((s32)sp_ + 28));
  t72 = FIELD_AT(sp_, s32, 4);
  t73 = *(u8 *)t72;
  t74 = FIELD_AT(sp_, s32, 28);
  t75 = FIELD_AT(sp_, s32, 36);
  t76 = Value4((s32 (*)())Func_02004224, t73, t74, t75, 0);
  v_r11 = t76;
  v_r6 = ((s32)sp_ + 28);
  if (v_r11 != 255)
    goto L_0200b23a;
L_0200b322:
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x10000;
  t77 = FIELD_AT(v_r7, s32, 12);
  t78 = FIELD_AT(sp_, s32, 20);
  t79 = FIELD_AT(sp_, s32, 16);
  Call4((void (*)())Func_02004204, v_r7, t78, t77, t79);
  Call1((void (*)())Func_0200420c, v_r7);
  Call1((void (*)())WaitFrames, 2);
  goto L_0200b096;
L_0200b348:
  Call1((void (*)())WaitFrames, 10);
L_0200b34e:
  FIELD_AT(v_r7, s32, 108) = 0;
  t84 = *(u8 *)(v_r7 + 90);
  *(u8 *)(v_r7 + 90) = (1 | t84);
  FIELD_AT(v_r7, s32, 52) = 0x4000;
  Call2((void (*)())Func_020041cc, v_r7, 1);
  return;
}
