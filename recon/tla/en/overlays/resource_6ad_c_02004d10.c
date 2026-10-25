// Draft of overlay 6ad's Func_02004d10, translated from its listing: differ
// 380 of 305. Left: its shape: 299 instructions against the listing's 305.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02004a44();
s32 Func_02005480();
s32 Func_020054b8();
s32 Func_020054c0();
s32 Func_020054e8();
s32 Func_02005668();
s32 Func_020056c0();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 ObjectMotion_OffsetPositionAndResetMotion();
s32 ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
s32 Object_SetModeById();
s32 Object_SetPositionAndResetMotion();
s32 WaitFrames();
extern u8 Func_02004fc0[];
extern u8 gData_02005f64[];
extern u8 gData_02005f7c[];
extern u8 gPartyState[];

void Func_02004d10(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4, s32 p5) {
  s32 sp_[18];
  s32 v_r7, v_r5, v_r0, v_r4, v_r14, v_r12, v_r10, v_r6, v_r9, v_r8;
  s32 v_r1, v_r2, v_r11, v_r3, t0, t1, t2, t3, t4, t5;
  s32 t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;
  s32 t16, t17, t19, t21, t24, t25, t26, t27, t28, t29;
  s32 t31, t32, t37, t38, t39, t41, t43, t45, t47, t48;
  s32 t50, t52, t54, t55, t59, t60, t61, t64, t65, t66;
  s32 t67, t68, t69, t70, t71, t73, t74, t75, t77, t78;
  s32 t79, t81, t82, t83, t84, t85, t86, t87, t88, t90;
  s32 t91, t92, t93;

  FIELD_AT(sp_, s32, 88) = p0;
  FIELD_AT(sp_, s32, 92) = p1;
  FIELD_AT(sp_, s32, 96) = p2;
  FIELD_AT(sp_, s32, 100) = p3;
  t0 = FIELD_AT(0x3000000, s32, 32);
  FIELD_AT(sp_, s32, 28) = t0;
  t1 = *(s32 *)(0x214 + (s32)gPartyState);
  t2 = Value1((s32 (*)())Object_GetById, t1);
  t3 = Value1((s32 (*)())Object_GetById, p4);
  t4 = FIELD_AT(t2, s32, 48);
  FIELD_AT(sp_, s32, 20) = t4;
  t5 = FIELD_AT(t2, s32, 52);
  FIELD_AT(sp_, s32, 12) = ((s32)sp_ + 32);
  FIELD_AT(sp_, s32, 16) = t5;
  t6 = FIELD_AT(sp_, s32, 100);
  t7 = FIELD_AT(t3, s32, 8);
  FIELD_AT(sp_, s32, 32) = t7;
  t8 = FIELD_AT(t3, s32, 16);
  FIELD_AT(sp_, s32, 40) = t8;
  FIELD_AT(sp_, s32, 8) = ((s32)sp_ + 44);
  v_r7 = (t6 << 2);
  t9 = *(s8 *)((s32)gData_02005f64 + v_r7);
  t10 = FIELD_AT(t3, s32, 8);
  t11 = FIELD_AT(sp_, s32, 8);
  *(s32 *)t11 = ((t10 + (t9 << 16)) >> 20);
  t12 = *(s8 *)((s32)gData_02005f64 + (v_r7 + 1));
  t13 = FIELD_AT(t3, s32, 16);
  t14 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(t14, s32, 8) = ((t13 + (t12 << 16)) >> 20);
  v_r8 = t2;
  v_r9 = 0;
  v_r6 = t3;
  v_r10 = (0x214 + (s32)gPartyState);
  v_r12 = ((t13 + (t12 << 16)) >> 20);
  v_r14 = ((t10 + (t9 << 16)) >> 20);
  v_r4 = t12;
  v_r0 = t9;
  v_r5 = (s32)gData_02005f64;
  if (v_r0 >= v_r9)
    goto L_0200cda0;
  v_r0 = -v_r0;
L_0200cda0:
  t15 = *(s8 *)(v_r5 + (v_r7 + 2));
  v_r1 = t15;
  if (v_r1 >= 0)
    goto L_0200cdaa;
  v_r1 = -v_r1;
L_0200cdaa:
  FIELD_AT(sp_, s32, 24) = ((v_r0 + v_r1) >> 4);
  v_r1 = v_r4;
  if (v_r1 >= 0)
    goto L_0200cdb8;
  v_r1 = -v_r1;
L_0200cdb8:
  t16 = *(s8 *)(v_r5 + (v_r7 + 3));
  v_r2 = t16;
  if (v_r2 >= 0)
    goto L_0200cdc2;
  v_r2 = -v_r2;
L_0200cdc2:
  t17 = FIELD_AT(sp_, s32, 24);
  Call6((void (*)())Func_02004a44, 0, v_r14, v_r12, t17, t17, t17);
  t19 = *(s32 *)v_r10;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, t19, 0x8000, 0x1999);
  t21 = *(s32 *)v_r10;
  Call2((void (*)())Object_SetModeById, t21, 8);
  Call1((void (*)())WaitFrames, 15);
  t24 = FIELD_AT(sp_, s32, 12);
  t25 = FIELD_AT(sp_, s32, 88);
  t26 = *(s32 *)t24;
  t27 = FIELD_AT(sp_, s32, 96);
  t28 = FIELD_AT(t24, s32, 8);
  t29 = *(s32 *)v_r10;
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, t29,
        ((t25 - t26) >> 17), ((t27 - t28) >> 17));
  t31 = *(s32 *)v_r10;
  t32 = Value1((s32 (*)())Object_GetById, t31);
  FIELD_AT(t32, s32, 108) = (s32)Func_02004fc0;
  Call1((void (*)())WaitFrames, 4);
  Call2((void (*)())Func_02005480, v_r6, 2);
  Call1((void (*)())Func_020056c0, 239);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, p4, 0x8000, 0x1999);
  t37 = FIELD_AT(sp_, s32, 88);
  t38 = FIELD_AT(sp_, s32, 92);
  t39 = FIELD_AT(sp_, s32, 96);
  Call4((void (*)())Func_020054b8, v_r6, t37, t38, t39);
  v_r5 = ((s32)gPartyState + 0x214);
  t41 = *(s32 *)v_r5;
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, t41);
  t43 = *(s32 *)v_r5;
  Call2((void (*)())Object_SetModeById, t43, 2);
  t45 = *(s32 *)v_r5;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, t45, 0x4ccc, 0x1999);
  t47 = *(s32 *)((s32)gData_02005f7c + (v_r9 << 2));
  t48 = *(s32 *)v_r5;
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, t48,
        ((t47 << 16) >> 31), ((t47 << 16) >> 17));
  v_r3 = p5;
  v_r11 = ((v_r1 + v_r2) >> 4);
  if (v_r3 == 0)
    goto L_0200ce98;
