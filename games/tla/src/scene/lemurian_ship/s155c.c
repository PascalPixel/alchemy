#include "lemurian_ship.h"
#include "macros.h"
#include "scene_tables.h"

void Func_0200155c(void) {
  struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
  struct LemurianShipObject *object;

  if (GameFlag_Test(0x202)) {
    object = Actor_GetById(10);
    if ((object->x | object->z) == 0) {
      work->field16a = 200;
      Actor_SetModeById(8, 1);
    }
  }
  if (GameFlag_Test(0x203)) {
    object = Actor_GetById(11);
    if ((object->x | object->z) == 0) {
      work->field16a = 201;
      Actor_SetModeById(9, 1);
    }
  }
}

void Func_020015c8(void) {
  s32 position[3];
  struct LemurianShipObject *object;

  SET_VECTOR(position, 0x3280000, 0x80000, 0x8c0000);
  Vector_AddPolarOffsetFar(0x200000, Random16Far(), position);
  object = Func_02002c00(0x2a2, position[0], position[1], position[2]);
  if (object != NULL) {
    object->motionFlags = 0;
    *(s32 *)((u8 *)object + 0x30) = 0x20000;
    *(s32 *)((u8 *)object + 0x34) = 0x8000;
    Actor_SetSingleChildFlags(object, 0);
    Func_02002bf0(object, 0);
    Func_02002c18(object, PIXELS(808), PIXELS(8), PIXELS(140));
    Func_02002bf8(object, gData_02003ef8);
  }
}
