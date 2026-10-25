// Draft of overlay 6aa's Func_02000e3c, translated from its listing: differ
// 157 of 143. Left: its shape: 136 instructions against the listing's 143.
#include "types.h"
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
s32 ObjectDispatch_SetAnimationSpeed();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Object_GetById();
s32 Object_SetModeById();
s32 WaitFrames();
extern u8 gMapCellBuffer[];
extern u8 gPartyState[];

void Func_02000e3c(s32 p0) {
  s32 v_r5, v_r0, v_r4, v_r2, v_r6, v_r7, v_r1, t0, t1, t4;
  s32 t5, t7, t12, t13, t14, t15, t16, t17, t18, t20;
  s32 t24, t25, t26, t28, t29, t30;

  v_r5 = ((s32)gPartyState + 0x214);
  t0 = *(s32 *)v_r5;
  t1 = Object_GetById(t0);
  Func_02004ca8();
  Func_02004da0(0, 0, 0);
  t4 = *(s32 *)v_r5;
  t5 = Object_GetById(t4);
  ObjectDispatch_SetSingleChildFlags(t5, 0);
  FIELD_AT(t1, s32, 108) = 0;
  t7 = *(s32 *)v_r5;
  Func_02004d00(t7, 0x100, 0);
  Func_02004bc0(t1, 27);
  ObjectDispatch_SetAnimationSpeed(t1, 48);
  WaitFrames(30);
  FIELD_AT(t1, u16, 6) = 0x4000;
  *(u8 *)(t1 + 85) = 0;
  t12 = FIELD_AT(t1, s32, 8);
  t13 = FIELD_AT(t1, s32, 16);
  t14 = FIELD_AT(0x3000000, s32, 32);
  v_r1 = t14;
  v_r7 = p0;
  v_r6 = t1;
  v_r2 = (s32)gMapCellBuffer;
  v_r4 = (t12 >> 20);
  v_r0 = (t13 >> 20);
  if (v_r1 == 0)
    goto L_02008ed4;
  t15 = *(u8 *)(v_r6 + 34);
  t16 = *(s32 *)(v_r1 + (((((3 & t15) << 3) - (3 & t15)) << 3) + 0x138));
  v_r2 = t16;
  goto L_02008ed4;
L_02008ed4:
  t17 = FIELD_AT(((v_r2 + ((v_r4 + (v_r0 << 7)) << 2)) + 0x200), u8, 3);
  if ((64 & t17) != 0)
    goto L_02008f0a;
  *(u16 *)0x400004a = 0xf1f;
  t18 = *(u16 *)0x4000000;
  *(u16 *)0x4000000 = (t18 | 0x8000);
  Func_02000df0();
L_02008f0a:
  t20 = *(s32 *)((s32)gPartyState + 0x214);
  Object_SetModeById(t20, 28);
  FieldEvent_WaitFrames(16);
  v_r5 = 0;
L_02008f22:
  if (v_r5 != 5)
    goto L_02008f2c;
  Func_02004de0(204);
L_02008f2c:
  t24 = FIELD_AT(v_r6, s32, 24);
  FIELD_AT(v_r6, s32, 24) = (t24 + -0x400);
  v_r2 = -0x300;
  goto L_02008f48;
L_02008f48:
  t25 = FIELD_AT(v_r6, s32, 28);
  FIELD_AT(v_r6, s32, 28) = (t25 + v_r2);
  t26 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(v_r6, s32, 12) = (t26 + -0x9999);
  WaitFrames(1);
  v_r5 = (v_r5 + 1);
  if (v_r5 <= 39)
    goto L_02008f22;
  t28 = *(s32 *)((s32)gPartyState + 0x214);
  t29 = Object_GetById(t28);
  *(u8 *)(t29 + 84) = 0;
  t30 = FIELD_AT(0x3000000, s32, 108);
  *(s32 *)(t30 + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004d30(v_r7);
  return;
}
