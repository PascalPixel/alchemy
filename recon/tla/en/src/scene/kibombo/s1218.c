#include "../../../../../../games/tla/src/scene/kibombo/kibombo.h"
#include "scene_tables.h"

void Party_RemoveOwnerRestored(s32 owner);

void Func_02001218(void) {
  if (!Kibombo_FlagCheck(SPLIT_CONSTANT(0x900, 46))) {
    Func_02005bc4(5);
    return;
  }
  Func_02005a9c();
  Func_02005c3c(0);
  Func_02005b1c(7, gMainRoutineWork.currentOwner);
  Actor_SetSpritePriority(
      7, Actor_GetById(gMainRoutineWork.currentOwner)->sprite->priority);
  wait(1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Kibombo_ObjSpeed(7, 0x19999, 0xcccc);
  Actor_SetPositionAndReset(7, 62, 444);
  Func_02005b7c(7, FACING_SOUTHEAST);
  Func_02005b4c((s32)MsgNowThatIKnow);
  Func_02005b64(7, 0);
  Actor_GetById(7)->priorityFlags |= 1;
  Kibombo_ObjWalk(7, 56, 420);
  Kibombo_ObjWalk(7, 40, 344);
  Func_02005b0c(7, 0, 0);
  Kibombo_ObjPlace(gMainRoutineWork.currentOwner, 80, 510);
  FieldEvent_Wait(10);
  Func_02005bc4(5);
  GameFlag_ClearBit(0x92e);
  GameFlag_SetBit(0x92f);
  Party_RemoveOwnerRestored(7);
  Func_02005aa4();
}
