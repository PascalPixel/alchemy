// Draft of overlay 6aa's Func_02002860, translated from its listing: differ
// 31 of 138. Left: its shape: 137 instructions against the listing's 138.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 FieldEvent_WaitFrames();
s32 Func_020024c0();
s32 Func_02004bc0();
s32 Func_02004c18();
s32 Func_02004ca8();
s32 Func_02004d10();
s32 Func_02004d50();
s32 Func_02004da0();
s32 GameFlag_ClearBit();
s32 GameFlag_SetBit();
s32 Object_GetById();
s32 WaitFrames();
extern u8 Func_02002614[];
extern u8 gData_020023c4[];
extern u8 gPartyState[];

void Func_02002860(void) {
  s32 v_r6, v_r5, v_r1, v_r7, v_r3, t0, t1, t2, t3, t8;
  s32 t9, t15, t16, t17, t18, t19, t20, t21, t22, t23;
  s32 t24, t25, t26, t27, t28, t29, t31, t32;

  t0 = FIELD_AT(0x3000000, s32, 108);
  v_r6 = (t0 + 0x1b2);
  t1 = *(u16 *)v_r6;
  v_r1 = 0x1b2;
  v_r5 = t0;
  if (t1 != 0)
    goto L_0200a88c;
  t2 = FIELD_AT(v_r5, s32, 428);
  t3 = *(s32 *)(v_r5 + (v_r1 + 2));
  Call2((void (*)())Func_02004d50, t2, t3);
  *(u16 *)v_r6 = 1;
L_0200a88c:
  Call1((void (*)())GameFlag_ClearBit, 0x106);
  FIELD_AT(v_r5, u16, 358) = 0;
  FIELD_AT(v_r5, u16, 346) = 0;
  FIELD_AT(v_r5, u16, 350) = 0;
  FIELD_AT(v_r5, u16, 348) = 0;
  FIELD_AT(v_r5, u16, 356) = 0;
  Func_020024c0(-10, 0);
  Call2((void (*)())Func_02004d10, 0x70000, 0xe000);
  t8 = FIELD_AT((s32)gPartyState, s32, 532);
  t9 = Value1((s32 (*)())Object_GetById, t8);
  Call1((void (*)())GameFlag_SetBit, 0x106);
  Func_02004ca8();
  Call1((void (*)())Func_02004da0, 0);
  Call1((void (*)())GameFlag_ClearBit, 0x106);
  FIELD_AT(t9, s32, 108) = (s32)Func_02002614;
  FIELD_AT(t9, u16, 6) = 0x4000;
  Call2((void (*)())Func_02004bc0, t9, 49);
  v_r7 = ((s32)gData_020023c4 + 0x188);
  t15 = FIELD_AT(v_r7, s32, 108);
  t16 = FIELD_AT(t15, s16, 100);
  v_r6 = t9;
  if (t16 != 0)
    goto L_0200a978;
L_0200a918:
  t17 = FIELD_AT(v_r6, s32, 8);
  t18 = FIELD_AT(v_r6, s32, 16);
  t19 = Value3((s32 (*)())Func_02004c18, 0, t17, t18);
  t20 = FIELD_AT(v_r7, s32, 108);
  v_r1 = t20;
  if (t19 == 7)
    goto L_0200a938;
  t21 = FIELD_AT(v_r1, s32, 112);
  t22 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(v_r6, s32, 8) = (t22 + t21);
  t23 = FIELD_AT(v_r6, s32, 16);
  t24 = FIELD_AT(v_r1, s32, 120);
  v_r3 = (t23 + t24);
  goto L_0200a960;
L_0200a938:
  t25 = FIELD_AT(v_r1, s32, 112);
  if (t25 == 0)
    goto L_0200a94c;
  t26 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(v_r6, s32, 8) = ((t26 & 0xfff00000) + 0x80000);
L_0200a94c:
  t27 = FIELD_AT(v_r7, s32, 108);
  t28 = FIELD_AT(t27, s32, 120);
  if (t28 == 0)
    goto L_0200a962;
  t29 = FIELD_AT(v_r6, s32, 16);
  v_r3 = ((t29 & 0xfff00000) + 0x80000);
L_0200a960:
  FIELD_AT(v_r6, s32, 16) = v_r3;
L_0200a962:
  FIELD_AT(v_r7, u16, 4) = 0;
  Call1((void (*)())WaitFrames, 1);
  t31 = FIELD_AT(v_r7, s32, 108);
  t32 = FIELD_AT(t31, s16, 100);
  if (t32 == 0)
    goto L_0200a918;
L_0200a978:
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  FIELD_AT(v_r6, s32, 108) = 0;
  Call2((void (*)())Animation_ApplyChildValues, v_r6, 0);
  FIELD_AT(v_r7, u16, 4) = 0;
  return;
}
