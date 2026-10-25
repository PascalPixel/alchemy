// Draft, differ 106 of 369: the Gabomba statue chamber's scene before the
// ceremony: the party and objects 5, 6, 7, 11 and 12 are placed, the
// screen fades in, object 12 walks up and speaks (MsgIMightBe), object 8
// drops; the leader is asked a question and object 7 answers by the reply,
// counting it in the event work at 0x1c4, then objects 6 and 11 speak and
// everyone turns to follow (gData_02001f4c); sets flag 0x8fd.
// Left: constants: the native keeps the angles 0xe000, 0xa000, 0x8000, 0 and 0x100 in r5, r8 to r10 across the calls while building each position anew (the draft's Call4 builds all of them anew, plain calls hoist the positions too), and it shares the reply's count increment between the two answers.
#include "../../../../games/tla/src/scene/gabomba_statue_chamber/gabomba_statue_chamber.h"

#include "call.h"

extern const u8 gData_02001f4c[];
void Func_02001010(void);
void Func_02000f00(void);
void Func_02001064(void);
void Func_02001d40(void);
void Func_02001e18(s32 actor, s32 x, s32 z, s32 angle);
void Func_02001f08(void);

#define EVENT_WORK                                                             \
  ((struct GabombaStatueChamberEventWork *)Ram_HeapSlots->eventWork)

void Func_02001710(void) {
  Func_02001da0();
  Func_02001ee8(0);
  GabombaStatueChamber_CamMove(-1, -1, -1, 0);
  GabombaStatueChamber_WaitFrames(1);
  Func_02001010();
  Func_02001d40();
  GabombaStatueChamber_WaitFrames(1);
  Call4(Func_02001e18, gPartyState.currentOwner, 0x1680000, 0xd80000, 0xe000);
  Call4(Func_02001e18, 7, 0x1780000, 0xd80000, 0xe000);
  Call4(Func_02001e18, 11, 0x1680000, 0xc80000, 0);
  Call4(Func_02001e18, 5, 0x1980000, 0xd80000, 0xa000);
  Call4(Func_02001e18, 6, 0x1a80000, 0xc80000, 0x8000);
  Call4(Func_02001e18, 12, 0x1880000, 0xc80000, 0xb000);
  GabombaStatueChamber_WaitFrames(1);
  EVENT_WORK->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  GabombaStatueChamber_Wait(40);
  GabombaStatueChamber_ObjArm(12, 0xd000, 20);
  GabombaStatueChamber_ObjTurn(12, 0x5000);
  Func_02001e88(12, 262, 40);
  GabombaStatueChamber_ObjArm(7, 0xc000, 0);
  GabombaStatueChamber_ObjArm(5, 0xc000, 0);
  GabombaStatueChamber_ObjArm(11, 0xd000, 0);
  GabombaStatueChamber_ObjArm(6, 0xa000, 0);
  GabombaStatueChamber_ObjSpeed(12, 0xcccc, 0x6666);
  GabombaStatueChamber_ObjWalk(12, 392, 168);
  GabombaStatueChamber_SePlay(210);
  Func_02000f00();
  GabombaStatueChamber_Wait(40);
  GabombaStatueChamber_SePlay(185);
  Object_GetById(8)->motionFlags = 0;
  Func_02001064();
  GabombaStatueChamber_ObjSpeed(8, 0x6666, 0x3333);
  ObjectMotion_SetPositionAndCommit(8, 424, 152);
  GabombaStatueChamber_ObjTurn(12, 0x5000);
  Func_02001e48((s32)MsgIMightBe);
  Func_02001e60(0x400c, 0);
  Motion_SetModeAndWaitAnimation(12, 3);
  GabombaStatueChamber_ObjWalk(12, 392, 140);
  GabombaStatueChamber_WaitFrames(1);
  GabombaStatueChamber_ObjPosSet(12, 0, 0);
  GabombaStatueChamber_WaitFrames(1);
  Func_02001e88(5, 0x100, 20);
  GabombaStatueChamber_ObjArm(5, 0x8000, 0);
  UiText_OpenMessageAtObject(0x4005, 0);
  if (Event_AskYesNoWithActorAnimation(gPartyState.currentOwner, 0) == 0) {
    GabombaStatueChamber_ObjTurn(7, 0);
    Object_SetModeById(7, 3);
    Func_02001e60(0x4007, 0);
    EVENT_WORK->field1c4++;
  } else {
    EVENT_WORK->field1c4++;
    GabombaStatueChamber_ObjTurn(7, 0x8000);
    Func_02001e60(0x4007, 0);
    Func_02001f08();
  }
  GabombaStatueChamber_ObjArm(6, 0x8000, 0);
  Func_02001e90(6, 258);
  GabombaStatueChamber_Wait(20);
  Func_02001e60(0x4006, 0);
  GabombaStatueChamber_ObjTurn(11, 0);
  Object_SetModeById(11, 4);
  Func_02001e60(0x400b, 0);
  GabombaStatueChamber_ObjTurn(11, 0xd000);
  Func_02001e60(0x400b, 0);
  GabombaStatueChamber_ObjTurn(11, 0x3000);
  Motion_SetModeAndWaitAnimation(11, 3);
  Func_02001e60(0x400b, 0);
  GabombaStatueChamber_ObjArm(7, 0x8000, 0);
  GabombaStatueChamber_ObjArm(7, 0x8000, 20);
  Object_SetModeById(gPartyState.currentOwner, 3);
  Object_SetModeById(7, 3);
  Object_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(6, 3);
  GabombaStatueChamber_ObjSpeed(11, 0xcccc, 0x6666);
  GabombaStatueChamber_ObjSpeed(7, 0xcccc, 0x6666);
  GabombaStatueChamber_ObjSpeed(5, 0x10000, 0x8000);
  GabombaStatueChamber_ObjSpeed(6, 0x13333, 0x9999);
  ObjectMotion_EnableActionAndSetCallback(11, gData_02001f4c);
  ObjectMotion_EnableActionAndSetCallback(7, gData_02001f4c);
  ObjectMotion_EnableActionAndSetCallback(5, gData_02001f4c);
  Object_SetActionCallbackAndRefreshById(6, gData_02001f4c);
  GabombaStatueChamber_FlagSet(0x8fd);
  Func_02001da8();
}
