// Draft of overlay 6a5's Func_02000cd0, translated from its listing: differ
// 105 of 691. The first camera move goes through Call4 (as a direct call CSE
// shares its -1 with later calls) and the two walks share their facing
// 0xc000 through a variable, as the native keeps it in r5. Left: its shape (696
// instructions against the listing's 691), and several calls whose constant
// arguments load in another order (Actor_SetModeById's r0 after r1 here,
// whether called directly, through Call2, prototyped or not).
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 Event_AskYesNoWithActorAnimation();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_WaitFrames();
s32 Func_0200577c();
s32 Func_020057ac();
s32 Func_020057b4();
s32 Func_020057ec();
s32 Func_02005814();
s32 Func_0200588c();
s32 Func_020058c4();
s32 Func_020058d4();
s32 Func_020058ec();
s32 Func_020058fc();
s32 Func_02005904();
s32 Func_02005914();
s32 Func_02005924();
s32 Func_0200592c();
s32 Func_0200593c();
s32 Func_02005984();
s32 Func_020059f4();
s32 GameFlag_SetBit();
s32 Motion_SetVarCbAndRefresh();
s32 Actor_ArmCallback();
s32 Actor_WaitForMoveById();
s32 Actor_CommitPositionAndActivate();
s32 Actor_OffsetPositionAndReset();
s32 Actor_OffsetPositionAndResetMotion();
s32 Actor_StartRepeatedMotion();
s32 Actor_GetById();
s32 Actor_SetModeById();
s32 PartyInventory_Add();
s32 Party_SetFields1eeAnd1f0();
s32 Party_SetFields1f2And1f4();
s32 FieldMessage_ShowAtActor();
s32 FieldMessage_Show();
s32 Unit_Get();
s32 WaitFrames();
extern u8 MsgWeHaveIgnited[];
extern u8 gAdventure[];