L_0200ce98:
  t50 = *(s32 *)v_r5;
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, t50);
  t52 = *(s32 *)v_r5;
  Call2((void (*)())Object_SetModeById, t52, 1);
  FIELD_AT(v_r8, s32, 108) = 0;
  t54 = FIELD_AT(sp_, s32, 20);
  FIELD_AT(v_r8, s32, 48) = t54;
  t55 = FIELD_AT(sp_, s32, 16);
  FIELD_AT(v_r8, s32, 52) = t55;
  Call1((void (*)())Func_020054c0, v_r6);
  Call1((void (*)())Func_020056c0, 0x12a);
  Call1((void (*)())Func_020056c0, 213);
  t59 = FIELD_AT(v_r6, s32, 12);
  t60 = FIELD_AT(sp_, s32, 88);
  t61 = FIELD_AT(sp_, s32, 96);
  Call4((void (*)())Object_SetPositionAndResetMotion, v_r6, t60, t59, t61);
  Call2((void (*)())Func_02005480, v_r6, 1);
  t64 = FIELD_AT(sp_, s32, 88);
  t65 = *(s8 *)((s32)gData_02005f64 + v_r7);
  t66 = *(s8 *)((s32)gData_02005f64 + (v_r7 + 1));
  t67 = FIELD_AT(sp_, s32, 96);
  t68 = FIELD_AT(sp_, s32, 28);
  FIELD_AT(sp_, s32, 88) = ((t64 + (t65 << 16)) >> 20);
  FIELD_AT(sp_, s32, 96) = ((t67 + (t66 << 16)) >> 20);
  t69 = *(s32 *)(t68 + 0x148);
  t70 = *(s32 *)(t68 + 0x14c);
  v_r8 = (t69 >> 20);
  v_r6 = (t70 >> 20);
  t71 = FIELD_AT(sp_, s32, 24);
  Call6((void (*)())Func_020054e8, ((t64 + (t65 << 16)) >> 20),
        ((t67 + (t66 << 16)) >> 20), t71, v_r11, v_r11, t71);
  t73 = FIELD_AT(sp_, s32, 88);
  t74 = FIELD_AT(sp_, s32, 96);
  t75 = FIELD_AT(sp_, s32, 24);
  Call6((void (*)())Func_02004a44, 0, t73, t74, t75, 0, 255);
  t77 = FIELD_AT(sp_, s32, 88);
  t78 = FIELD_AT(sp_, s32, 96);
  t79 = FIELD_AT(sp_, s32, 24);
  Call6((void (*)())Func_02004a44, 2, t77, t78, t79, t79, 255);
  t81 = FIELD_AT(sp_, s32, 12);
  t82 = *(s8 *)((s32)gData_02005f64 + v_r7);
  t83 = *(s32 *)t81;
  t84 = FIELD_AT(sp_, s32, 8);
  *(s32 *)t84 = ((t83 + (t82 << 16)) >> 20);
  v_r9 = (v_r7 + 1);
  t85 = *(s8 *)((s32)gData_02005f64 + v_r9);
  t86 = FIELD_AT(t81, s32, 8);
  t87 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(t87, s32, 8) = ((t86 + (t85 << 16)) >> 20);
  t88 = FIELD_AT(sp_, s32, 24);
  v_r8 = (v_r8 + ((t83 + (t82 << 16)) >> 20));
  v_r6 = (v_r6 + ((t86 + (t85 << 16)) >> 20));
  Call6((void (*)())Func_020054e8, v_r8, v_r6, t88, v_r11, v_r6, v_r11);
  t90 = FIELD_AT(sp_, s32, 8);
  t91 = *(s32 *)t90;
  t92 = FIELD_AT(t90, s32, 8);
  t93 = FIELD_AT(sp_, s32, 24);
  Call6((void (*)())Func_02004a44, 2, t91, t92, t93, t93, 0);
  Func_02005668();
  return;
}
