// Draft of overlay 6b3's Func_02000158, translated from its listing: differ
// 74 of 100. Left: its shape: 94 instructions against the listing's 100.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 BattleEffect_GetResourceId();
s32 Func_02000804();
s32 UiText_OpenMessageWindow();
s32 UiWork_Finalize();
s32 UiWork_IsComplete();
s32 WaitFrames();
extern u8 gData_02000a28[];
extern u8 gData_02000a2c[];
extern u8 gData_02000a34[];
extern u8 gData_02000a38[];

void Func_02000158(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4, s32 p5, s32 p6) {
  s32 sp_[1];
  s32 v_r7, v_r8, v_r5, v_r9, v_r10, v_r2, t0, t1, t2, t3;
  s32 t4, t6, t11;

  v_r7 = (p0 & 0x7fff);
  t0 = BattleEffect_GetResourceId(v_r7);
  t1 = *(s32 *)(s32)gData_02000a28;
  t2 = UiText_OpenMessageWindow(t1, p1, p2, ((t0 << 16) | 4));
  v_r8 = (0x8000 & p0);
  v_r10 = p3;
  v_r9 = 0;
  v_r5 = t2;
  if (v_r8 != v_r9)
    goto L_020081c8;
  t3 = Func_02000804(v_r7, 0, v_r10, p4);
  *(u8 *)(s32)gData_02000a34 = 1;
  *(u16 *)(s32)gData_02000a38 = (v_r10 << 3);
  *(u16 *)(s32)gData_02000a2c = (p4 << 3);
  v_r9 = t3;
L_020081c8:
  if (p6 != 2)
    goto L_020081e2;
  t4 = FIELD_AT(v_r5, u16, 14);
  FIELD_AT(v_r5, u16, 14) = (t4 + 1);
  FIELD_AT(v_r5, u16, 8) = 0;
  FIELD_AT(v_r5, u16, 10) = 0;
  goto L_020081e2;
L_020081dc:
  WaitFrames(1);
L_020081e2:
  t6 = UiWork_IsComplete();
  if (t6 == 0)
    goto L_020081dc;
  WaitFrames(1);
  UiWork_Finalize(v_r5, 2);
  if (v_r8 != 0)
    goto L_0200820c;
  UiWork_Finalize(v_r9, 2);
  *(u8 *)(s32)gData_02000a34 = v_r8;
L_0200820c:
  v_r2 = p5;
  if (v_r2 <= 0)
    goto L_02008220;
  v_r5 = v_r2;
L_02008214:
  WaitFrames(1);
  v_r5 = (v_r5 - 1);
  if (v_r5 != 0)
    goto L_02008214;
L_02008220:
  t11 = *(s32 *)(s32)gData_02000a28;
  *(s32 *)(s32)gData_02000a28 = (t11 + 1);
  return;
}
