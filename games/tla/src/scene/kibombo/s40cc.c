#include "kibombo.h"

void Func_02002eb0(void);
void Func_020032e0(void);
void Func_02003610(void);

void Func_020040cc(void) {
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_02005bbc()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Kibombo_Wait(40);
  Kibombo_SePlay(78);
  Kibombo_CamSpeed(0xcccc, 0x1999);
  Kibombo_CamMove(132 << 17, -1, 194 << 17, 1);
  Func_02005bb4();
  Kibombo_ObjTurn(17, 0x3000);
  Motion_SetModeAndWaitAnimation(17, 3);
  Func_02005b4c((s32)MsgLetUsNow);
  Kibombo_MessagePut(0x4000 + 17, 0);
  Kibombo_SePlay(26);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(27), 0);
  Object_GetById(27)->motionFlags = 0;
  Kibombo_ObjSetPos(Object_GetById(27), 0x1090000, 0x180000, 172 << 17);
  Kibombo_CamSpeed(0x6666, 0xccc);
  Kibombo_CamMove(132 << 17, -1, 170 << 17, 1);
  Kibombo_ObjSpeed(17, 0x3333, 0x1999);
  Kibombo_ObjWalk(17, 264, 374);
  WaitFrames(1);
  Kibombo_TaskAdd(Func_02002eb0, 0x480);
  Kibombo_Wait(20);
  Scheduler_RemoveCallbackFar(Func_02002ccc);
  Func_020032e0();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Kibombo_MessagePut(17, 0);
  Kibombo_SePlay(11);
  Func_02003610();
  Kibombo_Wait(160);
  Func_02005b5c(17, 0, 80);
  Kibombo_SePlay(0x240);
  Func_02003640(1, 1);
  Kibombo_Wait(20);
  Func_02005b5c(17, 0, 40);
  Scheduler_RemoveCallbackFar(Func_02002ccc);
  WaitFrames(1);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x202;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005bc4(10);
}
