// Draft of overlay 6ab's Func_02001424, translated from its listing: differ
// 79 of 91. Left: its shape: 94 instructions against the listing's 91.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_SetStateFlags();
s32 Func_020041cc();
s32 Func_020041e4();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Random16Far();
extern u8 Func_020013e4[];
extern u8 gFrameCount[];

void Func_02001424(void) {
  s32 v_r2, v_r1, v_r3, v_r5, t0, t1, t2, t3, t4, t5;
  s32 t6, t7, t8;

  t0 = *(s32 *)(s32)gFrameCount;
  v_r1 = 1;
  v_r2 = t0;
  if ((t0 & 1) != 0)
    goto L_020094e8;
  if ((((u32)v_r2 >> 1) & v_r1) == 0)
    goto L_02009454;
  t1 = Value0((s32 (*)())Random16Far);
  t2 = Value0((s32 (*)())Random16Far);
  v_r5 = ((u32)(((t1 << 3) - t1) << 2) >> 16);
  v_r3 = ((t2 << 4) - t2);
  v_r2 = 155;
  goto L_0200946a;
L_02009454:
  t3 = Value0((s32 (*)())Random16Far);
  t4 = Value0((s32 (*)())Random16Far);
  v_r5 = ((u32)(((t3 << 3) - t3) << 2) >> 16);
  v_r3 = ((t4 << 4) - t4);
  v_r2 = 178;
L_0200946a:
  v_r5 = ((v_r5 << 16) + (v_r2 << 18));
  t5 = Value4((s32 (*)())Func_020041e4, 0x11d, v_r5, 0x200000,
              ((((u32)(v_r3 << 3) >> 16) << 16) + 0x1cc0000));
  v_r5 = t5;
  if (v_r5 == 0)
    goto L_020094e8;
  FIELD_AT(v_r5, u8, 92) = 2;
  FIELD_AT(v_r5, u8, 85) = 0;
  FIELD_AT(v_r5, u16, 100) = 0;
  t6 = FIELD_AT(v_r5, u8, 35);
  FIELD_AT(v_r5, u8, 35) = (254 & t6);
  t7 = FIELD_AT(v_r5, s32, 80);
  t8 = FIELD_AT(t7, u8, 9);
  FIELD_AT(t7, u8, 9) = ((-13 & t8) | 8);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v_r5, 0);
  FIELD_AT(v_r5, s32, 24) = 0x8000;
  FIELD_AT(v_r5, s32, 28) = 0x8000;
  FIELD_AT(v_r5, s32, 104) = 60;
  FIELD_AT(v_r5, s32, 108) = (s32)Func_020013e4;
  Call2((void (*)())Func_020041cc, v_r5, 5);
  Call2((void (*)())Animation_SetStateFlags, v_r5, 1);
L_020094e8:
  return;
}
