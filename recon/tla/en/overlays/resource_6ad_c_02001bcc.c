// Draft of overlay 6ad's Func_02001bcc, translated from its listing: differ
// 10 of 165. Left: instruction order and register choice.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 Func_02005598();
s32 GameFlag_Test();
s32 Object_GetById();

void Func_02001bcc(void) {
  s32 v_r6, v_r7, v_r5, v_r9, v_r10, v_r11, v_r8, t0, t1, t2;
  s32 t3, t8, t9, t10, t11, t12, t13, t14, t15, t16;
  s32 t17, t19, t21, t22, t23, t24, t25, t26, t28, t30;
  s32 t31, t32, t33, t34;

  t0 = Value1((s32 (*)())Object_GetById, 9);
  t1 = Value1((s32 (*)())Object_GetById, 10);
  t2 = Value1((s32 (*)())Object_GetById, 23);
  t3 = FIELD_AT(t2, s32, 80);
  Call3((void (*)())Func_02005598, 24, 0x2000000, 0xf80000);
  Call3((void (*)())Func_02005598, 23, 0x2000000, 0xf80000);
  Call3((void (*)())Func_02005598, 9, 0x1d80000, 0x1000000);
  Call3((void (*)())Func_02005598, 10, 0x2280000, 0x1000000);
  t8 = Value1((s32 (*)())Object_GetById, 24);
  FIELD_AT(t8, s32, 24) = 0;
  *(u8 *)(85 + t2) = 0;
  t9 = FIELD_AT(t2, s32, 20);
  FIELD_AT(t2, s32, 12) = t9;
  *(u8 *)(85 + t0) = 0;
  t10 = FIELD_AT(t0, s32, 20);
  FIELD_AT(t0, s32, 12) = t10;
  *(u8 *)(85 + t1) = 0;
  t11 = FIELD_AT(t1, s32, 20);
  FIELD_AT(t1, s32, 12) = t11;
  t12 = Value1((s32 (*)())GameFlag_Test, 0xa0a);
  v_r8 = t3;
  v_r11 = (85 + t2);
  v_r10 = (85 + t1);
  v_r9 = (85 + t0);
  v_r5 = t2;
  v_r7 = t1;
  v_r6 = t0;
  if (t12 == 0)
    goto L_02009ca8;
  t13 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(v_r6, s32, 12) = (t13 + 0x66640);
  t14 = FIELD_AT(v_r5, s32, 12);
  FIELD_AT(v_r5, s32, 12) = (t14 + 0x1eb80);
  t15 = FIELD_AT(v_r5, s32, 8);
  FIELD_AT(v_r5, s32, 8) = (t15 + 0xfffd70c0);
  t16 = FIELD_AT(v_r8, u16, 18);
  FIELD_AT(v_r8, u16, 18) = (t16 + 0x800);
  t17 = Value1((s32 (*)())Object_GetById, 9);
  Call2((void (*)())Animation_ApplyChildValues, t17, 4);
  t19 = Value1((s32 (*)())Object_GetById, 11);
  Call2((void (*)())Animation_ApplyChildValues, t19, 4);
L_02009ca8:
  t21 = Value1((s32 (*)())GameFlag_Test, 0xa0b);
  if (t21 == 0)
    goto L_02009cf2;
  t22 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(v_r7, s32, 12) = (t22 + 0x66640);
  t23 = FIELD_AT(v_r5, s32, 12);
  FIELD_AT(v_r5, s32, 12) = (t23 + 0x1eb80);
  t24 = FIELD_AT(v_r5, s32, 8);
  FIELD_AT(v_r5, s32, 8) = (t24 + 0x28f40);
  t25 = FIELD_AT(v_r8, u16, 18);
  FIELD_AT(v_r8, u16, 18) = (t25 + -0x800);
  t26 = Value1((s32 (*)())Object_GetById, 10);
  Call2((void (*)())Animation_ApplyChildValues, t26, 4);
  t28 = Value1((s32 (*)())Object_GetById, 12);
  Call2((void (*)())Animation_ApplyChildValues, t28, 4);
L_02009cf2:
  t30 = Value1((s32 (*)())GameFlag_Test, 0xa0a);
  if (t30 == 0)
    goto L_02009d32;
  t31 = Value1((s32 (*)())GameFlag_Test, 0xa0b);
  if (t31 == 0)
    goto L_02009d32;
  t32 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(v_r6, s32, 12) = (t32 + 0x199900);
  t33 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(v_r7, s32, 12) = (t33 + 0x199900);
  t34 = FIELD_AT(v_r5, s32, 12);
  FIELD_AT(v_r5, s32, 12) = (t34 + 0x1b8480);
  *(u8 *)v_r9 = 4;
  *(u8 *)v_r10 = 4;
  *(u8 *)v_r11 = 4;
L_02009d32:
  return;
}
