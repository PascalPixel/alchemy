// Draft of overlay 6b4's Func_02000478, translated from its listing: differ
// 99 of 104. Left: its shape: 107 instructions against the listing's 104.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020000b8();
s32 Math_Cosine();
s32 Math_Sine();
s32 Object_GetById();
s32 Random16Far();
s32 __divsi3();
extern u8 Effect_AdvanceMotion[];

void Func_02000478(s32 p0, s32 p1) {
  s32 sp_[1];
  s32 spo_[4];
  s32 v_r9, v_r10, v_r11, v_r8, v_r7, v_r5, v_r3, v_r6, t0, t1;
  s32 t2, t3, t4, t5, t6, t7, t8, t9, t10, t11;
  s32 t12, t13, t14, t15;

  t0 = Object_GetById(8);
  FIELD_AT(sp_, s32, 48) = (s32)Effect_AdvanceMotion;
  FIELD_AT(sp_, s32, 20) = 0xcccc;
  FIELD_AT(sp_, s32, 24) = 0xcccc;
  t1 = FIELD_AT(t0, s32, 80);
  t2 = FIELD_AT(t1, u8, 9);
  FIELD_AT(sp_, s32, 12) = ((u32)(t2 << 28) >> 30);
  v_r7 = ((s32)sp_ + 12);
  v_r8 = t0;
  v_r11 = p0;
  v_r10 = 0;
  v_r9 = p1;
L_020084b2:
  t3 = __divsi3(0x10000, v_r9);
  v_r5 = (v_r10 * t3);
  t4 = Math_Cosine(v_r5);
  v_r6 = (s32)sp_;
  v_r3 = (v_r11 * t4);
  if (v_r3 >= 0)
    goto L_020084d2;
  v_r3 = (v_r3 + 255);
L_020084d2:
  *(s32 *)v_r6 = (v_r3 >> 8);
  FIELD_AT(v_r6, s32, 4) = 0;
  t5 = Math_Sine(v_r5);
  v_r3 = (v_r11 * t5);
  if (v_r3 >= 0)
    goto L_020084ea;
  v_r3 = (v_r3 + 255);
L_020084ea:
  FIELD_AT(v_r6, s32, 8) = (v_r3 >> 8);
  t6 = *(s32 *)v_r6;
  *(s32 *)v_r6 = (t6 + (t6 >> 1));
  t7 = Random16Far();
  t8 = *(s32 *)v_r6;
  *(s32 *)v_r6 = ((t8 + ((u32)(((t7 << 1) + t7) << 14) >> 16)) - 0x6000);
  t9 = Random16Far();
  t10 = FIELD_AT(v_r6, s32, 8);
  v_r5 = ((t10 + ((u32)(((t9 << 1) + t9) << 13) >> 16)) - 0x3000);
  FIELD_AT(v_r6, s32, 8) = v_r5;
  t11 = FIELD_AT(v_r6, s32, 4);
  t12 = FIELD_AT(v_r8, s32, 16);
  t13 = FIELD_AT(v_r8, s32, 8);
  t14 = FIELD_AT(v_r8, s32, 12);
  t15 = *(s32 *)v_r6;
  Func_020000b8(t13, t14, t12, t15, t11, v_r5, 0x10a0001, v_r7);
  v_r10 = (v_r10 + 1);
  if ((u32)v_r10 <= (u32)v_r9)
    goto L_020084b2;
  return;
}
