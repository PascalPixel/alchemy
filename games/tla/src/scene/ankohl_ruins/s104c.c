#include "ankohl_ruins.h"

void Func_0200104c(s32 unused, s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (id == 10 && x == 15 && z == 24) {
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    object->y -= 0x20000;
    FieldEvent_WaitFrames(1);
    object->y -= 0x20000;
    FieldEvent_WaitFrames(1);
    object->y -= 0x40000;
    FieldEvent_WaitFrames(1);
    object->y -= 0x80000;
    FieldEvent_WaitFrames(1);
    Object_SetModeById(9, 3);
    Func_02002e6c(10, 0, 0);
    Func_02002eec(188);
    FieldEvent_WaitFrames(45);
    Object_SetModeById(9, 4);
    Func_02002eec(564);
    GameFlag_SetBit(0x9dd);
  }
}
