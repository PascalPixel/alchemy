// Draft of overlay 6ac's Func_02002870, translated from its listing: differ
// 170 of 168. Left: its shape: 164 instructions against the listing's 168.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02003120();
s32 Func_02003128();
s32 Math_Cosine();
s32 Math_Sine();
s32 Actor_SetSingleChildFlags();
s32 Random16Far();
s32 ResourceMetadata_ClearRecord();
s32 Resource_ResetEntry();
extern u8 IwramMulQ16[];
extern u8 gData_020023c4[];
extern u8 gData_02003304[];

void Func_02002870(s32 p0) {
  s32 sp_[1];
  s32 v_r11, v_r5, v_r8, v_r6, v_r7, t0, t1, t2, t3, t4;
  s32 t5, t6, t7, t8, t9, t10, t11, t12, t13, t16;
  s32 t17, t18, t19, t20, t21, t22, t23, t25, t26, t27;
  s32 t29, t30, t31, t32, t33, t34, t35, t36, t37;

  t0 = FIELD_AT(((s32)gData_020023c4 + 0x188), s32, 112);
  v_r11 = p0;
  if (t0 != 0)
    goto L_0200a88c;
  goto L_0200a9e4;
L_0200a88c:
  *(s32 *)sp_ = 0;
L_0200a890:
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
  t9 = Value4((s32 (*)())Func_02003128, 0x1e9, v_r8, v_r6,
              ((t7 + v_r5) - (t6 << 4)));
  v_r7 = t9;
  if (v_r7 == 0)
    goto L_0200a9c4;
  t10 = Value0((s32 (*)())Random16Far);
  t11 = Value0((s32 (*)())Random16Far);
  t12 = Value0((s32 (*)())Random16Far);
  t13 = Value0((s32 (*)())Random16Far);
  Call2((void (*)())Func_02003120, v_r7, (s32)gData_02003304);
  Call2((void (*)())Actor_SetSingleChildFlags, v_r7, 0);
  FIELD_AT(v_r7, s32, 40) = ((t10 << 2) + 0x20000);
  t16 = Value1((s32 (*)())Math_Cosine, t10);
  v_r6 = (t11 << 3);
  t17 = ((s32 (*)())(s32)IwramMulQ16)(v_r6, t16);
  FIELD_AT(v_r7, s32, 44) = t17;
  t18 = Value1((s32 (*)())Math_Sine, t10);
  t19 = ((s32 (*)())(s32)IwramMulQ16)(v_r6, t18);
  FIELD_AT(v_r7, s32, 72) = 0x547a;
  t20 = FIELD_AT(((s32)gData_020023c4 + 0x188), s32, 112);
  v_r5 = (((u32)(t12 + t13) >> 2) + 0x8000);
  FIELD_AT(v_r7, s32, 24) = v_r5;
  FIELD_AT(v_r7, s32, 28) = v_r5;
  FIELD_AT(v_r7, s32, 68) = 0x8000;
  t21 = FIELD_AT(v_r7, s32, 80);
  FIELD_AT(v_r7, s32, 36) = t19;
  FIELD_AT(v_r7, s32, 52) = 0;
  t22 = FIELD_AT(t20, s32, 80);
  t23 = FIELD_AT(t21, u8, 16);
  Call1((void (*)())Resource_ResetEntry, t23);
  t25 = FIELD_AT(t21, u8, 17);
  FIELD_AT(t21, u8, 17) = (t25 | 1);
  t26 = FIELD_AT(((s32)gData_020023c4 + 0x188), u16, 12);
  t27 = FIELD_AT(t21, s32, 40);
  FIELD_AT(t21, u8, 16) = t26;
  Call1((void (*)())ResourceMetadata_ClearRecord, t27);
  FIELD_AT(t21, s32, 40) = 0;
  FIELD_AT(t21, u8, 27) = 0;
  t29 = FIELD_AT(t22, u8, 20);
  t30 = FIELD_AT(t21, u8, 5);
  FIELD_AT(t21, u8, 20) = t29;
  t31 = FIELD_AT(t22, u8, 21);
  FIELD_AT(t21, u8, 21) = t31;
  t32 = FIELD_AT(t22, u8, 5);
  FIELD_AT(t21, u8, 5) = ((63 & t30) | (((u32)t32 >> 6) << 6));
  t33 = FIELD_AT(t22, u8, 7);
  t34 = FIELD_AT(t21, u8, 7);
  FIELD_AT(t21, u8, 7) = ((63 & t34) | (((u32)t33 >> 6) << 6));
  t35 = FIELD_AT(t22, u16, 8);
  t36 = FIELD_AT(t21, u16, 8);
  FIELD_AT(t21, u16, 8) = ((t36 & -0x400) | ((u32)(t35 << 22) >> 22));
L_0200a9c4:
  t37 = *(s32 *)sp_;
  *(s32 *)sp_ = (t37 - 1);
  if ((t37 - 1) < 0)
    goto L_0200a9d0;
  goto L_0200a890;
L_0200a9d0:
  goto L_0200a9e4;
L_0200a9e4:
  return;
}
