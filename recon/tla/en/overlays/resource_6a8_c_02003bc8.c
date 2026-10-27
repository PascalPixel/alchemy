// Draft of overlay 6a8's Func_02003bc8, translated from its listing: differ
// 355 of 347. Left: its shape: 368 instructions against the listing's 347.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 ArcTan2();
s32 Func_020038e8();
s32 Func_020043c4();
s32 Func_020043f4();
s32 Func_020043fc();
s32 Func_02004424();
s32 Func_0200444c();
s32 Func_02004454();
s32 Map_GetTerrainHeight();
s32 ObjectDispatch_SetAnimationSpeed();
s32 ObjectTable_Get();
s32 Vector_AddPolarOffsetFar();
s32 WaitFrames();
extern u8 Func_0200385c[];
extern u8 IwramMulQ16[];
extern u8 gData_02004780[];
extern u8 gData_020062a4[];
extern u8 gInput[];
extern u8 gPartyState[];

void Func_02003bc8(void) {
  s32 sp_[1];
  s32 v_r7, v_r0, v_r1, v_r2, v_r10, v_r6, v_r5, v_r9, v_r11, v_r8;
  s32 t0, t1, t3, t4, t5, t6, t7, t8, t9, t10;
  s32 t11, t12, t13, t14, t15, t16, t17, t18, t19, t20;
  s32 t21, t22, t24, t25, t26, t27, t28, t29, t30, t31;
  s32 t32, t33, t34, t35, t36, t37, t38, t39, t43, t44;
  s32 t45, t47, t48, t49, t50, t51, t52, t53, t54, t55;
  s32 t56, t57, t58, t59, t60, t61, t62, t63, t64, t65;
  s32 t66, t69, t70, t71, t72, t74, t75, t76, t77, t78;
  s32 t79, t80, t81, t86;

  t0 = FIELD_AT((s32)gPartyState, s32, 532);
  t1 = Value1((s32 (*)())ObjectTable_Get, t0);
  *(u16 *)(s32)gData_020062a4 = 0;
  v_r7 = t1;
L_0200bbee:
  Func_020038e8();
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
  goto L_0200bc34;
L_0200bc34:
  t4 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(v_r0, s32, 4) = t4;
  t5 = FIELD_AT(v_r7, s32, 16);
  FIELD_AT(sp_, s32, 8) = ((t5 & v_r1) + v_r2);
  FIELD_AT(v_r0, s32, 8) = ((t5 & v_r1) + v_r2);
  t6 = FIELD_AT(sp_, s32, 12);
  FIELD_AT(sp_, s32, 16) = ((t5 & v_r1) + v_r2);
  FIELD_AT(sp_, s32, 20) = t6;
  FIELD_AT(sp_, s32, 4) = (v_r7 + 34);
  t7 = FIELD_AT(v_r7, u8, 34);
  t8 = FIELD_AT(sp_, s32, 16);
  t9 = Value3((s32 (*)())Func_02004424, t7, t6, t8);
  FIELD_AT(sp_, s32, 24) = t9;
  t10 = FIELD_AT(sp_, s32, 20);
  t11 = FIELD_AT(v_r7, s32, 8);
  t12 = FIELD_AT(sp_, s32, 16);
  t13 = FIELD_AT(v_r7, s32, 16);
  t14 = ((s32 (*)())(s32)IwramMulQ16)((t10 - t11));
  v_r6 = (t12 - t13);
  t15 = ((s32 (*)())(s32)IwramMulQ16)(v_r6, v_r6);
  v_r5 = (t14 + t15);
  if (v_r5 >= 0x40000)
    goto L_0200bcb0;
  t16 = *(s32 *)(s32)gInput;
  t17 = *(s16 *)((s32)gData_02004780 + ((((u32)t16 >> 4) & 15) << 1));
  v_r9 = t17;
  if ((t17 << 16) != 0xffff0000)
    goto L_0200bcdc;
  goto L_0200bea6;
L_0200bcb0:
  t18 = FIELD_AT(sp_, s32, 16);
  t19 = FIELD_AT(sp_, s32, 20);
  t20 = FIELD_AT(v_r7, s32, 16);
  t21 = FIELD_AT(v_r7, s32, 8);
  t22 = Value2((s32 (*)())ArcTan2, (t18 - t20), (t19 - t21));
  v_r9 = ((t22 << 16) >> 16);
  v_r9 = ((((v_r9 + 0x2000) & -0x4000) << 16) >> 16);
  goto L_0200bcdc;
L_0200bcdc:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, v_r9, ((s32)sp_ + 28));
  t24 = FIELD_AT(sp_, s32, 4);
  t25 = *(s32 *)v_r10;
  t26 = *(u8 *)t24;
  t27 = FIELD_AT(v_r10, s32, 8);
  t28 = Value3((s32 (*)())Func_02004424, t26, t25, t27);
  if (t28 == 255)
    goto L_0200bd56;
  t29 = FIELD_AT(sp_, s32, 4);
  t30 = *(u8 *)t29;
  t31 = *(s32 *)v_r10;
  t32 = FIELD_AT(v_r10, s32, 8);
  t33 = Value3((s32 (*)())Map_GetTerrainHeight, t30, t31, t32);
  t34 = FIELD_AT(v_r7, s32, 12);
  if ((t33 - t34) > 0x80000)
    goto L_0200bd56;
  t35 = FIELD_AT(sp_, s32, 12);
  *(s32 *)v_r10 = t35;
  t36 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r10, s32, 8) = t36;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x1999;
  FIELD_AT(v_r7, u16, 100) = 0;
  t37 = FIELD_AT(sp_, s32, 12);
  t38 = FIELD_AT(v_r7, s32, 12);
  t39 = FIELD_AT(sp_, s32, 8);
  Call4((void (*)())Func_020043f4, v_r7, t37, t38, t39);
  Call2((void (*)())Func_020043c4, v_r7, 2);
  Call2((void (*)())ObjectDispatch_SetAnimationSpeed, v_r7, 48);
  v_r5 = 0;
  goto L_0200bd7e;
