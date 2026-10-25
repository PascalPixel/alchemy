// Draft of overlay 6ae's Func_020004a4, translated from its listing: differ
// 90 of 103. Left: its shape: 111 instructions against the listing's 103.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_0200556c();
s32 Func_02005574();
s32 Math_Sine();
s32 Object_GetById();
s32 Object_SetPartAttribute();
s32 Random16Far();
s32 __divsi3();
extern u8 Data_0200eb60[];
extern u8 Func_020005a0[];
extern u8 gData_02006ad0[];

s32 Func_020004a4(void) {
  s32 v_r0, v_r6, v_r8, v_r5, v_r7, v_r3, t0, t1, t2, t3;
  s32 t4, t5, t6, t7, t8, t11, t12, t13, t14, t15;
  s32 t16;

  t0 = Object_GetById(8);
  t1 = Random16Far();
  v_r8 = t0;
  v_r6 = (((u32)(((t1 << 1) + t1) << 4) >> 16) << 16);
  v_r0 = t0;
  if (v_r8 == 0)
    goto L_0200858a;
  t2 = *(s32 *)(s32)Data_0200eb60;
  v_r5 = (t2 & 15);
  if (v_r5 != 0)
    goto L_0200858a;
  t3 = FIELD_AT(v_r0, s32, 12);
  t4 = FIELD_AT(v_r0, s32, 8);
  t5 = FIELD_AT(v_r0, s32, 16);
  t6 = Func_02005574(0x2a0, (t4 + 0x80000), ((t3 + v_r6) + 0x80000), t5);
  t7 = __divsi3(v_r6, 0x60000);
  v_r6 = (t7 << 16);
  v_r7 = t6;
  v_r0 = t7;
  if (v_r7 == 0)
    goto L_0200858a;
  t8 = FIELD_AT(v_r7, s32, 80);
  Func_0200556c(v_r7, (s32)gData_02006ad0);
  Object_SetPartAttribute(v_r7, 10);
  *(u8 *)(v_r7 + 85) = v_r5;
  t11 = Random16Far();
  *(u16 *)(v_r7 + 100) = (0xffff000 & t11);
  *(u16 *)(v_r7 + 102) = v_r5;
  FIELD_AT(v_r7, s32, 108) = (s32)Func_020005a0;
  FIELD_AT(v_r7, s32, 104) = v_r8;
  v_r6 = (v_r6 & 0xfffff);
  t12 = Math_Sine((v_r6 >> 4), v_r8);
  FIELD_AT(v_r7, s32, 48) = ((((t12 << 1) + t12) << 3) >> 16);
  FIELD_AT(t8, u8, 26) = 0;
  t13 = FIELD_AT(v_r8, s32, 80);
  t14 = FIELD_AT(t13, u8, 9);
  t15 = FIELD_AT(t8, u8, 9);
  v_r0 = t8;
  v_r3 = ((-13 & t15) | (12 & t14));
  goto L_02008588;
L_02008588:
  FIELD_AT(v_r0, u8, 9) = v_r3;
L_0200858a:
  t16 = *(s32 *)(s32)Data_0200eb60;
  *(s32 *)(s32)Data_0200eb60 = (t16 + 1);
  return v_r0;
}