void Func_02000cd0(void) {
  s32 v_r7, v_r5, v_r6, t2, t4, t75, t77, t78, t79, t80;
  s32 t91, t92, t93, t96, t97, t101, t102, t105, t106, t112;
  s32 t113, t118, t119, t129, t135, t139, t140, t143, t144;
  s32 facing;

  Func_02005814();
  Call1((void (*)())Func_02005984, 0);
  t2 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t2, s32, 428) = 0x100;
  FIELD_AT(t2, s32, 436) = 120;
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  t4 = Value0((s32 (*)())Func_0200592c);
  FIELD_AT(t4, u8, 85) = 0;
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x2000000, 0x600000, 0xf00000,
        0);
  Call1((void (*)())WaitFrames, 1);
  Func_0200577c();
  Call1((void (*)())WaitFrames, 1);
  Call1((void (*)())Func_020058c4, (s32)MsgWeHaveIgnited);
  Call4((void (*)())Func_0200588c, 4, 0x1fc0000, 0x1160000, facing = 0xc000);
  Call4((void (*)())Func_0200588c, 7, 0x2140000, 0x1160000, facing);
  Call1((void (*)())Func_020059f4, 141);
  Call3((void (*)())Func_020057ac, 0x40000, 0x40000, 0x10000);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3((void (*)())Func_020057ac, 0x30000, 0x30000, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call3((void (*)())Func_020057ac, 0x20000, 0x20000, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call1((void (*)())Func_020059f4, 0x186);
  Call3((void (*)())Func_020057ac, -1, -1, 0xe666);
  Func_020057b4();
  Call2((void (*)())Func_02005914, 0x6666, 0xccc);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x2000000, 0x600000, 0x1280000,
        1);
  Func_02005924();
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Actor_SetModeById, 7, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Actor_SetModeById, 4, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Call2((void (*)())Func_020058ec, 4, 0x4000);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Actor_OffsetPositionAndReset, 4, 0, 8);
  Call3((void (*)())Actor_OffsetPositionAndReset, 7, 0, 8);
  Call3((void (*)())Actor_OffsetPositionAndReset, 27, 32, -16);
  Call3((void (*)())Actor_OffsetPositionAndReset, 28, 48, -16);
  Call1((void (*)())Actor_WaitForMoveById, 4);
  Call1((void (*)())Actor_WaitForMoveById, 7);
  Call1((void (*)())Actor_WaitForMoveById, 27);
  Call1((void (*)())Actor_WaitForMoveById, 28);
  Call3((void (*)())Actor_ArmCallback, 4, 0x2000, 0);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Call3((void (*)())Actor_ArmCallback, 27, 0xa000, 0);
  Call3((void (*)())Actor_ArmCallback, 28, 0xb000, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call1((void (*)())Func_020059f4, 78);
  Call3((void (*)())Func_020058d4, 7, 0, 0);
  Call2((void (*)())Actor_SetModeById, 28, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 33);
  Call1((void (*)())Func_020059f4, 34);
  Call3((void (*)())Func_020058d4, 0x101c, 0, 0);
  Func_020058fc(7, 0x100, 0);
  Call3((void (*)())Func_020058fc, 4, 0x100, 30);
  Call2((void (*)())Actor_SetModeById, 27, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 33);
  Call3((void (*)())Func_020058d4, 27, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
  Call2((void (*)())Func_020058ec, 7, 0x8000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Actor_SetModeById, 28, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 33);
  Call3((void (*)())Func_020058d4, 0x101c, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0x2000, 0);
  Call2((void (*)())Func_020058ec, 7, 0x4000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Actor_SetModeById, 27, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020058d4, 27, 0, 0);
  Func_020058fc(7, 0x102, 30);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 28, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call2((void (*)())FieldMessage_ShowAtActor, 0x101c, 0);
  t75 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0);
  v_r7 = 0x3000000;
  if (t75 != 0)
    goto L_02008fc4;
  Call3((void (*)())Func_020058d4, 0x101c, 0, 0);
  t77 = FIELD_AT(v_r7, s32, 108);
  t78 = FIELD_AT(t77, u16, 452);
  FIELD_AT(t77, u16, 452) = (t78 + 1);
  goto L_02008fe0;
L_02008fc4:
  t79 = FIELD_AT(v_r7, s32, 108);
  t80 = FIELD_AT(t79, u16, 452);
  FIELD_AT(t79, u16, 452) = (t80 + 1);
  Call3((void (*)())Func_020058d4, 0x101c, 0, 0);
L_02008fe0:
  Call3((void (*)())Func_020058fc, 7, 0x100, 0);
  Func_020058fc(4, 0x100, 30);
  Call3((void (*)())Actor_ArmCallback, 7, 0x8000, 0);
  Call2((void (*)())Func_020058ec, 4, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Actor_CommitPositionAndActivate, 28, -16, -0x18);
  Call3((void (*)())Actor_ArmCallback, 28, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0x4000, 0);
  Call3((void (*)())Func_020058fc, 7, 0x102, 0);
  t91 = Value1((s32 (*)())Actor_GetById, 28);
  t92 = FIELD_AT(t91, u8, 90);
  FIELD_AT(t91, u8, 90) = (254 & t92);
  t93 = Value1((s32 (*)())Actor_GetById, 28);
  FIELD_AT(t93, s32, 40) = 0x40000;
  Call3((void (*)())Actor_OffsetPositionAndResetMotion, 28, 0, 24);
  Call1((void (*)())Actor_WaitForMoveById, 28);
  t96 = Value1((s32 (*)())Actor_GetById, 28);
  t97 = FIELD_AT(t96, u8, 90);
  FIELD_AT(t96, u8, 90) = (1 | t97);
  Call3((void (*)())Actor_ArmCallback, 28, 0xa000, 0);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Call3((void (*)())Func_020058d4, 7, 0, 0);
  t101 = FIELD_AT(0x3000000, s32, 108);
  t102 = FIELD_AT(t101, u16, 452);
  FIELD_AT(t101, u16, 452) = (t102 + 1);
  Call2((void (*)())FieldMessage_Show, ((t102 << 16) >> 16),
        5);
  Call1((void (*)())PartyInventory_Add, 224);
  v_r6 = 216;
  v_r5 = 0;
  goto L_020090d4;
L_020090d0:
  v_r6 = (v_r6 + 2);
  v_r5 = (v_r5 + 1);
L_020090d4:
  if (v_r5 > 14)
    goto L_020090f8;
  t105 = Value1((s32 (*)())Unit_Get, 4);
  t106 = *(u16 *)(t105 + v_r6);
  if ((0x1ff & t106) != 224)
    goto L_020090d0;
  Call2((void (*)())Func_020057ec, 4, v_r5);
  goto L_020090f8;
L_020090f8:
  Call2((void (*)())Motion_SetVarCbAndRefresh, 4, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call3((void (*)())Actor_OffsetPositionAndReset, 4, 0, 8);
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, -0x18, -8);
  t112 = Value1((s32 (*)())Actor_GetById, 4);
  t113 = FIELD_AT(t112, u8, 90);
  FIELD_AT(t112, u8, 90) = (254 & t113);
  Call3((void (*)())Actor_OffsetPositionAndResetMotion, 4, 0, -8);
  Call3((void (*)())Func_020058fc, 28, 0x108, 30);
  Call3((void (*)())Func_020058d4, 28, 0, 0);
  Call1((void (*)())Actor_WaitForMoveById, 4);
  t118 = Value1((s32 (*)())Actor_GetById, 4);
  t119 = FIELD_AT(t118, u8, 90);
  v_r5 = (254 & t119);
  FIELD_AT(t118, u8, 90) = v_r5;
  Func_020058fc(4, 0x109, 30);
  Call2((void (*)())Func_020058ec, 7, 0x8000);
  Call3((void (*)())Func_020058d4, 7, 0, 0);
  Call3((void (*)())Func_020058fc, 27, 0x108, 30);
  Call3((void (*)())Actor_ArmCallback, 27, 0xd000, 0);
  Call3((void (*)())Func_020058d4, 0x201b, 0, 0);
  Call2((void (*)())Func_020058ec, 7, 0x4000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Func_02005904, 7, 0x102);
  t129 = FIELD_AT((s32)gAdventure, s32, 532);
  Call2((void (*)())Func_02005904, t129, 0x102);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Func_020058fc(28, 0x108, 30);
  Call3((void (*)())Actor_ArmCallback, 28, 0xb000, 0);
  FieldMessage_ShowAtActor(28, 0);
  t135 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0);
  if (t135 != 0)
    goto L_02009236;
  Call2((void (*)())Actor_SetModeById, 28, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020058d4, 28, 0, 0);
  t139 = FIELD_AT(0x3000000, s32, 108);
  t140 = FIELD_AT(t139, u16, 452);
  FIELD_AT(t139, u16, 452) = (t140 + 1);
  goto L_02009260;
