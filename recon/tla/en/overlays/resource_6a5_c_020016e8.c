// Draft, differ 20 of 400 (was 58), the twin of Func_02001388's draft. The
// sixth Func_0200588c call passes a speed too (0xc000, in r3 and kept in sl
// for Func_020058ec(6, ...)). The speeds several calls share are locals (fast
// 0xa000, slow 0x2000, mid 0xc000), as the native keeps them in r6, r8 and sl
// while it rebuilds every coordinate; the other calls go through call.h's
// forwarding inlines, and Func_020058fc's through an inline of its own that
// calls it directly. Left, all sched2 ties: in Func_0200588c's calls the
// speed's copy into r3 comes after the coordinates' shifts (native: before
// r0), three later calls load their arguments in another order, and MsgShield
// loads before the byte store.
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
s32 Camera_MoveWithinMapBounds();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_020058c4();
s32 Func_020058d4();
s32 Actor_SetModeById();
s32 Func_020058fc();
s32 Actor_GetById();
s32 Actor_OffsetPositionAndResetMotion();
s32 Actor_WaitForMoveById();
s32 Camera_FollowActor();
s32 Actor_SetPositionAndReset();
s32 Motion_SetVarCbAndRefresh();
s32 FieldEvent_Wait();
s32 Func_020058ec();
s32 Actor_OffsetPositionAndReset();
s32 Actor_ArmCallback();
s32 Unit_AddHp();
s32 Party_SetFields1eeAnd1f0();
s32 Party_SetFields1f2And1f4();
s32 GameFlag_ClearBit();
s32 Func_0200593c();
extern u8 MsgAlhafra19[];
extern u8 gAdventure[];
extern u8 MsgShield[];

static __inline__ void W_020058fc(s32 a, s32 b, s32 c) {
  Func_020058fc(a, b, c);
}

