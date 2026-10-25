// Draft of overlay 6aa's Func_02000338, translated from its listing: differ
// 49 of 93. Left: its shape: 94 instructions against the listing's 93.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020001f8();
s32 Func_02003b30();
s32 GameFlag_SetBit();
s32 Object_GetById();
s32 Random16Far();

void Func_02000338(s32 p0, s32 p1) {
  s32 sp_[1];
  s32 spo_[4];
  s32 v_r9, v_r7, v_r10, v_r6, t0, t1, t2, t3, t4, t5;
  s32 t6, t7, t9, t10, t12, t13;

  t0 = *(s32 *)0x30000e0;
  t1 = FIELD_AT(t0, s16, 26);
  t2 = Object_GetById(t1);
  v_r6 = p1;
  v_r10 = t0;
  v_r7 = t2;
  v_r9 = p0;
  if (v_r6 > 7)
    goto L_020083cc;
  *(s32 *)sp_ = 0;
  FIELD_AT(sp_, u16, 24) = 0x117;
  FIELD_AT(sp_, s32, 12) = 0x10000;
  FIELD_AT(sp_, s32, 8) = 0x10000;
  FIELD_AT(sp_, s32, 20) = 0x8000;
  FIELD_AT(sp_, s32, 16) = 0x8000;
  t3 = Random16Far();
  FIELD_AT(sp_, u16, 32) = ((t3 & 15) << 12);
  t4 = Random16Far();
  t5 = FIELD_AT(v_r7, s32, 12);
  t6 = FIELD_AT(v_r7, s32, 16);
  t7 = FIELD_AT(v_r7, s32, 8);
  Func_02003b30(t7, ((t5 - (((v_r6 << 1) + v_r6) << 16)) + 0x100000), t6,
                (((15 & t4) - 8) << 13), 0, 0, 0x5a0000, (s32)sp_);
  goto L_020083c4;
L_020083c4:
  t9 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t9 - 0x1f00);
L_020083cc:
  if (v_r9 != 1)
    goto L_020083fa;
  FIELD_AT(v_r7, s32, 28) = 0x10000;
  t10 = FIELD_AT(v_r7, s16, 100);
  GameFlag_SetBit(t10);
  t12 = FIELD_AT(v_r10, s16, 26);
  t13 = Object_GetById(t12);
  Func_020001f8(t13);
  FIELD_AT(v_r7, s32, 16) = 0;
  FIELD_AT(v_r7, s32, 12) = 0;
  FIELD_AT(v_r7, s32, 8) = 0;
L_020083fa:
  return;
}
