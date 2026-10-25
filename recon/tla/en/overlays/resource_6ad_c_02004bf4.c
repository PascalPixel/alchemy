// Draft of overlay 6ad's Func_02004bf4, translated from its listing: differ
// 212 of 132. Left: its shape: 152 instructions against the listing's 132.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Object_GetById();
extern u8 gData_02005f58[];
extern u8 gData_02005f64[];
extern u8 gData_02005f7c[];
extern u8 gPartyState[];

s32 Func_02004bf4(s32 p0, s32 p1, s32 p2) {
  s32 sp_[3];
  s32 v_r5, v_r14, v_r11, v_r7, v_r6, v_r10, v_r9, v_r0, v_r12, v_r8;
  s32 v_r1, t0, t1, t2, t3, t4, t5, t6, t7, t8;
  s32 t9, t10, t11, t12, t13, t14, t15, t16, t17, t18;
  s32 t19, t20, t21, t22, t23, t24, t25;

  FIELD_AT(sp_, s32, 8) = p0;
  FIELD_AT(sp_, s32, 4) = p1;
  FIELD_AT(sp_, s32, 0) = p2;
  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = *(s32 *)((s32)gPartyState + 0x214);
  t2 = Object_GetById(t1);
  t3 = FIELD_AT(t2, u16, 6);
  t4 = FIELD_AT(sp_, s32, 8);
  *(s32 *)t4 = ((u32)t3 >> 12);
  v_r5 = (t0 + 52);
  v_r7 = t2;
  v_r11 = 8;
  v_r14 = v_r5;
L_0200cc30:
  t5 = *(s32 *)v_r14;
  v_r5 = 0;
  v_r6 = t5;
L_0200cc36:
  t6 = FIELD_AT(v_r6, s32, 80);
  t7 = FIELD_AT(t6, s32, 40);
  t8 = *(s16 *)t7;
  t9 = *(u16 *)((s32)gData_02005f58 + (v_r5 << 1));
  if (t8 != t9)
    goto L_0200ccda;
  t10 = FIELD_AT(sp_, s32, 8);
  t11 = FIELD_AT(v_r7, s16, 10);
  t12 = *(s32 *)t10;
  t13 = *(s32 *)((s32)gData_02005f7c + (t12 << 2));
  t14 = FIELD_AT(v_r7, s16, 18);
  t15 = FIELD_AT(v_r6, s16, 10);
  t16 = *(s8 *)((s32)gData_02005f64 + (v_r5 << 2));
  t17 = FIELD_AT(v_r6, s16, 18);
  t18 = *(s8 *)((s32)gData_02005f64 + ((v_r5 << 2) + 1));
  t19 = *(s8 *)((s32)gData_02005f64 + ((v_r5 << 2) + 2));
  t20 = *(s8 *)((s32)gData_02005f64 + ((v_r5 << 2) + 3));
  v_r10 = ((t15 + t16) >> 4);
  v_r9 = ((t11 + (t13 >> 16)) >> 4);
  v_r1 = ((t17 + t20) >> 4);
  v_r8 = ((t14 + ((t13 << 16) >> 16)) >> 4);
  v_r12 = ((t17 + t18) >> 4);
  v_r0 = ((t15 + t19) >> 4);
  if (v_r10 > v_r9)
    goto L_0200ccda;
  if (v_r9 >= v_r0)
    goto L_0200ccda;
  if (v_r12 > v_r8)
    goto L_0200ccda;
  if (v_r8 >= v_r1)
    goto L_0200ccda;
  t21 = FIELD_AT(sp_, s32, 0);
  *(s32 *)t21 = v_r5;
  if ((1 & v_r5) == 0)
    goto L_0200ccc8;
  t22 = FIELD_AT(v_r7, s32, 8);
  if (v_r10 == (t22 >> 20))
    goto L_0200ccda;
  t23 = FIELD_AT(sp_, s32, 4);
  *(s32 *)t23 = v_r11;
  v_r0 = v_r6;
  goto L_0200ccf0;
L_0200ccc8:
  t24 = FIELD_AT(v_r7, s32, 16);
  if (v_r12 == (t24 >> 20))
    goto L_0200ccda;
  t25 = FIELD_AT(sp_, s32, 4);
  *(s32 *)t25 = v_r11;
  v_r0 = v_r6;
  goto L_0200ccf0;
L_0200ccda:
  v_r5 = (v_r5 + 1);
  if ((u32)v_r5 <= (u32)5)
    goto L_0200cc36;
  v_r11 = (v_r11 + 1);
  v_r14 = (v_r14 + 4);
  if ((u32)v_r11 <= (u32)63)
    goto L_0200cc30;
  v_r0 = 0;
L_0200ccf0:
  return v_r0;
}
