#include "lemurian_ship.h"

void Func_02001474(void);
void Func_02001a9c(void);

/// Sets up the scene by flags 0x93f, 0x9e4 and 0x22f: actor 64, actors 8 to 13
/// and the camera.
void Func_02002820(void) {
  struct LemurianShipObject *object;

  Actor_GetById(gSequenceParentWork.currentOwner)->priorityFlags |= 32;
  LEMURIAN_SHIP_TASK_ADD(Func_02001474, 0x480);
  if (LemurianShip_FlagCheck(0x93f)) {
    object = Actor_GetById(64);
    object->motionFlags = 0;
    LemurianShip_ObjSetPos(Actor_GetById(64), 0x880000, 0xffe00000, 0x1880000);
    object->height = 0xfff00000;
    Actor_SetSpritePriority(64, 3);
    Actor_SetPartAttribute(Actor_GetById(64), 1);
    LemurianShip_ObjPlace(8, 0, 0);
    LemurianShip_ObjPlace(9, 0, 0);
    LemurianShip_ObjPlace(10, 0, 0);
    LemurianShip_ObjPlace(11, 0, 0);
    LemurianShip_ObjPlace(12, 0, 0);
    LemurianShip_ObjPlace(13, 0, 0);
    wait_(1);
  }
  if (LemurianShip_FlagCheck(0x9e4)) {
    struct LemurianShipObject *marker = Actor_GetById(64);

    LemurianShip_ObjPlace(64, marker->x, marker->z + 0xfed00000);
    Actor_SetPartAttribute(Actor_GetById(64), 0);
    LemurianShip_BlendStart(6, 0, 1, 2, 6, 4);
    LemurianShip_CamSet(0x80000, 0x100000, 0xf80000, 0x1000000);
    Camera_FollowActor(gSequenceParentWork.currentOwner, 0);
    Func_02002c10();
    wait_(1);
  }
  if (LemurianShip_FlagCheck(0x22f)) {
    Func_02001a9c();
    GameFlag_ClearBit(0x22f);
  }
}
