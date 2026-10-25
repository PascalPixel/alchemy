// Draft of overlay 6b5's Func_02000fbc, translated from its listing: differ
// 117 of 153. Left: its shape: 148 instructions against the listing's 153.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02001a14();
s32 Func_02001a1c();
s32 Func_02001a64();
s32 Func_02001a74();
s32 Func_02001ab4();
s32 Func_02001ad4();
s32 Func_02001adc();
s32 ObjectMotion_SetAngleToward();
s32 RenderOutput_PrepareForRedraw();
s32 UiText_DrawNumber();
s32 UiWindow_Create();
s32 UiWork_Finalize();
s32 WaitFrames();
extern u8 MsgComeBackWhen[];
extern u8 MsgYouHaveA[];
extern u8 gData_02001b02[];
extern u8 gData_02003860[];
extern u8 gInput[];
extern u8 gPartyState[];

void Func_02000fbc(s32 p0) {
  s32 sp_[1];
  s32 v_r5, v_r8, v_r9, v_r10, v_r7, v_r1, v_r6, t2, t6, t8;
  s32 t11, t12, t13, t14, t15, t16, t17, t18, t21, t28;

  Func_02001a14();
  Func_02001ab4(0, 0, 0);
  t2 = *(s32 *)((s32)gPartyState + 0x214);
  ObjectMotion_SetAngleToward(p0, t2, 0);
  Func_02001a64((s32)MsgYouHaveA);
  Func_02001a74(p0, 0);
  t6 = *(s32 *)(s32)gInput;
  v_r9 = 0;
  v_r8 = p0;
  v_r5 = 0;
  if ((t6 & 0x248) != 0x248)
    goto L_0200900a;
  v_r9 = 1;
L_0200900a:
  Func_02001ad4(78);
L_02009010:
  t8 = UiWindow_Create(12, 8, 6, 3, 3);
  v_r7 = t8;
  v_r10 = -1;
L_02009028:
  if (v_r5 == v_r10)
    goto L_02009042;
  RenderOutput_PrepareForRedraw(v_r7);
  FIELD_AT(sp_, s32, 0) = 0;
  UiText_DrawNumber(v_r5, 3, v_r7);
  v_r10 = v_r5;
L_02009042:
  t11 = FIELD_AT((s32)gInput, s32, 12);
  v_r6 = 0;
  v_r1 = (s32)gInput;
  if ((t11 & 32) == v_r6)
    goto L_02009052;
  v_r6 = (v_r6 - 1);
L_02009052:
  t12 = FIELD_AT(v_r1, s32, 12);
  if ((t12 & 16) == 0)
    goto L_0200906e;
  v_r6 = 1;
  goto L_0200906e;
L_02009060:
  t13 = *(u16 *)((s32)gData_02001b02 + (v_r5 << 1));
  t14 = Func_02001adc(t13, v_r1, (s32)gData_02001b02, (v_r5 << 1));
  if (t14 != 0)
    goto L_020090a0;
L_0200906e:
  if (v_r6 == 0)
    goto L_020090a0;
  v_r5 = (v_r5 + v_r6);
  if (v_r5 >= 0)
    goto L_0200907a;
  v_r5 = 96;
L_0200907a:
  if ((u32)v_r5 <= (u32)96)
    goto L_02009080;
  v_r5 = 0;
L_02009080:
  if (v_r9 != 0)
    goto L_020090a0;
  t15 = *(u8 *)((s32)gPartyState + 0x22f);
  if (t15 != 0)
    goto L_020090a0;
  t16 = *(s16 *)(s32)gData_02003860;
  if (t16 == 0)
    goto L_02009060;
L_020090a0:
  t17 = FIELD_AT((s32)gInput, s32, 4);
  v_r1 = (s32)gInput;
  if ((t17 & 1) != 0)
    goto L_020090be;
  t18 = FIELD_AT(v_r1, s32, 4);
  if ((t18 & 2) != 0)
    goto L_020090d6;
  WaitFrames(1);
  goto L_02009028;
L_020090be:
  UiWork_Finalize(v_r7, 1);
  if (v_r5 < 0)
    goto L_020090dc;
  t21 = *(u16 *)((s32)gData_02001b02 + (v_r5 << 1));
  Func_02001ad4(t21);
  goto L_02009010;
L_020090d6:
  v_r5 = -1;
  goto L_020090be;
L_020090dc:
  Func_02001ad4(78);
  WaitFrames(120);
  Func_02001ad4(0x2c5);
  Func_02001a64((s32)MsgComeBackWhen);
  Func_02001a74(v_r8, 0);
  t28 = WaitFrames(10);
  Func_02001a1c(t28, 0);
  return;
}
