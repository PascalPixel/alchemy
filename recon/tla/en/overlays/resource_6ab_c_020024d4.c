// Draft of overlay 6ab's Func_020024d4, translated from its listing: differ
// 187 of 168. Left: its shape: 163 instructions against the listing's 168.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020041dc();
s32 Func_020041e4();
s32 Math_Cosine();
s32 Math_Sine();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Random16Far();
s32 ResourceMetadata_ClearRecord();
s32 Resource_ResetEntry();
extern u8 IwramMulQ16[];
extern u8 gData_020023c4[];
extern u8 gData_02004774[];

void Func_020024d4(s32 p0) {
  s32 sp_[1];
  s32 v_r11, v_r5, v_r8, v_r6, v_r7, t0, t1, t2, t3, t4;
  s32 t5, t6, t7, t8, t9, t10, t11, t12, t13, t16;
  s32 t18, t19, t20, t21, t23, t24, t25, t27, t28, t29;
  s32 t30, t31, t32, t33, t34, t35;

  t0 = FIELD_AT(((s32)gData_020023c4 + 0x188), s32, 112);
  v_r11 = p0;
  if (t0 != 0)
    goto L_0200a4f0;
  goto L_0200a648;
L_0200a4f0:
  FIELD_AT(sp_, s32, 0) = 0;
L_0200a4f4:
  t1 = Value0((s32 (*)())Random16Far);
  t2 = Value0((s32 (*)())Random16Far);
  t3 = FIELD_AT(v_r11, s32, 8);
  v_r5 = (t1 << 4);
  v_r8 = (t3 + v_r5);
  t4 = Value0((s32 (*)())Random16Far);
  t5 = Value0((s32 (*)())Random16Far);
  t6 = Value0((s32 (*)())Random16Far);
  t7 = FIELD_AT(v_r11, s32, 16);
  t8 = FIELD_AT(v_r11, s32, 12);
  v_r5 = (t5 << 4);
  v_r8 = (v_r8 - (t2 << 4));
  v_r6 = (((t4 << 3) + t8) + 0x40000);
  t9 = Value4((s32 (*)())Func_020041e4, 0x1e9, v_r8, v_r6,
              ((t7 + v_r5) - (t6 << 4)));
  v_r7 = t9;
  if (v_r7 == 0)
    goto L_0200a628;
  t10 = Value0((s32 (*)())Random16Far);
  t11 = Value0((s32 (*)())Random16Far);
  t12 = Value0((s32 (*)())Random16Far);
  t13 = Value0((s32 (*)())Random16Far);
  Call2((void (*)())Func_020041dc, v_r7, (s32)gData_02004774);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v_r7, 0);
  FIELD_AT(v_r7, s32, 40) = ((t10 << 2) + 0x20000);
  t16 = Value1((s32 (*)())Math_Cosine, t10);
  v_r6 = (t11 << 3);
  FIELD_AT(v_r7, s32, 44) = v_r6;
  Call2((void (*)())Math_Sine, t10, t16);
  FIELD_AT(v_r7, s32, 72) = 0x547a;
  t18 = FIELD_AT(((s32)gData_020023c4 + 0x188), s32, 112);
  v_r5 = (((u32)(t12 + t13) >> 2) + 0x8000);
  FIELD_AT(v_r7, s32, 24) = v_r5;
  FIELD_AT(v_r7, s32, 28) = v_r5;
  FIELD_AT(v_r7, s32, 68) = 0x8000;
  t19 = FIELD_AT(v_r7, s32, 80);
  FIELD_AT(v_r7, s32, 36) = v_r6;
  FIELD_AT(v_r7, s32, 52) = 0;
  t20 = FIELD_AT(t18, s32, 80);
  t21 = FIELD_AT(t19, u8, 16);
  Call1((void (*)())Resource_ResetEntry, t21);
  t23 = FIELD_AT(t19, u8, 17);
  FIELD_AT(t19, u8, 17) = (t23 | 1);
  t24 = FIELD_AT(((s32)gData_020023c4 + 0x188), u16, 12);
  t25 = FIELD_AT(t19, s32, 40);
  FIELD_AT(t19, u8, 16) = t24;
  Call1((void (*)())ResourceMetadata_ClearRecord, t25);
  FIELD_AT(t19, s32, 40) = 0;
  FIELD_AT(t19, u8, 27) = 0;
  t27 = FIELD_AT(t20, u8, 20);
  t28 = FIELD_AT(t19, u8, 5);
  FIELD_AT(t19, u8, 20) = t27;
  t29 = FIELD_AT(t20, u8, 21);
  FIELD_AT(t19, u8, 21) = t29;
  t30 = FIELD_AT(t20, u8, 5);
  FIELD_AT(t19, u8, 5) = ((63 & t28) | (((u32)t30 >> 6) << 6));
  t31 = FIELD_AT(t20, u8, 7);
  t32 = FIELD_AT(t19, u8, 7);
  FIELD_AT(t19, u8, 7) = ((63 & t32) | (((u32)t31 >> 6) << 6));
  t33 = FIELD_AT(t20, u16, 8);
  t34 = FIELD_AT(t19, u16, 8);
  FIELD_AT(t19, u16, 8) = ((t34 & -0x400) | ((u32)(t33 << 22) >> 22));
L_0200a628:
  t35 = FIELD_AT(sp_, s32, 0);
  FIELD_AT(sp_, s32, 0) = (t35 - 1);
  if ((t35 - 1) < 0)
    goto L_0200a634;
  goto L_0200a4f4;
L_0200a634:
  goto L_0200a648;
L_0200a648:
  return;
}