L_0200bd56:
  FIELD_AT(v_r7, u16, 6) = v_r9;
  FIELD_AT(v_r7, s32, 36) = 0;
  FIELD_AT(v_r7, s32, 44) = 0;
  t43 = FIELD_AT(sp_, s32, 12);
  FIELD_AT(v_r7, s32, 8) = t43;
  t44 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r7, s32, 16) = t44;
  goto L_0200bea6;
L_0200bd6a:
  t45 = *(u16 *)(s32)gData_020062a4;
  if ((t45 << 16) == 0)
    goto L_0200bd76;
  goto L_0200bea6;
L_0200bd76:
  Call1((void (*)())WaitFrames, 1);
  v_r5 = (v_r5 + 1);
L_0200bd7e:
  if (v_r5 > 179)
    goto L_0200bd8c;
  t47 = Value1((s32 (*)())Func_0200444c, v_r7);
  if (t47 == 0)
    goto L_0200bd6a;
L_0200bd8c:
  FIELD_AT(v_r7, s32, 108) = (s32)Func_0200385c;
  goto L_0200be5a;
L_0200bd92:
  t48 = FIELD_AT(sp_, s32, 4);
  t49 = *(s32 *)v_r6;
  t50 = *(u8 *)t48;
  t51 = FIELD_AT(v_r6, s32, 8);
  t52 = Value3((s32 (*)())Map_GetTerrainHeight, t50, t49, t51);
  t53 = FIELD_AT(v_r7, s32, 12);
  if ((t52 - t53) > 0x80000)
    goto L_0200be7a;
  t54 = *(u16 *)(s32)gData_020062a4;
  if ((t54 << 16) != 0)
    goto L_0200bea6;
  t55 = FIELD_AT(v_r7, u16, 32);
  *(s32 *)sp_ = (t55 - 2);
  t56 = FIELD_AT(0x3000000, s32, 20);
  v_r8 = (t56 + 89);
  v_r10 = 0;
  v_r5 = t56;
L_0200bdca:
  t57 = *(s32 *)v_r5;
  if (t57 == 0)
    goto L_0200bdf4;
  t58 = *(u8 *)v_r8;
  if ((1 & t58) == 0)
    goto L_0200bdf4;
  if (v_r5 == v_r7)
    goto L_0200bdf4;
  t59 = FIELD_AT(v_r5, u16, 32);
  t60 = *(s32 *)sp_;
  t61 = Value4((s32 (*)())Func_02004454, (v_r5 + 8), t60, ((s32)sp_ + 28),
               (t59 - 2));
  if (t61 >= 0)
    goto L_0200be7a;
L_0200bdf4:
  v_r10 = (v_r10 + 1);
  v_r8 = (v_r8 + 128);
  v_r5 = (v_r5 + 128);
  if (v_r10 <= 63)
    goto L_0200bdca;
  t62 = *(s32 *)v_r6;
  FIELD_AT(sp_, s32, 20) = t62;
  t63 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(sp_, s32, 16) = t63;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x1999;
  t64 = *(s32 *)v_r6;
  t65 = FIELD_AT(v_r6, s32, 4);
  t66 = FIELD_AT(v_r6, s32, 8);
  Call4((void (*)())Func_020043f4, v_r7, t64, t65, t66);
  v_r5 = 0;
  goto L_0200be32;
L_0200be2a:
  Call1((void (*)())WaitFrames, 1);
  v_r5 = (v_r5 + 1);
L_0200be32:
  if (v_r5 > 179)
    goto L_0200be4a;
  t69 = Value1((s32 (*)())Func_0200444c, v_r7);
  if (t69 != 0)
    goto L_0200be4a;
  t70 = *(u16 *)(s32)gData_020062a4;
  if ((t70 << 16) == 0)
    goto L_0200be2a;
L_0200be4a:
  t71 = *(u16 *)(s32)gData_020062a4;
  if ((t71 << 16) != 0)
    goto L_0200bea6;
  t72 = FIELD_AT(sp_, s32, 24);
  if (v_r11 != t72)
    goto L_0200bea0;
L_0200be5a:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, v_r9, ((s32)sp_ + 28));
  t74 = FIELD_AT(sp_, s32, 4);
  t75 = *(u8 *)t74;
  t76 = FIELD_AT(sp_, s32, 28);
  t77 = FIELD_AT(sp_, s32, 36);
  t78 = Value3((s32 (*)())Func_02004424, t75, t76, t77);
  v_r11 = t78;
  v_r6 = ((s32)sp_ + 28);
  if (v_r11 != 255)
    goto L_0200bd92;
L_0200be7a:
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x10000;
  t79 = FIELD_AT(v_r7, s32, 12);
  t80 = FIELD_AT(sp_, s32, 20);
  t81 = FIELD_AT(sp_, s32, 16);
  Call4((void (*)())Func_020043f4, v_r7, t80, t79, t81);
  Call1((void (*)())Func_020043fc, v_r7);
  Call1((void (*)())WaitFrames, 2);
  goto L_0200bbee;
L_0200bea0:
  Call1((void (*)())WaitFrames, 10);
L_0200bea6:
  FIELD_AT(v_r7, s32, 108) = 0;
  t86 = FIELD_AT(v_r7, u8, 90);
  FIELD_AT(v_r7, u8, 90) = (1 | t86);
  FIELD_AT(v_r7, s32, 52) = 0x4000;
  Call2((void (*)())Func_020043c4, v_r7, 1);
  return;
}
