#include "lemurian_ship.h"
#include "call.h"

/// Places the leader and object 64 304 pixels back along z (Func_02002cd0),
/// lets the camera follow the leader, starts a blend and sets flag 0x9e4.
void Func_020012b4(void) {
  struct LemurianShipObject *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct LemurianShipObject *object;

  Func_02002c68();
  Func_02002da8(0);
  Call4((void (*)())Func_02002d68, 0x80000, 0x100000, 0xf80000, 0x1000000);
  Func_02002cd0(gMainRoutineWork.currentOwner, leader->x, leader->z + 0xfed00000);
  object = Actor_GetById(64);
  Func_02002cd0(64, object->x, object->z + 0xfed00000);
  Actor_SetPartAttribute(Actor_GetById(64), 0);
  wait(10);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 0);
  wait(1);
  LemurianShip_BlendStart(6, 0, 1, 2, 6, 4);
  Func_02002c10();
  wait(1);
  GameFlag_SetBit(0x9e4);
  Func_02002d88(0x10000, 0);
  Func_02002c70();
}
