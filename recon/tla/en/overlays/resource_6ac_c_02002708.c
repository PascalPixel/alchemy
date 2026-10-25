// Draft of overlay 6ac's Func_02002708, translated from its listing: differ
// 57 of 138. Left: its shape: 129 instructions against the listing's 138.
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
s32 Func_02002368();
s32 Func_02003118();
s32 Func_02003168();
s32 Func_020031e0();
s32 Func_02003218();
s32 Func_02003240();
s32 Func_02003280();
s32 GameFlag_ClearBit();
s32 GameFlag_SetBit();
s32 Object_GetById();
s32 WaitFrames();
extern u8 Func_020024bc[];
extern u8 gData_020023c4[];
extern u8 gPartyState[];

void Func_02002708(void) {
  s32 v_r6, v_r5, v_r1, v_r7, v_r3, t0, t1, t2, t3, t8;
  s32 t9, t15, t16, t17, t18, t19, t20, t21, t22, t23;
  s32 t24, t25, t26, t27, t28, t29, t31, t32;

  t0 = FIELD_AT(0x3000000, s32, 108);
  v_r6 = (t0 + 0x1b2);
  t1 = *(u16 *)v_r6;
  v_r1 = 0x1b2;
  v_r5 = t0;
  if (t1 != 0)
    goto L_0200a734;
  t2 = *(s32 *)(v_r5 + 0x1ac);
  t3 = *(s32 *)(v_r5 + (v_r1 + 2));
  Call2((void (*)())Func_02003240, t2, t3);
  *(u16 *)v_r6 = 1;
L_0200a734:
  Call1((void (*)())GameFlag_ClearBit, 0x106);
  *(u16 *)(v_r5 + 0x166) = 0;
  *(u16 *)(v_r5 + 0x15a) = 0;
  *(u16 *)(v_r5 + 0x15e) = 0;
  *(u16 *)(v_r5 + 0x15c) = 0;
  *(u16 *)(v_r5 + 0x164) = 0;
  Call2((void (*)())Func_02002368, -10, 0);
  Call2((void (*)())Func_02003218, 0x70000, 0xe000);
  t8 = *(s32 *)((s32)gPartyState + 0x214);
  t9 = Value1((s32 (*)())Object_GetById, t8);
  Call1((void (*)())GameFlag_SetBit, 0x106);
  Func_020031e0();
  Call1((void (*)())Func_02003280, 0);
  Call1((void (*)())GameFlag_ClearBit, 0x106);
  FIELD_AT(t9, s32, 108) = (s32)Func_020024bc;
  FIELD_AT(t9, u16, 6) = 0x4000;
  Call2((void (*)())Func_02003118, t9, 49);
  v_r7 = ((s32)gData_020023c4 + 0x188);
  t15 = FIELD_AT(v_r7, s32, 108);
  t16 = *(s16 *)(t15 + 100);
  v_r6 = t9;
  if (t16 != 0)
    goto L_0200a820;
L_0200a7c0:
  t17 = FIELD_AT(v_r6, s32, 8);
  t18 = FIELD_AT(v_r6, s32, 16);
  t19 = Value3((s32 (*)())Func_02003168, 0, t17, t18);
  t20 = FIELD_AT(v_r7, s32, 108);
  v_r1 = t20;
  if (t19 == 7)
    goto L_0200a7e0;
  t21 = FIELD_AT(v_r1, s32, 112);
  t22 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(v_r6, s32, 8) = (t22 + t21);
  t23 = FIELD_AT(v_r6, s32, 16);
  t24 = FIELD_AT(v_r1, s32, 120);
  v_r3 = (t23 + t24);
  goto L_0200a808;
L_0200a7e0:
  t25 = FIELD_AT(v_r1, s32, 112);
  if (t25 == 0)
    goto L_0200a7f4;
  t26 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(v_r6, s32, 8) = ((t26 & 0xfff00000) + 0x80000);
L_0200a7f4:
  t27 = FIELD_AT(v_r7, s32, 108);
  t28 = FIELD_AT(t27, s32, 120);
  if (t28 == 0)
    goto L_0200a80a;
  t29 = FIELD_AT(v_r6, s32, 16);
  v_r3 = ((t29 & 0xfff00000) + 0x80000);
L_0200a808:
  FIELD_AT(v_r6, s32, 16) = v_r3;
L_0200a80a:
  FIELD_AT(v_r7, u16, 4) = 0;
  Call1((void (*)())WaitFrames, 1);
  t31 = FIELD_AT(v_r7, s32, 108);
  t32 = *(s16 *)(t31 + 100);
  if (t32 == 0)
    goto L_0200a7c0;
L_0200a820:
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  FIELD_AT(v_r6, s32, 108) = 0;
  Call2((void (*)())Animation_ApplyChildValues, v_r6, 0);
  FIELD_AT(v_r7, u16, 4) = 0;
  return;
}
