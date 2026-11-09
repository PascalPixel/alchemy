#include "lemurian_ship.h"
#include "event_inline.h"

/// Places the leader and object 64 304 pixels back along z (Actor_SetHorizontalPositionWithTerrain),
/// lets the camera follow the leader, starts a blend and sets flag 0x9e4.
void Func_020012b4(void) {
  struct LemurianShipObject *leader = ((struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner));
  struct LemurianShipObject *object;

  Func_02002c68();
  Event_ResolvePendingActions(0);
  Field_SetCameraBounds(0x80000, 0x100000, 0xf80000, 0x1000000);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, leader->x, leader->z + 0xfed00000);
  object = ((struct LemurianShipObject *)Actor_GetById(64));
  Actor_SetHorizontalPositionWithTerrain(64, object->x, object->z + 0xfed00000);
  Actor_SetPartAttribute(((struct LemurianShipObject *)Actor_GetById(64)), 0);
  wait(10);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 0);
  wait(1);
  MapGrid_CopyCellAttributes(6, 0, 1, 2, 6, 4);
  Func_02002c10();
  wait(1);
  GameFlag_SetBit(0x9e4);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  Func_02002c70();
}
