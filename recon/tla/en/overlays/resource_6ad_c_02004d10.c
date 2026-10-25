// Draft of overlay 6ad's Func_02004d10, translated from its listing: differ
// 350 of 305. Left: its shape: 313 instructions against the listing's 305.
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
  s32 sp_[12];
  s32 spo_[2];
  s32 v_r7, v_r5, v_r0, v_r4, v_r14, v_r12, v_r10, v_r6, v_r9, v_r8;
  s32 v_r1, v_r2, v_r11, v_r3, t0, t1, t2, t3, t4, t5;
  s32 t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;
  s32 t16, t18, t20, t23, t24, t25, t26, t28, t29, t35;
  s32 t37, t39, t41, t42, t45, t47, t49, t50, t54, t57;
  s32 t58, t59, t60, t61, t62, t64, t66, t68, t69, t70;
  s32 t71, t72, t73, t74, t75, t77, t78, t79, t80;

  t0 = FIELD_AT(0x3000000, s32, 32);
  FIELD_AT(sp_, s32, 20) = t0;
  t1 = FIELD_AT((s32)gPartyState, s32, 532);
  t2 = Value1((s32 (*)())Object_GetById, t1);
  t3 = Value1((s32 (*)())Object_GetById, p4);
  t4 = FIELD_AT(t2, s32, 48);
  FIELD_AT(sp_, s32, 12) = t4;
  t5 = FIELD_AT(t2, s32, 52);
  FIELD_AT(sp_, s32, 4) = ((s32)sp_ + 24);
  FIELD_AT(sp_, s32, 8) = t5;
  t6 = FIELD_AT(t3, s32, 8);
  FIELD_AT(sp_, s32, 24) = t6;
  t7 = FIELD_AT(t3, s32, 16);
  FIELD_AT(sp_, s32, 32) = t7;
  *(s32 *)sp_ = ((s32)sp_ + 36);
  v_r7 = (p3 << 2);
  t8 = *(s8 *)((s32)gData_02005f64 + v_r7);
  t9 = FIELD_AT(t3, s32, 8);
  t10 = *(s32 *)sp_;
  *(s32 *)t10 = ((t9 + (t8 << 16)) >> 20);
  t11 = *(s8 *)((s32)gData_02005f64 + (v_r7 + 1));
  t12 = FIELD_AT(t3, s32, 16);
  t13 = *(s32 *)sp_;
  FIELD_AT(t13, s32, 8) = ((t12 + (t11 << 16)) >> 20);
  v_r8 = t2;
  v_r9 = 0;
  v_r6 = t3;
  v_r10 = ((s32)gPartyState + 0x214);
  v_r12 = ((t12 + (t11 << 16)) >> 20);
  v_r14 = ((t9 + (t8 << 16)) >> 20);
  v_r4 = t11;
  v_r0 = t8;
  v_r5 = (s32)gData_02005f64;
  if (v_r0 >= v_r9)
    goto L_0200cda0;
  v_r0 = -v_r0;
L_0200cda0:
  t14 = *(s8 *)(v_r5 + (v_r7 + 2));
  v_r1 = t14;
  if (v_r1 >= 0)
    goto L_0200cdaa;
  v_r1 = -v_r1;
L_0200cdaa:
  FIELD_AT(sp_, s32, 16) = ((v_r0 + v_r1) >> 4);
  v_r1 = v_r4;
  if (v_r1 >= 0)
    goto L_0200cdb8;
  v_r1 = -v_r1;
L_0200cdb8:
  t15 = *(s8 *)(v_r5 + (v_r7 + 3));
  v_r2 = t15;
  if (v_r2 >= 0)
    goto L_0200cdc2;
  v_r2 = -v_r2;
