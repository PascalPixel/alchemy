#include "kibombo.h"

void Func_02003a08(s32 x, s32 z) {
  struct SequenceParentWork *party = &gSequenceParentWork;
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
  struct KibomboObject *leader = Actor_GetById(party->currentOwner);

  Func_02005a9c();
  Func_02005c3c(0);
  leader->motionFlags = 0;
  Kibombo_ObjSpeed(gSequenceParentWork.currentOwner, 0x10000, 0x8000);
  Kibombo_ObjWalk(gSequenceParentWork.currentOwner, x, z);
  Kibombo_ObjTurn(gSequenceParentWork.currentOwner, 0xc000);
  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  Actor_SetModeById(gSequenceParentWork.currentOwner, 13);
  Func_02005a54(leader, x << 16, 0xfff40000, z << 16);
  Func_02005a5c(leader);
  Actor_SetModeById(gSequenceParentWork.currentOwner, 10);
  Func_02005c9c(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005bc4(work->exit);
}
