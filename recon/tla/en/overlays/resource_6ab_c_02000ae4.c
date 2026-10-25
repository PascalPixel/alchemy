// Draft of overlay 6ab's Func_02000ae4, translated from its listing: differ
// 57 of 80. Left: its shape: 77 instructions against the listing's 80.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020041e4();
s32 Func_020042ac();
s32 Func_0200446c();
s32 Runtime_AllocateHeapBlockFar();
s32 Runtime_ReleaseHeapBlock();
s32 VramCache_Load();
s32 WaitFrames();
extern u8 Data_02004cec[];
extern u8 Func_02000ab4[];

void Func_02000ae4(s32 p0) {
  s32 v_r0, v_r5, v_r7, v_r8, t0, t1, t2, t3, t4, t6;
  s32 t10;

  t0 = Func_020041e4(0x1e9, 0x2be0000, 0, 0xe00000);
  *(s32 *)(s32)Data_02004cec = t0;
  v_r8 = (s32)Data_02004cec;
  v_r7 = p0;
  v_r5 = 0;
  v_r0 = t0;
  if (v_r0 == v_r5)
    goto L_02008b92;
  t1 = FIELD_AT(v_r0, s32, 80);
  t2 = FIELD_AT(t1, u8, 5);
  t3 = FIELD_AT(t1, u8, 9);
  FIELD_AT(t1, u8, 5) = (-0x21 & t2);
  FIELD_AT(t1, u8, 9) = (((15 & t3) & -13) | 4);
  FIELD_AT(v_r0, u8, 85) = 3;
  FIELD_AT(v_r0, u8, 92) = 1;
  FIELD_AT(v_r0, s32, 108) = (s32)Func_02000ab4;
  FIELD_AT(t1, u8, 26) = v_r5;
  FIELD_AT(t1, u8, 27) = v_r5;
  t4 = Runtime_AllocateHeapBlockFar(68, 0x608);
  Func_020042ac(247);
  t6 = FIELD_AT(t1, u8, 16);
  v_r5 = (t4 + 0x400);
  VramCache_Load(t6, 128, v_r5);
  Runtime_ReleaseHeapBlock(68);
  if (v_r7 == 0)
    goto L_02008b92;
  WaitFrames(1);
  t10 = *(s32 *)v_r8;
  FIELD_AT(t10, s32, 40) = 0x28000;
  WaitFrames(4);
  Func_0200446c(135);
  WaitFrames(8);
  Func_0200446c(135);
L_02008b92:
  return;
}
