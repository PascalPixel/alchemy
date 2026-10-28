#include "lemurian_ship.h"

/// When actor 8 or 9 stands on its switch cell (column 35 or 29, row 9), lowers
/// it, edits the map and sets flag 0x202 or 0x203; once both are set, the last
/// map edit sets flag 0x982.
void Func_02000e2c(void) {
  struct LemurianShipObject *object;
  struct LemurianShipObject *leader;
  s32 column;
  s32 row;
  u32 i;
  s32 done;

  done = GameFlag_Test(0x202);
  if (!done && GameFlag_Test(0x204)) {
    object = Object_GetById(8);
    column = object->x >> 20;
    if (column == 35) {
      row = object->z >> 20;
      if (row == 9) {
        leader = Object_GetById(gAdventure.currentOwner);
        Func_02002c68();
        Func_02002da8(0);
        FieldEvent_WaitFrames(20);
        Func_02002de0(183);
        LemurianShip_MapCopy(35, 68, 35, 69, 1, 1);
        LemurianShip_BlendStart(35, 0, 1, 1, column, row);
        object->motionFlags = done;
        ObjectMotion_SetActionVariant(8, 3);
        if (object->z > leader->z)
          ObjectMotion_SetActionVariant(gAdventure.currentOwner, 3);
        for (i = 0; i < 128; i++) {
          object->y += 0xffffe000;
          WaitFrames(1);
        }
        Object_SetModeById(8, 3);
        LemurianShip_ObjPlace(10, 0x2380000, 0x980000);
        FieldEvent_WaitFrames(20);
        GameFlag_SetBit(0x202);
        if (object->z > leader->z) {
          ObjectMotion_SetActionVariant(gAdventure.currentOwner, 2);
          Object_GetById(gAdventure.currentOwner)->priorityFlags |= 1;
        }
        Func_02002c70();
      }
    }
  }
  done = GameFlag_Test(0x203);
  if (!done && GameFlag_Test(0x205)) {
    object = Object_GetById(9);
    column = object->x >> 20;
    if (column == 29) {
      row = object->z >> 20;
      if (row == 9) {
        leader = Object_GetById(gAdventure.currentOwner);
        Func_02002c68();
        Func_02002da8(0);
        FieldEvent_WaitFrames(20);
        Func_02002de0(183);
        LemurianShip_MapCopy(29, 68, 29, 69, 1, 1);
        LemurianShip_BlendStart(35, 0, 1, 1, column, row);
        object->motionFlags = done;
        ObjectMotion_SetActionVariant(9, 3);
        if (object->z > leader->z)
          ObjectMotion_SetActionVariant(gAdventure.currentOwner, 3);
        for (i = 0; i < 128; i++) {
          object->y += 0xffffe000;
          WaitFrames(1);
        }
        Object_SetModeById(9, 3);
        LemurianShip_ObjPlace(11, 0x1d80000, 0x980000);
        FieldEvent_WaitFrames(20);
        GameFlag_SetBit(0x203);
        if (object->z > leader->z) {
          ObjectMotion_SetActionVariant(gAdventure.currentOwner, 2);
          Object_GetById(gAdventure.currentOwner)->priorityFlags |= 1;
        }
        Func_02002c70();
      }
    }
  }
  if (GameFlag_Test(0x202) && GameFlag_Test(0x203)) {
    Func_02002c68();
    Func_02002da8(0);
    Func_02002de0(188);
    LemurianShip_MapCopy(4, 124, 31, 64, 3, 2);
    FieldEvent_WaitFrames(20);
    GameFlag_SetBit(0x982);
    Func_02002c70();
  }
}
