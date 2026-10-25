// Draft of overlay 6a5's Func_020005b8, translated from its listing: differ
// 2 of 69. Left: instruction order and register choice.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02005884();
s32 Object_GetById();
s32 Random16Far();
extern u8 gFrameCount[];

void Func_020005b8(void) {
  s32 v_r6, v_r5, v_r0, t0, t1, t3, t4, t5, t6, t7;
  s32 t9, t10, t12, t13, t14;

  t0 = Object_GetById(36);
  t1 = FIELD_AT(0x3000000, s32, 32);
  Random16Far();
  v_r6 = t1;
  if (t0 == 0)
    goto L_02008656;
  t3 = FIELD_AT((v_r6 + 232), s16, 2);
  if (t3 > 177)
    goto L_0200864c;
  t4 = Object_GetById(36);
  t5 = FIELD_AT(t4, s32, 80);
  t6 = FIELD_AT(t5, s32, 40);
  FIELD_AT(t6, u8, 5) = 1;
  t7 = *(s32 *)(s32)gFrameCount;
  if ((t7 & 1) == 0)
    goto L_0200861a;
  Func_02005884(36, 0x2000000, 0x1080000);
  t9 = Object_GetById(36);
  FIELD_AT(t9, s32, 12) = 0x6f8000;
  t10 = Object_GetById(36);
  v_r0 = t10;
  v_r5 = 0x10000;
  goto L_02008640;
L_0200861a:
  Func_02005884(36, 0x2000000, 0x1080000);
  t12 = Object_GetById(36);
  FIELD_AT(t12, s32, 12) = 0x688000;
  t13 = Object_GetById(36);
  v_r0 = t13;
  v_r5 = 0x14ccc;
L_02008640:
  FIELD_AT(v_r0, s32, 24) = v_r5;
  t14 = Object_GetById(36);
  FIELD_AT(t14, s32, 28) = v_r5;
  goto L_02008656;
L_0200864c:
  Func_02005884(36, 0, 0);
L_02008656:
  return;
}
