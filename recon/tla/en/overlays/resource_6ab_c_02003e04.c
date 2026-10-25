// Draft of overlay 6ab's Func_02003e04, translated from its listing: differ
// 108 of 169. Left: its shape: 161 instructions against the listing's 169.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_WaitFrames();
s32 Func_0200363c();
s32 Func_020041fc();
s32 Func_02004264();
s32 Func_0200426c();
s32 Func_020042dc();
s32 Func_020042e4();
s32 Func_020043bc();
s32 Func_0200444c();
s32 Func_0200446c();
s32 Math_Cosine();
s32 Math_Sine();
s32 Object_GetById();
s32 Object_SetModeById();
s32 Random16Far();
s32 WaitFrames();
extern u8 Effect_AdvanceMotion[];
extern u8 gPartyState[];

void Func_02003e04(void) {
  s32 sp_[1];
  s32 v_r5, v_r10, v_r7, v_r8, t0, t1, t7, t12, t13, t14;
  s32 t15, t16, t17, t18, t19, t20, t21, t22, t23, t26;
  s32 t28, t33, t36;

  t0 = *(s32 *)((s32)gPartyState + 0x214);
  t1 = Value1((s32 (*)())Object_GetById, t0);
  Func_020042dc();
  Call1((void (*)())Func_0200444c, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_020041fc();
  Call1((void (*)())WaitFrames, 1);
  FIELD_AT(t1, s32, 12) = 0x820000;
  FIELD_AT(t1, s32, 72) = 0x8000;
  FIELD_AT(t1, s32, 68) = 0;
  v_r5 = (t1 + 85);
  *(u8 *)v_r5 = 0;
  t7 = FIELD_AT(0x3000000, s32, 108);
  *(s32 *)(t7 + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())Func_0200446c, 204);
  *(u8 *)v_r5 = 3;
  Call1((void (*)())FieldEvent_WaitFrames, 24);
  FIELD_AT(sp_, s32, 32) = 7;
  FIELD_AT(sp_, s32, 64) = (s32)Effect_AdvanceMotion;
  FIELD_AT(sp_, s32, 36) = 0xa3d7;
  FIELD_AT(sp_, s32, 40) = 0xa3d7;
  v_r8 = ((s32)sp_ + 28);
  v_r7 = t1;
  v_r10 = 0;
L_0200be9e:
  v_r5 = (v_r10 << 12);
  t12 = Value1((s32 (*)())Math_Cosine, v_r5);
  FIELD_AT(sp_, s32, 16) = t12;
  FIELD_AT(sp_, s32, 20) = 0;
  t13 = Value2((s32 (*)())Math_Sine, v_r5, 0);
  t14 = FIELD_AT(sp_, s32, 16);
  FIELD_AT(sp_, s32, 24) = t13;
  FIELD_AT(sp_, s32, 16) = (t14 + (t14 >> 1));
  t15 = Value0((s32 (*)())Random16Far);
  t16 = FIELD_AT(sp_, s32, 16);
  FIELD_AT(sp_, s32, 16) =
      ((t16 + ((u32)(((t15 << 1) + t15) << 14) >> 16)) + -0x6000);
  t17 = Value0((s32 (*)())Random16Far);
  t18 = FIELD_AT(sp_, s32, 24);
  t19 = FIELD_AT(sp_, s32, 20);
  v_r5 = ((t18 + ((u32)(((t17 << 1) + t17) << 13) >> 16)) + -0x3000);
  FIELD_AT(sp_, s32, 24) = v_r5;
  t20 = FIELD_AT(v_r7, s32, 16);
  t21 = FIELD_AT(sp_, s32, 16);
  t22 = FIELD_AT(v_r7, s32, 8);
  t23 = FIELD_AT(v_r7, s32, 12);
  Func_0200363c(t22, t23, t20, t21, v_r8, v_r5, v_r8, v_r8);
  v_r10 = (v_r10 + 1);
  if ((u32)v_r10 <= (u32)16)
    goto L_0200be9e;
  Call1((void (*)())Func_0200446c, 188);
  v_r5 = ((s32)gPartyState + 0x214);
  t26 = *(s32 *)v_r5;
  Call2((void (*)())Func_020043bc, t26, 0x101);
  t28 = *(s32 *)v_r5;
  Call2((void (*)())Object_SetModeById, t28, 49);
  Call3((void (*)())Func_02004264, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02004264, -1, -1, 0xe666);
  Func_0200426c();
  t33 = *(s32 *)v_r5;
  Call2((void (*)())Func_020043bc, t33, 0x100);
  FIELD_AT(v_r7, s32, 72) = 0x10000;
  FIELD_AT(v_r7, s32, 68) = 0x4000;
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  t36 = *(s32 *)v_r5;
  Call2((void (*)())Object_SetModeById, t36, 1);
  Func_020042e4();
  return;
}