L_02009236:
  Call2((void (*)())Actor_SetModeById, 28, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  t143 = FIELD_AT(0x3000000, s32, 108);
  t144 = FIELD_AT(t143, u16, 452);
  FIELD_AT(t143, u16, 452) = (t144 + 1);
  Call3((void (*)())Func_020058d4, 28, 0, 0);
L_02009260:
  Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call3((void (*)())Func_020058d4, 7, 0, 0);
  Func_020058fc(27, 0x109, 30);
  Call3((void (*)())Actor_ArmCallback, 27, 0xd000, 0);
  Call3((void (*)())Func_020058d4, 0x201b, 0, 0);
  Func_020058fc(4, 0x108, 30);
  Call3((void (*)())Actor_ArmCallback, 28, 0xd000, 0);
  Call2((void (*)())Actor_SetModeById, 28, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 33);
  Call3((void (*)())Func_020058d4, 28, 0, 0);
  Func_020058fc(4, 0x101, 30);
  Call3((void (*)())Actor_ArmCallback, 27, 0xb000, 0);
  Call2((void (*)())Actor_SetModeById, 27, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 33);
  Call3((void (*)())Func_020058d4, 0x201b, 0, 0);
  Call2((void (*)())Actor_SetModeById, 28, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020058d4, 28, 0, 0);
  Call2((void (*)())Actor_StartRepeatedMotion, 4, 2);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call2((void (*)())Actor_SetModeById, 27, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020058d4, 0x201b, 0, 0);
  FIELD_AT((s32)gAdventure, u8, 587) = 2;
  Call2((void (*)())Party_SetFields1eeAnd1f0, 0x109, 10);
  Call2((void (*)())Party_SetFields1f2And1f4, 0x109, 10);
  Call1((void (*)())GameFlag_SetBit, 0x11b);
  Call2((void (*)())Func_0200593c, 13, 5);
  return;
}
