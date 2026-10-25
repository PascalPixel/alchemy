// Draft of overlay 6ad's Func_02004938, translated from its listing: differ
// 145 of 126. Left: its shape: 121 instructions against the listing's 126.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02004a44();
s32 Func_020054e8();
s32 Object_GetById();
extern u8 gData_02005f58[];
extern u8 gData_02005f64[];

void Func_02004938(s32 p0) {
  s32 v_r1, v_r0, v_r8, v_r5, v_r4, v_r2, v_r6, v_r3, v_r7, t0;
  s32 t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
  s32 t11, t12, t13, t14, t15, t16, t17;

  t0 = FIELD_AT(0x3000000, s32, 32);
  t1 = Object_GetById(p0);
  t2 = FIELD_AT(t1, s32, 80);
  t3 = FIELD_AT(t2, s32, 40);
  t4 = *(s16 *)t3;
  t5 = *(u16 *)(s32)gData_02005f58;
  v_r4 = (t4 << 16);
  v_r5 = (s32)gData_02005f58;
  v_r8 = t0;
  v_r0 = t1;
  v_r1 = 0;
  if (((u32)(t4 << 16) >> 16) == t5)
    goto L_0200c97e;
L_0200c964:
  t6 = ((u32)((v_r1 << 16) + 0x10000) >> 16);
  v_r1 = (((v_r1 << 16) + 0x10000) >> 16);
  v_r2 = t6;
  if ((u32)v_r2 > (u32)5)
    goto L_0200c97e;
  t7 = *(u16 *)(v_r5 + (v_r2 << 1));
  if (((u32)v_r4 >> 16) != t7)
    goto L_0200c964;
L_0200c97e:
  v_r2 = ((u32)(v_r1 << 16) >> 16);
  if (v_r2 != 6)
    goto L_0200c98a;
  goto L_0200ca32;
L_0200c98a:
  t8 = *(s8 *)((s32)gData_02005f64 + (v_r2 << 2));
  v_r4 = t8;
  v_r1 = t8;
  v_r6 = (s32)gData_02005f64;
  v_r2 = (v_r2 << 2);
  if (v_r4 >= 0)
    goto L_0200c998;
  v_r1 = -v_r4;
L_0200c998:
  t9 = *(s8 *)(v_r6 + (v_r2 + 2));
  v_r3 = t9;
  if (v_r3 >= 0)
    goto L_0200c9a2;
  v_r3 = -v_r3;
L_0200c9a2:
  t10 = *(s8 *)(v_r6 + (v_r2 + 1));
  t11 = ((v_r1 + v_r3) >> 4);
  v_r1 = t10;
  v_r5 = t10;
  v_r7 = t11;
  if (v_r1 >= 0)
    goto L_0200c9b2;
  v_r5 = -v_r1;
L_0200c9b2:
  t12 = *(s8 *)(v_r6 + (v_r2 + 3));
  v_r2 = t12;
  if (v_r2 >= 0)
    goto L_0200c9bc;
  v_r2 = -v_r2;
L_0200c9bc:
  v_r5 = (v_r5 + v_r2);
  t13 = FIELD_AT(v_r0, s32, 8);
  t14 = FIELD_AT(v_r0, s32, 16);
  t15 = FIELD_AT(v_r8, s32, 328);
  t16 = FIELD_AT(v_r8, s32, 332);
  v_r6 = ((t13 + (v_r4 << 16)) >> 20);
  t17 = (v_r5 >> 4);
  v_r5 = ((t14 + (v_r1 << 16)) >> 20);
  Func_020054e8(v_r6, v_r5, v_r7, t17,
                (v_r6 + ((u32)((t15 >> 20) << 16) >> 16)),
                (v_r5 + ((u32)((t16 >> 20) << 16) >> 16)));
  Func_02004a44(0, v_r6, v_r5, v_r7, t17, 255);
  Func_02004a44(2, v_r6, v_r5, v_r7, t17, 255);
L_0200ca32:
  return;
}