L_0200cdc2:
  t16 = FIELD_AT(sp_, s32, 16);
  Call6((void (*)())Func_02004a44, 0, v_r14, v_r12, t16, ((v_r1 + v_r2) >> 4),
        0);
  t18 = *(s32 *)v_r10;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, t18, 0x8000, 0x1999);
  t20 = *(s32 *)v_r10;
  Call2((void (*)())Object_SetModeById, t20, 8);
  Call1((void (*)())WaitFrames, 15);
  t23 = FIELD_AT(sp_, s32, 4);
  t24 = *(s32 *)t23;
  t25 = FIELD_AT(t23, s32, 8);
  t26 = *(s32 *)v_r10;
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, t26,
        ((p0 - t24) >> 17), ((p2 - t25) >> 17));
  t28 = *(s32 *)v_r10;
  t29 = Value1((s32 (*)())Object_GetById, t28);
  FIELD_AT(t29, s32, 108) = (s32)Func_02004fc0;
  Call1((void (*)())WaitFrames, 4);
  Call2((void (*)())Func_02005480, v_r6, 2);
  Call1((void (*)())Func_020056c0, 239);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, p4, 0x8000, 0x1999);
  Call4((void (*)())Func_020054b8, v_r6, p0, p1, p2);
  v_r5 = ((s32)gPartyState + 0x214);
  t35 = *(s32 *)v_r5;
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, t35);
  t37 = *(s32 *)v_r5;
  Call2((void (*)())Object_SetModeById, t37, 2);
  t39 = *(s32 *)v_r5;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, t39, 0x4ccc, 0x1999);
  t41 = *(s32 *)((s32)gData_02005f7c + (v_r9 << 2));
  t42 = *(s32 *)v_r5;
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, t42,
        ((t41 << 16) >> 31), ((t41 << 16) >> 17));
  v_r3 = p5;
  v_r11 = ((v_r1 + v_r2) >> 4);
  if (v_r3 == 0)
    goto L_0200ce98;
  ((s32 (*)())v_r3)();
L_0200ce98:
  t45 = *(s32 *)v_r5;
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, t45);
  t47 = *(s32 *)v_r5;
  Call2((void (*)())Object_SetModeById, t47, 1);
  FIELD_AT(v_r8, s32, 108) = 0;
  t49 = FIELD_AT(sp_, s32, 12);
  FIELD_AT(v_r8, s32, 48) = t49;
  t50 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r8, s32, 52) = t50;
  Call1((void (*)())Func_020054c0, v_r6);
  Call1((void (*)())Func_020056c0, 0x12a);
  Call1((void (*)())Func_020056c0, 213);
  t54 = FIELD_AT(v_r6, s32, 12);
  Call4((void (*)())Object_SetPositionAndResetMotion, v_r6, p0, t54, p2);
  Call2((void (*)())Func_02005480, v_r6, 1);
  t57 = *(s8 *)((s32)gData_02005f64 + v_r7);
  t58 = *(s8 *)((s32)gData_02005f64 + (v_r7 + 1));
  t59 = FIELD_AT(sp_, s32, 20);
  p0 = ((p0 + (t57 << 16)) >> 20);
  p2 = ((p2 + (t58 << 16)) >> 20);
  t60 = FIELD_AT(t59, s32, 328);
  t61 = FIELD_AT(t59, s32, 332);
  v_r8 = (t60 >> 20);
  v_r6 = (t61 >> 20);
  t62 = FIELD_AT(sp_, s32, 16);
  Call6((void (*)())Func_020054e8, ((p0 + (t57 << 16)) >> 20),
        ((p2 + (t58 << 16)) >> 20), t62, v_r11,
        (v_r8 + ((p0 + (t57 << 16)) >> 20)),
        (v_r6 + ((p2 + (t58 << 16)) >> 20)));
  t64 = FIELD_AT(sp_, s32, 16);
  Call6((void (*)())Func_02004a44, 0, p0, p2, t64, v_r11, 255);
  t66 = FIELD_AT(sp_, s32, 16);
  Call6((void (*)())Func_02004a44, 2, p0, p2, t66, v_r11, 255);
  t68 = FIELD_AT(sp_, s32, 4);
  t69 = *(s8 *)((s32)gData_02005f64 + v_r7);
  t70 = *(s32 *)t68;
  t71 = *(s32 *)sp_;
  *(s32 *)t71 = ((t70 + (t69 << 16)) >> 20);
  v_r9 = (v_r7 + 1);
  t72 = *(s8 *)((s32)gData_02005f64 + v_r9);
  t73 = FIELD_AT(t68, s32, 8);
  t74 = *(s32 *)sp_;
  FIELD_AT(t74, s32, 8) = ((t73 + (t72 << 16)) >> 20);
  t75 = FIELD_AT(sp_, s32, 16);
  v_r8 = (v_r8 + ((t70 + (t69 << 16)) >> 20));
  v_r6 = (v_r6 + ((t73 + (t72 << 16)) >> 20));
  Call6((void (*)())Func_020054e8, v_r8, v_r6, t75, v_r11,
        ((t70 + (t69 << 16)) >> 20), ((t73 + (t72 << 16)) >> 20));
  t77 = *(s32 *)sp_;
  t78 = *(s32 *)t77;
  t79 = FIELD_AT(t77, s32, 8);
  t80 = FIELD_AT(sp_, s32, 16);
  Call6((void (*)())Func_02004a44, 2, t78, t79, t80, v_r11, 0);
  Func_02005668();
  return;
}
