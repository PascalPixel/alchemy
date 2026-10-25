// Draft of overlay 6a9's Func_020006a8, translated from its listing: differ
// 112 of 148. Left: its shape: 144 instructions against the listing's 148.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020003f0();
s32 Func_0200053c();
s32 Func_020010cc();
s32 Map_GetTerrainHeight();
s32 Object_GetById();
s32 Vector_AddPolarOffsetFar();
extern u8 gData_02001220[];
extern u8 gPartyState[];

void Func_020006a8(void) {
  s32 sp_[1];
  s32 v_r8, v_r6, v_r7, v_r2, v_r1, v_r5, v_r3, t0, t1, t2;
  s32 t3, t4, t6, t7, t8, t9, t10, t11, t13, t14;
  s32 t15, t16, t17, t18, t19, t20, t21, t22, t23, t25;
  s32 t26, t27, t28, t29, t31, t32, t34, t35, t36;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = *(s32 *)0x30000e0;
  *(s32 *)(t0 + 0xcbc) = 0;
  v_r1 = (t0 + 0xcbc);
  v_r2 = t1;
  v_r7 = t0;
  v_r6 = 0;
  v_r8 = (s32)gData_02001220;
  if (v_r2 == v_r6)
    goto L_020086f2;
  t2 = *(s16 *)(v_r7 + 0x166);
  if (t2 == 0)
    goto L_020086f2;
  t3 = FIELD_AT(v_r2, s32, 20);
  *(s32 *)v_r1 = t3;
  t4 = FIELD_AT(v_r2, s16, 28);
  v_r6 = t3;
  if (t4 == 147)
    goto L_020087a2;
  Call1((void (*)())Func_0200053c, 1);
  goto L_020087a2;
L_020086f2:
  t6 = *(s32 *)((s32)gPartyState + 0x214);
  t7 = Value1((s32 (*)())Object_GetById, t6);
  t8 = FIELD_AT(t7, u16, 6);
  t9 = FIELD_AT(t7, s32, 8);
  FIELD_AT(sp_, s32, 0) = t9;
  t10 = FIELD_AT(t7, s32, 12);
  FIELD_AT(sp_, s32, 4) = t10;
  t11 = FIELD_AT(t7, s32, 16);
  FIELD_AT(sp_, s32, 8) = t11;
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000,
        ((t8 + 0x2000) & 0xc000), ((s32)sp_ + 0));
  t13 = *(u16 *)v_r8;
  v_r5 = (s32)sp_;
  if (t13 == 0xffff)
    goto L_0200878e;
L_02008734:
  t14 = *(u16 *)v_r8;
  t15 = Value1((s32 (*)())Object_GetById, t14);
  t16 = *(s32 *)v_r5;
  t17 = FIELD_AT(t15, s32, 8);
  v_r6 = t15;
  if ((t16 >> 20) != (t17 >> 20))
    goto L_0200877a;
  t18 = FIELD_AT(v_r5, s32, 4);
  v_r3 = t18;
  if (v_r3 >= 0)
    goto L_02008758;
  v_r3 = (v_r3 + 0xffff);
L_02008758:
  t19 = FIELD_AT(v_r6, s32, 12);
  t20 = (v_r3 >> 16);
  v_r3 = t19;
  v_r2 = t20;
  if (v_r3 >= 0)
    goto L_02008768;
  v_r3 = (v_r3 + 0xffff);
L_02008768:
  if (v_r2 != (v_r3 >> 16))
    goto L_0200877a;
  t21 = FIELD_AT(v_r5, s32, 8);
  t22 = FIELD_AT(v_r6, s32, 16);
  if ((t21 >> 20) == (t22 >> 20))
    goto L_0200878e;
L_0200877a:
  v_r8 = (v_r8 + 2);
  t23 = *(u16 *)v_r8;
  v_r6 = 0;
  if (t23 != 0xffff)
    goto L_02008734;
L_0200878e:
  if (v_r6 == 0)
    goto L_020087da;
  *(s32 *)(v_r7 + 0xcbc) = v_r6;
  Call1((void (*)())Func_0200053c, 1);
L_020087a2:
  t25 = FIELD_AT(v_r6, s32, 8);
  t26 = FIELD_AT(v_r6, s32, 16);
  t27 = Value3((s32 (*)())Map_GetTerrainHeight, 0, t25, t26);
  t28 = FIELD_AT(v_r6, s32, 8);
  t29 = FIELD_AT(v_r6, s32, 16);
  v_r5 = ((t27 >> 19) - 2);
  Call4((void (*)())Func_020010cc, t28, t29, 0, v_r5);
  t31 = FIELD_AT(v_r6, s32, 8);
  t32 = FIELD_AT(v_r6, s32, 16);
  Call4((void (*)())Func_020010cc, t31, t32, 2, v_r5);
  t34 = FIELD_AT(v_r6, s32, 8);
  t35 = FIELD_AT(v_r6, s32, 16);
  t36 = FIELD_AT(v_r6, s32, 76);
  Call4((void (*)())Func_020003f0, 0, (t34 >> 20), (t35 >> 20), t36);
L_020087da:
  return;
}
