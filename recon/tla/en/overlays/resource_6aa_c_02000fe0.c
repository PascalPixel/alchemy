// Draft of overlay 6aa's Func_02000fe0, translated from its listing: differ
// 50 of 130. Left: its shape: 124 instructions against the listing's 130.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Event_FinishScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_WaitFrames();
s32 Func_02000df0();
s32 Func_02004bc0();
s32 Func_02004ca8();
s32 Func_02004d00();
s32 Func_02004d30();
s32 Func_02004da0();
s32 Func_02004de0();
s32 GameFlag_SetByte();
s32 ObjectDispatch_SetAnimationSpeed();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Object_GetById();
s32 Object_SetModeById();
s32 WaitFrames();
extern u8 gPartyState[];

void Func_02000fe0(s32 p0) {
  s32 v_r7, v_r5, v_r8, v_r6, t0, t1, t2, t3, t8, t9;
  s32 t11, t16, t17, t19, t23, t24, t25, t27, t28, t29;

  t0 = *(s32 *)(0x214 + (s32)gPartyState);
  t1 = Value1((s32 (*)())Object_GetById, t0);
  t2 = FIELD_AT(t1, s32, 8);
  t3 = FIELD_AT(t1, s32, 16);
  v_r7 = (t2 >> 20);
  Call2((void (*)())GameFlag_SetByte, 0x340, v_r7);
  v_r5 = (t3 >> 20);
  Call2((void (*)())GameFlag_SetByte, 0x348, v_r5);
  Func_02004ca8();
  Call3((void (*)())Func_02004da0, 0, 0, 0);
  v_r8 = (0x214 + (s32)gPartyState);
  t8 = *(s32 *)v_r8;
  t9 = Value1((s32 (*)())Object_GetById, t8);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t9, 0);
  FIELD_AT(t1, s32, 108) = 0;
  t11 = *(s32 *)v_r8;
  Call3((void (*)())Func_02004d00, t11, 0x100, 0);
  Call2((void (*)())Func_02004bc0, t1, 27);
  Call2((void (*)())ObjectDispatch_SetAnimationSpeed, t1, 48);
  Call1((void (*)())WaitFrames, 30);
  FIELD_AT(t1, u16, 6) = 0x4000;
  *(u8 *)(t1 + 85) = 0;
  t16 = FIELD_AT(p0, u8, 3);
  v_r6 = t1;
  if ((64 & t16) != 0)
    goto L_020090a0;
  *(u16 *)0x400004a = 0xf1f;
  t17 = *(u16 *)0x4000000;
  *(u16 *)0x4000000 = (t17 | 0x8000);
  goto L_0200909c;
L_0200909c:
  Func_02000df0();
L_020090a0:
  t19 = *(s32 *)v_r8;
  Call2((void (*)())Object_SetModeById, t19, 28);
  Call1((void (*)())FieldEvent_WaitFrames, 16);
  v_r7 = 0;
L_020090b2:
  if (v_r7 != 5)
    goto L_020090bc;
  Call1((void (*)())Func_02004de0, 204);
L_020090bc:
  t23 = FIELD_AT(v_r6, s32, 24);
  FIELD_AT(v_r6, s32, 24) = (t23 + -0x400);
  t24 = FIELD_AT(v_r6, s32, 28);
  FIELD_AT(v_r6, s32, 28) = (t24 + -0x300);
  t25 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(v_r6, s32, 12) = (t25 + -0x9999);
  Call1((void (*)())WaitFrames, 1);
  v_r7 = (v_r7 + 1);
  if (v_r7 <= 39)
    goto L_020090b2;
  t27 = *(s32 *)((s32)gPartyState + 0x214);
  t28 = Value1((s32 (*)())Object_GetById, t27);
  *(u8 *)(t28 + 84) = 0;
  t29 = FIELD_AT(0x3000000, s32, 108);
  *(s32 *)(t29 + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())Func_02004d30, 11);
  return;
}
