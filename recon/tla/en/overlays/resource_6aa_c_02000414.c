// Draft of overlay 6aa's Func_02000414, translated from its listing: differ
// 92 of 113. Left: its shape: 109 instructions against the listing's 113.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 Func_02004bd0();
s32 Func_02004bd8();
s32 Math_Cosine();
s32 Math_Sine();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Object_GetById();
s32 Random16Far();
extern u8 IwramMulQ16[];
extern u8 gData_02005394[];

void Func_02000414(s32 p0) {
  s32 v_r9, v_r11, v_r5, v_r8, v_r6, v_r7, t0, t1, t2, t3;
  s32 t4, t5, t6, t7, t8, t9, t10, t11, t12, t15;

  t0 = Value1((s32 (*)())Object_GetById, p0);
  v_r11 = 5;
  v_r9 = t0;
L_0200842c:
  t1 = Value0((s32 (*)())Random16Far);
  t2 = Value0((s32 (*)())Random16Far);
  t3 = FIELD_AT(v_r9, s32, 8);
  v_r5 = (t1 << 4);
  v_r8 = (t3 + v_r5);
  t4 = Value0((s32 (*)())Random16Far);
  t5 = Value0((s32 (*)())Random16Far);
  t6 = Value0((s32 (*)())Random16Far);
  t7 = FIELD_AT(v_r9, s32, 12);
  t8 = FIELD_AT(v_r9, s32, 16);
  v_r5 = (t5 << 4);
  v_r8 = (v_r8 - (t2 << 4));
  v_r6 = (((t4 << 3) + t7) + 0x40000);
  t9 = Value4((s32 (*)())Func_02004bd8, 0x145, v_r8, v_r6,
              ((t8 + v_r5) - (t6 << 4)));
  v_r7 = t9;
  if (v_r7 == 0)
    goto L_020084fc;
  t10 = Value0((s32 (*)())Random16Far);
  t11 = Value0((s32 (*)())Random16Far);
  t12 = Value0((s32 (*)())Random16Far);
  Call2((void (*)())Func_02004bd0, v_r7, (s32)gData_02005394);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v_r7, 0);
  FIELD_AT(v_r7, s32, 40) = ((t10 << 2) + 0x20000);
  t15 = Value1((s32 (*)())Math_Cosine, t10);
  v_r6 = (t11 << 3);
  FIELD_AT(v_r7, s32, 44) = v_r6;
  Call2((void (*)())Math_Sine, t10, t15);
  FIELD_AT(v_r7, s32, 52) = 0;
  FIELD_AT(v_r7, s32, 72) = 0x547a;
  FIELD_AT(v_r7, s32, 36) = v_r6;
  v_r5 = (((u32)t12 >> 1) + 0x4000);
  FIELD_AT(v_r7, s32, 24) = v_r5;
  FIELD_AT(v_r7, s32, 28) = v_r5;
  FIELD_AT(v_r7, s32, 68) = 0x8000;
  Call2((void (*)())Animation_ApplyChildValues, v_r7, 1);
L_020084fc:
  v_r11 = (v_r11 + -1);
  if (v_r11 >= 0)
    goto L_0200842c;
  return;
}
