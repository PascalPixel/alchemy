// Draft of overlay 6ad's Func_02002e8c, translated from its listing: differ
// 94 of 98. Left: its shape: 96 instructions against the listing's 98.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 Func_02005490();
s32 Func_02005498();
s32 Math_Cosine();
s32 Math_Sine();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Random16Far();
extern u8 IwramMulQ16[];
extern u8 gData_02005e00[];

void Func_02002e8c(s32 p0, s32 p1, s32 p2, s32 p3) {
  s32 sp_[4];
  s32 v_r11, v_r7, v_r5, v_r8, v_r6, t0, t1, t2, t3, t4;
  s32 t5, t6, t7, t8, t11, t12;

  FIELD_AT(sp_, s32, 12) = p0;
  FIELD_AT(sp_, s32, 8) = p1;
  FIELD_AT(sp_, s32, 4) = p2;
  FIELD_AT(sp_, s32, 0) = p3;
  v_r11 = 1;
L_0200aea8:
  t0 = FIELD_AT(sp_, s32, 12);
  t1 = FIELD_AT(sp_, s32, 8);
  t2 = FIELD_AT(sp_, s32, 4);
  t3 = Func_02005498(0x145, t0, t1, t2);
  v_r7 = t3;
  if (v_r7 == 0)
    goto L_0200af4a;
  t4 = Random16Far();
  t5 = Random16Far();
  t6 = FIELD_AT(sp_, s32, 0);
  v_r5 = ((t6 + ((u32)t4 >> 4)) - ((u32)t5 >> 4));
  t7 = Random16Far();
  t8 = Random16Far();
  Func_02005490(v_r7, (s32)gData_02005e00);
  ObjectDispatch_SetSingleChildFlags(v_r7, 0);
  t11 = (v_r5 + 0x4000);
  v_r5 = (v_r5 + 0x14000);
  FIELD_AT(v_r7, s32, 40) = v_r5;
  t12 = Math_Cosine(t11);
  v_r8 = (t7 << 3);
  FIELD_AT(v_r7, s32, 44) = v_r8;
  Math_Sine(t11, t12);
  FIELD_AT(v_r7, s32, 52) = 0;
  FIELD_AT(v_r7, s32, 72) = 0x547a;
  FIELD_AT(v_r7, s32, 36) = v_r8;
  v_r6 = (((u32)t8 >> 1) + 0x4000);
  FIELD_AT(v_r7, s32, 24) = v_r6;
  FIELD_AT(v_r7, s32, 28) = v_r6;
  FIELD_AT(v_r7, s32, 68) = 0x8000;
  Animation_ApplyChildValues(v_r7, 1);
L_0200af4a:
  v_r11 = (v_r11 + -1);
  if (v_r11 >= 0)
    goto L_0200aea8;
  return;
}
