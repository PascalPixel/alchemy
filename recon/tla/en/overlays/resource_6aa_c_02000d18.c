// Draft of overlay 6aa's Func_02000d18, translated from its listing: differ
// 116 of 93. Left: instruction order and register choice.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02004b80();
s32 Map_GetTerrainHeight();
s32 Object_GetById();
extern u8 Data_020068a4[];
extern u8 gData_020068a2[];
extern u8 gPartyState[];
extern u8 gResourceTableEntries[];

void Func_02000d18(void) {
  s32 v_r8, v_r6, v_r4, v_r7, t0, t1, t2, t3, t4, t5;
  s32 t6, t7, t8, t9, t10, t11, t12;

  t0 = FIELD_AT(0x3000000, s32, 32);
  t1 = FIELD_AT(t0, s32, 228);
  t2 = FIELD_AT(t0, s32, 232);
  t3 = *(s32 *)t0;
  t4 = FIELD_AT(t3, s32, 4);
  t5 = *(u16 *)(s32)gData_020068a2;
  t6 = FIELD_AT(((t5 << 2) + (s32)gResourceTableEntries), u16, 2);
  t7 = FIELD_AT((s32)gPartyState, s32, 532);
  t8 = Object_GetById(t7);
  t9 = FIELD_AT(t8, s32, 8);
  t10 = FIELD_AT(t8, s32, 16);
  t11 = Map_GetTerrainHeight(0, t9, t10);
  t12 = FIELD_AT(t8, s32, 16);
  v_r8 = (t2 & 0xffff0000);
  v_r6 = ((((t9 >> 20) << 20) - (t1 & 0xffff0000)) >> 16);
  v_r7 = ((u32)t6 >> 5);
  v_r4 = (((((((t12 >> 20) << 20) - v_r8) - t4) - (t11 - t4)) >> 16) + 16);
  if ((u32)(v_r6 + 15) > (u32)0x14e)
    goto L_02008dd0;
  if (v_r4 < -15)
    goto L_02008dd0;
  if (v_r4 > 239)
    goto L_02008dd0;
  *(s32 *)(s32)Data_020068a4 = 0;
  v_r6 = (v_r6 & 0x1ff);
  *(s32 *)((s32)Data_020068a4 + 4) =
      (((v_r4 & 255) | (v_r6 << 16)) | 0x80008800);
  v_r7 = (v_r7 | 0x400);
  FIELD_AT((s32)Data_020068a4, s32, 8) = v_r7;
  Func_02004b80((s32)Data_020068a4);
L_02008dd0:
  return;
}
