#include "lemurian_ship.h"
#include "scene_tables.h"

void Func_02001464(void);
void Func_0200155c(void);

/// Restores the scene from its flags: two map edits, actors 8 to 11 and two
/// scheduled tasks.
void Func_02002970(void) {
  struct LemurianShipObject *object;

  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02002dc8(gData_0200304e);
  if (LemurianShip_FlagCheck(0x982)) {
    LemurianShip_MapCopy(4, 124, 31, 64, 3, 2);
    LemurianShip_FlagSet(0x202);
    LemurianShip_FlagSet(0x203);
  }
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  if (LemurianShip_FlagCheck(0x202)) {
    object = Actor_GetById(8);
    LemurianShip_MapCopy(35, 68, 35, 69, 1, 1);
    LemurianShip_BlendStart(35, 0, 1, 1, 35, 9);
    object->motionFlags = 0;
    LemurianShip_ObjSetPos(object, PIXELS(568), PIXELS(-16), PIXELS(152));
    Actor_SetSpritePriority(8, 3);
    LemurianShip_FlagSet(0x204);
    LemurianShip_ObjPlace(10, PIXELS(568), PIXELS(152));
  }
  if (LemurianShip_FlagCheck(0x204)) {
    if (LemurianShip_FlagCheck(0x202))
      Actor_SetModeById(8, 3);
    else
      Actor_SetModeById(8, 2);
  }
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  if (!LemurianShip_FlagCheck(0x109))
    LemurianShip_FlagSet(0x203);
  if (LemurianShip_FlagCheck(0x203)) {
    object = Actor_GetById(9);
    LemurianShip_MapCopy(29, 68, 29, 69, 1, 1);
    LemurianShip_BlendStart(35, 0, 1, 1, 29, 9);
    object->motionFlags = 0;
    object->y = 0xfff00000;
    Actor_SetSpritePriority(9, 3);
    LemurianShip_FlagSet(0x205);
    LemurianShip_ObjPlace(11, PIXELS(472), PIXELS(152));
  }
  if (LemurianShip_FlagCheck(0x205)) {
    if (LemurianShip_FlagCheck(0x203))
      Actor_SetModeById(9, 3);
    else
      Actor_SetModeById(9, 2);
  }
  wait_(1);
  Func_02002db8(gData_02003014);
  LemurianShip_TaskAdd(Func_02001464, 0x480);
  if (!LemurianShip_FlagCheck(0x210))
    Actor_SetPartAttribute(Actor_GetById(10), 15);
  if (!LemurianShip_FlagCheck(0x211))
    Actor_SetPartAttribute(Actor_GetById(11), 15);
  LEMURIAN_SHIP_TASK_ADD(Func_0200155c, 0x480);
}
