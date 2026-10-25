// Draft of overlay 6ae's Func_020053e4, translated from its listing: differ
// 124 of 112. Left: its shape: 88 instructions against the listing's 112.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020057a4();
s32 Func_020057b4();
s32 Resource_FindFreeEntry();
s32 VramCache_Load();
extern u8 Func_02005268[];
extern u8 gData_020057e4[];
extern u8 gResourceTableEntries[];

s32 Func_020053e4(s32 p0, s32 p1) {
  s32 sp_[1];
  s32 v_r8, v_r0, v_r3, v_r6, v_r7, v_r5, v_r10, v_r11, v_r9, t0;
  s32 t2, t3, t5, t6, t7, t8;

  t0 = Resource_FindFreeEntry();
  v_r8 = (164 + p1);
  *(u16 *)v_r8 = t0;
  VramCache_Load(((t0 << 16) >> 16), 0x100, (s32)gData_020057e4);
  *(u16 *)(p1 + 166) = 0;
  *(u16 *)((p1 + 166) - 6) = 0x3e7;
  *(u16 *)(((p1 + 166) - 6) + 2) = 0x3e7;
  *(s32 *)((((p1 + 166) - 6) + 2) + 6) = p0;
  *(u8 *)(p0 + 98) = 0;
  v_r5 = 0;
  v_r7 = p1;
  v_r6 = p0;
  v_r3 = ((p0 + 98) + 1);
  v_r0 = 0;
  goto L_0200d448;
L_0200d448:
  *(u8 *)v_r3 = v_r0;
  FIELD_AT(v_r6, s32, 108) = (s32)Func_02005268;
  t2 = *(s16 *)v_r8;
  t3 = FIELD_AT(((t2 << 2) + (s32)gResourceTableEntries), u16, 2);
  FIELD_AT(v_r6, s32, 104) = v_r7;
  v_r9 = v_r5;
  v_r11 = ((u32)t3 >> 5);
  v_r10 = v_r5;
L_0200d466:
  v_r8 = 1;
  v_r5 = (v_r10 + v_r7);
L_0200d46e:
  FIELD_AT(sp_, s32, 0) = v_r11;
  Func_020057a4(v_r5, 16, 16, 0x80004000);
  t5 = FIELD_AT(v_r5, u8, 5);
  t6 = FIELD_AT(v_r5, u8, 9);
  FIELD_AT(v_r5, u8, 5) = (t5 & -0x21);
  FIELD_AT(v_r5, u8, 9) = (15 & t6);
  t7 = Func_020057b4(v_r6);
  t8 = FIELD_AT(v_r5, u8, 9);
  FIELD_AT(v_r5, u8, 9) = ((t8 & -13) | ((t7 & 3) << 2));
  v_r8 = (v_r8 + -1);
  v_r5 = (v_r5 + 40);
  if (v_r8 >= 0)
    goto L_0200d46e;
  v_r9 = (v_r9 + 1);
  v_r0 = 80;
  v_r10 = (v_r10 + 80);
  if (v_r9 <= 1)
    goto L_0200d466;
  return v_r0;
}
