#include "lemurian_ship.h"

void Func_02001474(void);
void Func_02001a9c(void);

// Sets up the scene by flags 0x93f, 0x9e4 and 0x22f: actor 64, actors 8 to 13
// and the camera.
void Func_02002820(void) {
  struct LemurianShipObject *object;

  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
  LEMURIAN_SHIP_TASK_ADD(Func_02001474, 0x480);
  if (LemurianShip_FlagCheck(0x93f)) {
    object = Object_GetById(64);
    object->motionFlags = 0;
    LemurianShip_ObjSetPos(Object_GetById(64), 0x880000, 0xffe00000, 0x1880000);
    object->height = 0xfff00000;
    ObjectMotion_SetActionVariant(64, 3);
    Object_SetPartAttribute(Object_GetById(64), 1);
    LemurianShip_ObjPlace(8, 0, 0);
    LemurianShip_ObjPlace(9, 0, 0);
    LemurianShip_ObjPlace(10, 0, 0);
    LemurianShip_ObjPlace(11, 0, 0);
    LemurianShip_ObjPlace(12, 0, 0);
    LemurianShip_ObjPlace(13, 0, 0);
    LemurianShip_WaitFrames(1);
  }
  if (LemurianShip_FlagCheck(0x9e4)) {
    struct LemurianShipObject *marker = Object_GetById(64);

    LemurianShip_ObjPlace(64, marker->x, marker->z + 0xfed00000);
    Object_SetPartAttribute(Object_GetById(64), 0);
    LemurianShip_BlendStart(6, 0, 1, 2, 6, 4);
    LemurianShip_CamSet(0x80000, 0x100000, 0xf80000, 0x1000000);
    Object_AttachWorkTargetToObject(gPartyState.currentOwner, 0);
    Func_02002c10();
    LemurianShip_WaitFrames(1);
  }
  if (LemurianShip_FlagCheck(0x22f)) {
    Func_02001a9c();
    GameFlag_ClearBit(0x22f);
  }
}
