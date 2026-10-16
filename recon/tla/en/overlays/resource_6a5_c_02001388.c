#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02005814();
s32 Func_02005984();
s32 Func_0200588c();
s32 Object_SetModeById();
s32 Motion_CamBounds();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_020058c4();
s32 Func_020058d4();
s32 Func_020058fc();
s32 Object_GetById();
s32 ObjectMotion_OffsetPositionAndResetMotion();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 Object_AttachWorkTargetToObject();
s32 ObjectMotion_SetPositionAndReset();
s32 Func_020058ec();
s32 Battle_WaitMode0();
s32 ObjectMotion_ArmCallback();
s32 Func_020058dc();
s32 ObjectMotion_OffsetPositionAndReset();
s32 Unit_AddHp();
s32 Party_SetFields1eeAnd1f0();
s32 Party_SetFields1f2And1f4();
s32 Func_0200593c();
extern u8 MsgAlhafra19[];
extern u8 gPartyState[];
extern u8 MsgShield[];

void Func_02001388(void) {
  s32 t24;
  s32 t25;
  s32 t26;
  s32 t27;
  s32 t66;
  s32 t67;
  s32 t68;
  s32 t69;
  s32 t70;
  s32 t20;
  s32 t21;
  s32 t22;
  s32 t23;
  s32 t33;
  s32 t34;
  s32 t35;
  s32 t36;

  Func_02005814();
  Call1(Func_02005984, 0);
  Func_0200588c(4, 0x2000000, 0x1380000, 0xa000);
  Call4(Func_0200588c, 7, 0x2100000, 0x1300000, 0xa000);
  Func_0200588c(0x1b, 0x1f80000, 0x1300000, 0x2000);
  Call4(Func_0200588c, 0x1c, 0x2080000, 0x1200000, 0x2000);
  Call4(Func_0200588c, 5, 0x2280000, 0x1880000, 0xc000);
  Call2(Object_SetModeById, 0x1b, 9);
  Call2(Object_SetModeById, 0x1c, 8);
  Call2(Object_SetModeById, 4, 0x32);
  Call2(Object_SetModeById, 7, 0xc);
  Call4(Motion_CamBounds, 0x2080000, 0x680000, 0x1380000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1(Func_020058c4, (s32)MsgAlhafra19);
  Call3(Func_020058d4, 5, 0, 0);
  Call3(Func_020058fc, 4, 0x100, 0);
  Call3(Func_020058fc, 7, 0x100, 0);
  Call3(Func_020058fc, 0x1c, 0x100, 0);
  Call3(Func_020058fc, 0x1b, 0x100, 0x1e);
  t20 = Value1(Object_GetById, 4);
  FIELD_AT((t20 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t20 + 0x5a), u8, 0));
  t21 = Value1(Object_GetById, 7);
  FIELD_AT((t21 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t21 + 0x5a), u8, 0));
  t22 = Value1(Object_GetById, 0x1b);
  FIELD_AT((t22 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t22 + 0x5a), u8, 0));
  t23 = Value1(Object_GetById, 0x1c);
  FIELD_AT((t23 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t23 + 0x5a), u8, 0));
  t24 = Value1(Object_GetById, 4);
  FIELD_AT(t24, s32, 40) = 0x30000;
  t25 = Value1(Object_GetById, 7);
  FIELD_AT(t25, s32, 40) = 0x30000;
  t26 = Value1(Object_GetById, 0x1b);
  FIELD_AT(t26, s32, 40) = 0x30000;
  t27 = Value1(Object_GetById, 0x1c);
  FIELD_AT(t27, s32, 40) = 0x30000;
  Call3(ObjectMotion_OffsetPositionAndResetMotion, 4, 8, 4);
  Call3(ObjectMotion_OffsetPositionAndResetMotion, 7, 8, 4);
  Call3(ObjectMotion_OffsetPositionAndResetMotion, 0x1b, -16, -8);
  Call3(ObjectMotion_OffsetPositionAndResetMotion, 0x1c, -16, -8);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 4);
  t33 = Value1(Object_GetById, 4);
  FIELD_AT((t33 + 0x5a), u8, 0) = (FIELD_AT((t33 + 0x5a), u8, 0) | 1);
  t34 = Value1(Object_GetById, 7);
  FIELD_AT((t34 + 0x5a), u8, 0) = (FIELD_AT((t34 + 0x5a), u8, 0) | 1);
  t35 = Value1(Object_GetById, 0x1b);
  FIELD_AT((t35 + 0x5a), u8, 0) = (FIELD_AT((t35 + 0x5a), u8, 0) | 1);
  t36 = Value1(Object_GetById, 0x1c);
  FIELD_AT((t36 + 0x5a), u8, 0) = (1 | FIELD_AT((t36 + 0x5a), u8, 0));
  Call2(Object_AttachWorkTargetToObject, 4, 1);
  Call3(ObjectMotion_SetPositionAndReset, 5, 0x228, 0x158);
  Func_020058fc(5, 0x102, 0x1e);
  Call3(Func_020058d4, 5, 0, 0);
  Call2(Func_020058ec, 7, 0x4000);
  Call1(Battle_WaitMode0, 0xa);
  Call3(Func_020058d4, 7, 0, 0);
  Call2(Func_020058ec, 0x1c, 0x6000);
  Func_020058fc(0x1c, 0x107, 0x1e);
  Call3(Func_020058d4, 0x1c, 0, 0);
  Call3(Func_020058d4, 0x1b, 0, 0);
  ObjectMotion_ArmCallback(0x1c, 0x2000, 0);
  Func_020058ec(0x1b, 0x2000);
  Call2(Func_020058ec, 5, 0xa000);
  Call1(Battle_WaitMode0, 0xa);
  Call2(Func_020058dc, 5, 0);
  Call3(ObjectMotion_OffsetPositionAndReset, 5, -8, -24);
  Call2(Func_020058ec, 4, 0x2000);
  Call2(Object_SetModeById, 4, 3);
  Call1(Battle_WaitMode0, 0x14);
  Call3(ObjectMotion_ArmCallback, 7, 0xa000, 0);
  Call2(Func_020058ec, 4, 0xa000);
  Call2(Func_020058ec, 5, 0xa000);
  Call1(Battle_WaitMode0, 0xa);
  Call3(ObjectMotion_OffsetPositionAndResetMotion, 4, -16, -8);
  Call3(ObjectMotion_OffsetPositionAndResetMotion, 7, -16, -8);
  Call3(ObjectMotion_OffsetPositionAndResetMotion, 5, -16, -8);
  Call3(ObjectMotion_OffsetPositionAndResetMotion, 0x1b, 0x10, 8);
  Call3(ObjectMotion_OffsetPositionAndResetMotion, 0x1c, 0x10, 8);
  t66 = Value1(Object_GetById, 4);
  FIELD_AT(t66, s32, 40) = 0x40000;
  t67 = Value1(Object_GetById, 7);
  FIELD_AT(t67, s32, 40) = 0x40000;
  t68 = Value1(Object_GetById, 5);
  FIELD_AT(t68, s32, 40) = 0x40000;
  t69 = Value1(Object_GetById, 0x1b);
  FIELD_AT(t69, s32, 40) = 0x40000;
  t70 = Value1(Object_GetById, 0x1c);
  FIELD_AT(t70, s32, 40) = 0x40000;
  Call1(Battle_WaitMode0, 4);
  if (FIELD_AT(gPartyState, s16, 598) == -1) {
    Call2(Unit_AddHp, 4, -10);
  }
  FIELD_AT(gPartyState, u8, 587) = 2;
  Call2(Party_SetFields1eeAnd1f0, (s32)MsgShield, 0xb);
  Call2(Party_SetFields1f2And1f4, (s32)MsgShield, 0xb);
  Call2(Func_0200593c, 0xd, 6);
}
