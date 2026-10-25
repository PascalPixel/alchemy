// Draft of overlay 6ab's Func_020005b0, translated from its listing: differ
// 32 of 135. Left: its shape: 131 instructions against the listing's 135.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 FieldEvent_WaitFrames();
s32 Func_02000d88();
s32 Func_02004264();
s32 Func_020042dc();
s32 Func_0200432c();
s32 Func_020043bc();
s32 Func_020043e4();
s32 Func_0200444c();
s32 Func_0200446c();
s32 ObjectMotion_Launch();
s32 ObjectMotion_OffsetPositionAndResetMotion();
s32 ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
s32 Object_SetModeById();
s32 Party_SetFields1eeAnd1f0();
extern u8 gPartyState[];

void Func_020005b0(void) {
  s32 v_r5, t5, t6, t7, t8, t10, t12, t14, t16, t18;

  Func_020042dc();
  Call1((void (*)())Func_0200444c, 0);
  Func_02000d88();
  Call1((void (*)())Func_0200446c, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  v_r5 = ((s32)gPartyState + 0x214);
  t5 = *(s32 *)v_r5;
  t6 = Value1((s32 (*)())Object_GetById, t5);
  t7 = *(u8 *)(t6 + 90);
  *(u8 *)(t6 + 90) = (254 & t7);
  t8 = *(s32 *)v_r5;
  Call2((void (*)())Func_020043bc, t8, 0x102);
  t10 = *(s32 *)v_r5;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, t10, 0x19999, 0xcccc);
  t12 = *(s32 *)v_r5;
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, t12, 0, 8);
  t14 = *(s32 *)v_r5;
  Call3((void (*)())ObjectMotion_Launch, t14, 4, 10);
  t16 = *(s32 *)v_r5;
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, t16, 0, 16);
  t18 = *(s32 *)v_r5;
  Call3((void (*)())ObjectMotion_Launch, t18, 6, 20);
  Call1((void (*)())Func_0200446c, 53);
  Call2((void (*)())Object_SetModeById, 11, 2);
  Call2((void (*)())Object_SetModeById, 12, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 24);
  Call3((void (*)())Func_0200432c, 11, 0x2b80000, 0xe80000);
  Call3((void (*)())Func_0200432c, 12, 0x2980000, 0xe80000);
  Call2((void (*)())Object_SetModeById, 11, 3);
  Call2((void (*)())Object_SetModeById, 12, 3);
  Call1((void (*)())Func_0200446c, 107);
  Call3((void (*)())Func_02004264, 0x20000, 0x40000, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())Func_0200446c, 0x186);
  Call3((void (*)())Func_02004264, 0x10000, 0x20000, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_02004264, -1, -1, 0xe666);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call1((void (*)())Func_0200446c, 148);
  Call2((void (*)())Object_SetModeById, 11, 4);
  Call2((void (*)())Object_SetModeById, 12, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  *(u8 *)((s32)gPartyState + 0x24b) = 2;
  Call2((void (*)())Party_SetFields1eeAnd1f0, 0x11e, 20);
  *(u16 *)((s32)gPartyState + 0x1f6) = 104;
  Call2((void (*)())Func_020043e4, 102, 0);
  return;
}
