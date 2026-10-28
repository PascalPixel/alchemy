#include "kibombo.h"

void Func_02003a08(s32 x, s32 z) {
  struct Adventure *party = &gAdventure;
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
  struct KibomboObject *leader = Object_GetById(party->currentOwner);

  Func_02005a9c();
  Func_02005c3c(0);
  leader->motionFlags = 0;
  Kibombo_ObjSpeed(gAdventure.currentOwner, 0x10000, 0x8000);
  Kibombo_ObjWalk(gAdventure.currentOwner, x, z);
  Kibombo_ObjTurn(gAdventure.currentOwner, 0xc000);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gAdventure.currentOwner),
                                     0);
  Object_SetModeById(gAdventure.currentOwner, 13);
  Func_02005a54(leader, x << 16, 0xfff40000, z << 16);
  Func_02005a5c(leader);
  Object_SetModeById(gAdventure.currentOwner, 10);
  Func_02005c9c(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005bc4(work->exit);
}