void Func_020016e8(void) {
  s32 fast;
  s32 slow;
  s32 mid;
  s32 t28;
  s32 t29;
  s32 t30;
  s32 t31;
  s32 t32;
  s32 t76;
  s32 t77;
  s32 t78;
  s32 t79;
  s32 t80;
  s32 t81;
  s32 t23;
  s32 t24;
  s32 t25;
  s32 t26;
  s32 t27;
  s32 t39;
  s32 t40;
  s32 t41;
  s32 t42;
  s32 t43;

  Func_02005814();
  Call1(Func_02005984, 0);
  fast = 0xa000;
  Call4(Func_0200588c, 4, 0x2000000, 0x1380000, fast);
  Call4(Func_0200588c, 7, 0x2100000, 0x1300000, fast);
  slow = 0x2000;
  Call4(Func_0200588c, 0x1b, 0x1f80000, 0x1300000, slow);
  Call4(Func_0200588c, 0x1c, 0x2080000, 0x1200000, slow);
  Call4(Func_0200588c, 5, 0x2100000, 0x1400000, fast);
  mid = 0xc000;
  Call4(Func_0200588c, 6, 0x2200000, 0x1800000, mid);
  Call4(Camera_MoveWithinMapBounds, 0x2080000, 0x680000, 0x1380000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1(Func_020058c4, (s32)MsgAlhafra19);
  Call3(Func_020058d4, 6, 0, 0);
  Call2(Actor_SetModeById, 0x1b, 9);
  Call2(Actor_SetModeById, 0x1c, 8);
  Call2(Actor_SetModeById, 4, 0x32);
  Call2(Actor_SetModeById, 7, 0xc);
  Call2(Actor_SetModeById, 5, 0x1b);
  W_020058fc(4, 0x100, 0);
  W_020058fc(7, 0x100, 0);
  W_020058fc(5, 0x100, 0);
  W_020058fc(0x1c, 0x100, 0);
  W_020058fc(0x1b, 0x100, 0x1e);
  t23 = Value1(Actor_GetById, 4);
  FIELD_AT((t23 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t23 + 0x5a), u8, 0));
  t24 = Value1(Actor_GetById, 7);
  FIELD_AT((t24 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t24 + 0x5a), u8, 0));
  t25 = Value1(Actor_GetById, 5);
  FIELD_AT((t25 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t25 + 0x5a), u8, 0));
  t26 = Value1(Actor_GetById, 0x1b);
  FIELD_AT((t26 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t26 + 0x5a), u8, 0));
  t27 = Value1(Actor_GetById, 0x1c);
  FIELD_AT((t27 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t27 + 0x5a), u8, 0));
  t28 = Value1(Actor_GetById, 4);
  FIELD_AT(t28, s32, 40) = 0x30000;
  t29 = Value1(Actor_GetById, 7);
  FIELD_AT(t29, s32, 40) = 0x30000;
  t30 = Value1(Actor_GetById, 5);
  FIELD_AT(t30, s32, 40) = 0x30000;
  t31 = Value1(Actor_GetById, 0x1b);
  FIELD_AT(t31, s32, 40) = 0x30000;
  t32 = Value1(Actor_GetById, 0x1c);
  FIELD_AT(t32, s32, 40) = 0x30000;
  Call3(Actor_OffsetPositionAndResetMotion, 4, 8, 4);
  Call3(Actor_OffsetPositionAndResetMotion, 7, 8, 4);
  Call3(Actor_OffsetPositionAndResetMotion, 5, 8, 4);
  Call3(Actor_OffsetPositionAndResetMotion, 0x1b, -16, -8);
  Call3(Actor_OffsetPositionAndResetMotion, 0x1c, -16, -8);
  Call1(Actor_WaitForMoveById, 4);
  t39 = Value1(Actor_GetById, 4);
  FIELD_AT((t39 + 0x5a), u8, 0) = (FIELD_AT((t39 + 0x5a), u8, 0) | 1);
  t40 = Value1(Actor_GetById, 7);
  FIELD_AT((t40 + 0x5a), u8, 0) = (FIELD_AT((t40 + 0x5a), u8, 0) | 1);
  t41 = Value1(Actor_GetById, 5);
  FIELD_AT((t41 + 0x5a), u8, 0) = (FIELD_AT((t41 + 0x5a), u8, 0) | 1);
  t42 = Value1(Actor_GetById, 0x1b);
  FIELD_AT((t42 + 0x5a), u8, 0) = (FIELD_AT((t42 + 0x5a), u8, 0) | 1);
  t43 = Value1(Actor_GetById, 0x1c);
  FIELD_AT((t43 + 0x5a), u8, 0) = (1 | FIELD_AT((t43 + 0x5a), u8, 0));
  Call2(Camera_FollowActor, 4, 1);
  Call3(Actor_SetPositionAndReset, 6, 0x228, 0x158);
  W_020058fc(6, 0x102, 0x1e);
  Call3(Func_020058d4, 6, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 5, 2);
  Call1(FieldEvent_Wait, 5);
  Call3(Func_020058d4, 5, 0, 0);
  Func_020058fc(0x1c, 0x102, 0x1e);
  Call3(Func_020058d4, 0x1c, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 0x1b, 2);
  Call1(FieldEvent_Wait, 5);
  Call3(Func_020058d4, 0x1b, 0, 0);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_020058ec, 6, mid);
  Call1(FieldEvent_Wait, 0xa);
  Call3(Func_020058d4, 6, 0, 0);
  Call3(Actor_OffsetPositionAndReset, 6, 0, -24);
  Call1(Actor_WaitForMoveById, 6);
  Func_020058ec(4, slow);
  Call3(Actor_ArmCallback, 6, fast, 0);
  Call2(Actor_SetModeById, 4, 3);
  Call1(FieldEvent_Wait, 0x14);
  Call3(Actor_ArmCallback, 7, fast, 0);
  Call2(Func_020058ec, 4, fast);
  Call2(Func_020058ec, 5, fast);
  Call1(FieldEvent_Wait, 0xa);
  Call3(Actor_OffsetPositionAndResetMotion, 4, -16, -8);
  Call3(Actor_OffsetPositionAndResetMotion, 7, -16, -8);
  Call3(Actor_OffsetPositionAndResetMotion, 5, -16, -8);
  Call3(Actor_OffsetPositionAndResetMotion, 6, -16, -8);
  Call3(Actor_OffsetPositionAndResetMotion, 0x1b, 0x10, 8);
  Call3(Actor_OffsetPositionAndResetMotion, 0x1c, 0x10, 8);
  t76 = Value1(Actor_GetById, 4);
  FIELD_AT(t76, s32, 40) = 0x40000;
  t77 = Value1(Actor_GetById, 7);
  FIELD_AT(t77, s32, 40) = 0x40000;
  t78 = Value1(Actor_GetById, 5);
  FIELD_AT(t78, s32, 40) = 0x40000;
  t79 = Value1(Actor_GetById, 6);
  FIELD_AT(t79, s32, 40) = 0x40000;
  t80 = Value1(Actor_GetById, 0x1b);
  FIELD_AT(t80, s32, 40) = 0x40000;
  t81 = Value1(Actor_GetById, 0x1c);
  FIELD_AT(t81, s32, 40) = 0x40000;
  Call1(FieldEvent_Wait, 4);
  if (FIELD_AT(gAdventure, s16, 598) == -1) {
    Call2(Unit_AddHp, 4, -10);
  }
  FIELD_AT(gAdventure, u8, 587) = 2;
  Call2(Party_SetFields1eeAnd1f0, (s32)MsgShield, 0xc);
  Call2(Party_SetFields1f2And1f4, (s32)MsgShield, 0xd);
  Call1(GameFlag_ClearBit, 0x11b);
  Call2(Func_0200593c, 0xd, 7);
}
