// Draft, differ 14 of 369, skeleton gap 0 (tla-en, 2026-11-01)
// The Gabomba statue chamber's scene before the ceremony: the party and
// objects 5, 6, 7, 11 and 12 are placed, the screen fades in, object 12 walks
// up and speaks (MsgIMightBe), object 8 drops; the leader is asked a question
// and object 7 answers by the reply, counting it in the event work at 0x1c4,
// then objects 6 and 11 speak and everyone turns to follow (gData_02001f4c);
// sets flag 0x8fd.
// Left: only the else branch's count increment, where r2 and r3 swap (the
// native uses the then branch's registers: pointer in r2, value in r3).
// Pointer and value locals, += and pre-increment, a local answer and plain
// calls for the turns all leave it. The angle locals each hold one constant;
// Value3 on Func_02001e88 and Call3 on Actor_SetPositionAndCommit are the forms
// the matched s0184.c and s10d4.c use, and MessagePut on 0x400b stops the three
// calls sharing the constant.
#include "../../../../games/tla/src/scene/gabomba_statue_chamber/gabomba_statue_chamber.h"

#include "call.h"

extern const u8 gData_02001f4c[];
void Func_02001010(void);
void Func_02000f00(void);
void Func_02001064(void);
void Func_02001d40(void);
void Func_02001e18(s32 actor, s32 x, s32 z, s32 angle);
void Func_02001f08(void);
void Actor_SetPositionAndCommit();

#define EVENT_WORK                                                             \
  ((struct GabombaStatueChamberEventWork *)Ram_HeapSlots->eventWork)

void Func_02001710(void) {
  s32 partyAngle;
  s32 zero;
  s32 angle5;
  s32 angle6;
  s32 value100;
  s32 angle12;
  s32 leader;

  Func_02001da0();
  Func_02001ee8(0);
  GabombaStatueChamber_CamMove(-1, -1, -1, 0);
  wait_(1);
  Func_02001010();
  Func_02001d40();
  wait_(1);
  leader = gMainRoutineWork.currentOwner;
  partyAngle = 0xe000;
  Call4(Func_02001e18, leader, 0x1680000, 0xd80000, partyAngle);
  Call4(Func_02001e18, 7, 0x1780000, 0xd80000, partyAngle);
  zero = 0;
  angle5 = 0xa000;
  Call4(Func_02001e18, 11, 0x1680000, 0xc80000, zero);
  Call4(Func_02001e18, 5, 0x1980000, 0xd80000, angle5);
  angle6 = 0x8000;
  Call4(Func_02001e18, 6, 0x1a80000, 0xc80000, angle6);
  angle12 = 0xb000;
  Call4(Func_02001e18, 12, 0x1880000, 0xc80000, angle12);
  wait_(1);
  EVENT_WORK->screenEffect = value100 = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  GabombaStatueChamber_Wait(40);
  GabombaStatueChamber_ObjArm(12, 0xd000, 20);
  GabombaStatueChamber_ObjTurn(12, 0x5000);
  Value3((s32 (*)())Func_02001e88, 12, 262, 40);
  GabombaStatueChamber_ObjArm(7, 0xc000, 0);
  GabombaStatueChamber_ObjArm(5, 0xc000, 0);
  GabombaStatueChamber_ObjArm(11, 0xd000, 0);
  GabombaStatueChamber_ObjArm(6, angle5, 0);
  GabombaStatueChamber_ObjSpeed(12, 0xcccc, 0x6666);
  GabombaStatueChamber_ObjWalk(12, 392, 168);
  GabombaStatueChamber_SePlay(210);
  Func_02000f00();
  GabombaStatueChamber_Wait(40);
  GabombaStatueChamber_SePlay(185);
  Actor_GetById(8)->motionFlags = zero;
  Func_02001064();
  GabombaStatueChamber_ObjSpeed(8, 0x6666, 0x3333);
  Call3(Actor_SetPositionAndCommit, 8, 424, 152);
  GabombaStatueChamber_ObjTurn(12, 0x5000);
  Func_02001e48((s32)MsgIMightBe);
  Func_02001e60(0x400c, 0);
  Motion_SetModeAndWaitAnimation(12, 3);
  GabombaStatueChamber_ObjWalk(12, 392, 140);
  wait_(1);
  GabombaStatueChamber_ObjPosSet(12, 0, 0);
  wait_(1);
  Value3((s32 (*)())Func_02001e88, 5, value100, 20);
  GabombaStatueChamber_ObjArm(5, angle6, 0);
  FieldMessage_ShowAtActor(0x4005, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    GabombaStatueChamber_ObjTurn(7, 0);
    Actor_SetModeById(7, 3);
    Func_02001e60(0x4007, 0);
    EVENT_WORK->field1c4++;
  } else {
    EVENT_WORK->field1c4++;
    GabombaStatueChamber_ObjTurn(7, angle6);
    Func_02001e60(0x4007, 0);
    Func_02001f08();
  }
  GabombaStatueChamber_ObjArm(6, 0x8000, 0);
  Func_02001e90(6, 258);
  GabombaStatueChamber_Wait(20);
  Func_02001e60(0x4006, 0);
  GabombaStatueChamber_ObjTurn(11, 0);
  Actor_SetModeById(11, 4);
  GabombaStatueChamber_MessagePut(0x400b, 0);
  GabombaStatueChamber_ObjTurn(11, 0xd000);
  GabombaStatueChamber_MessagePut(0x400b, 0);
  GabombaStatueChamber_ObjTurn(11, 0x3000);
  Motion_SetModeAndWaitAnimation(11, 3);
  GabombaStatueChamber_MessagePut(0x400b, 0);
  GabombaStatueChamber_ObjArm(7, 0x8000, 0);
  GabombaStatueChamber_ObjArm(7, 0x8000, 20);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(6, 3);
  GabombaStatueChamber_ObjSpeed(11, 0xcccc, 0x6666);
  GabombaStatueChamber_ObjSpeed(7, 0xcccc, 0x6666);
  GabombaStatueChamber_ObjSpeed(5, 0x10000, 0x8000);
  GabombaStatueChamber_ObjSpeed(6, 0x13333, 0x9999);
  Actor_SetMovementTypeById(11, gData_02001f4c);
  Actor_SetMovementTypeById(7, gData_02001f4c);
  Actor_SetMovementTypeById(5, gData_02001f4c);
  Actor_SetMovementTypeAndWaitById(6, gData_02001f4c);
  GabombaStatueChamber_FlagSet(0x8fd);
  Func_02001da8();
}
