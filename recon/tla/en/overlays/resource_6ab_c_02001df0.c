// Draft of overlay 6ab's Func_02001df0, translated from its listing: differ
// 191 of 145. Left: its shape: 148 instructions against the listing's 145.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 GameFlag_Test();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Object_GetById();
s32 Scheduler_AddOrUpdateCallback();
extern u8 Func_02001bf8[];
extern u8 gData_020023c4[];

void Func_02001df0(s32 p0, s32 p1, s32 p2, s32 p3) {
  s32 sp_[1];
  s32 v_r6, v_r8, v_r9, v_r10, v_r11, v_r0, v_r5, v_r7, v_r1, v_r3;
  s32 v_r2, t0, t1, t2, t3, t5, t6, t7, t8, t9;
  s32 t10, t11, t12, t13, t14, t15, t16;

  FIELD_AT(sp_, s32, 4) = ((s32)gData_020023c4 + 0x188);
  FIELD_AT(sp_, s32, 12) = p2;
  FIELD_AT(sp_, s32, 8) = p3;
  t0 = GameFlag_Test(0x109);
  v_r9 = (((s32)gData_020023c4 + 0x188) + 20);
  v_r8 = p1;
  v_r6 = p0;
  if (t0 != 0)
    goto L_02009ed4;
  t1 = *(u16 *)v_r6;
  v_r11 = 0;
  v_r10 = t1;
  v_r6 = (v_r6 + 2);
  if (v_r10 <= v_r11)
    goto L_02009e92;
L_02009e2a:
  t2 = *(u16 *)v_r6;
  FIELD_AT(sp_, s32, 0) = (15 & t2);
  t3 = Object_GetById(v_r10);
  v_r7 = (t2 & 240);
  v_r5 = t3;
  v_r0 = t3;
  v_r6 = (v_r6 + 2);
  if (v_r5 == 0)
    goto L_02009e7e;
  ObjectDispatch_SetSingleChildFlags(v_r0, 0);
  *(u8 *)(v_r5 + 98) = 1;
  v_r2 = (128 & v_r7);
  v_r3 = (v_r5 + 98);
  v_r1 = 1;
  if (v_r2 != 0)
    goto L_02009e5e;
  *(u8 *)(v_r3 - 9) = v_r2;
L_02009e5e:
  *(u16 *)v_r9 = v_r1;
  FIELD_AT(v_r9, u16, 4) = v_r7;
  t5 = Object_GetById(v_r10);
  FIELD_AT(v_r9, s32, 8) = t5;
  t6 = FIELD_AT(sp_, s32, 0);
  FIELD_AT(v_r9, s32, 12) = (t6 << 16);
  FIELD_AT(v_r9, u16, 2) = v_r11;
  v_r9 = (v_r9 + 16);
L_02009e7e:
  v_r11 = (v_r11 + 1);
  if (v_r11 > 3)
    goto L_02009e92;
  t7 = *(u16 *)v_r6;
  v_r10 = t7;
  v_r6 = (v_r6 + 2);
  if (v_r10 > 0)
    goto L_02009e2a;
L_02009e92:
  v_r3 = v_r8;
  if (v_r3 == 0)
    goto L_02009ed4;
  t8 = *(u16 *)v_r3;
  t9 = FIELD_AT(sp_, s32, 4);
  FIELD_AT(t9, u16, 84) = t8;
  v_r11 = 0;
  v_r8 = (v_r8 + 2);
  v_r1 = t9;
  if (t8 <= v_r11)
    goto L_02009ed4;
  v_r2 = (v_r1 + 84);
L_02009eb0:
  t10 = *(u16 *)v_r8;
  FIELD_AT(v_r2, u16, 2) = t10;
  v_r11 = (v_r11 + 1);
  v_r2 = (v_r2 + 4);
  v_r8 = (v_r8 + 2);
  if (v_r11 > 3)
    goto L_02009ed4;
  t11 = *(u16 *)v_r8;
  *(u16 *)v_r2 = t11;
  v_r8 = (v_r8 + 2);
  if (t11 > 0)
    goto L_02009eb0;
L_02009ed4:
  t12 = FIELD_AT(sp_, s32, 12);
  t13 = FIELD_AT(sp_, s32, 4);
  FIELD_AT(t13, s32, 16) = t12;
  t14 = FIELD_AT(sp_, u16, 8);
  t15 = FIELD_AT(sp_, s32, 4);
  FIELD_AT(t15, u16, 10) = t14;
  t16 = *(u16 *)0x4000050;
  v_r1 = 0x4000050;
  if (t16 != 0)
    goto L_02009f04;
  *(u16 *)0x4000052 = 0xc08;
  *(u16 *)v_r1 = 0x3f10;
L_02009f04:
  Scheduler_AddOrUpdateCallback((s32)Func_02001bf8, 0x480);
  return;
}
