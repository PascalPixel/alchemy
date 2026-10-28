#include "kibombo.h"

void Party_RemoveOwnerRestored(s32 owner);

void Func_02001218(void) {
  if (!Kibombo_FlagCheck(0x92e)) {
    Func_02005bc4(5);
    return;
  }
  Func_02005a9c();
  Func_02005c3c(0);
  Func_02005b1c(7, gMainRoutineWork.currentOwner);
  Actor_SetSpritePriority(
      7, Actor_GetById(gMainRoutineWork.currentOwner)->sprite->priority);
  wait(1);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, 0xa000, 0);
  Kibombo_ObjSpeed(7, 0x19999, 0xcccc);
  Kibombo_ObjWalk(7, 62, 444);
  Kibombo_ObjTurn(7, 0x2000);
  Func_02005b4c((s32)MsgNowThatIKnow);
  Kibombo_MessagePut(7, 0);
  Actor_GetById(7)->priorityFlags |= 1;
  Kibombo_ObjWalk(7, 56, 420);
  Kibombo_ObjWalk(7, 40, 344);
  Func_02005b0c(7, 0, 0);
  Kibombo_ObjPlace(gMainRoutineWork.currentOwner, 80, 510);
  Kibombo_Wait(10);
  Func_02005bc4(5);
  GameFlag_ClearBit(0x92e);
  GameFlag_SetBit(0x92f);
  Party_RemoveOwnerRestored(7);
  Func_02005aa4();
}
