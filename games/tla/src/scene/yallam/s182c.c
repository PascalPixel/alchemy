#include "../../../../games/tla/src/scene/yallam/yallam.h"
#include "task.h"

void Func_0200182c(void) {
  struct Actor *object;

  object = Actor_GetById(8);
  object->action = gData_02002fc0[0].action;
  object->unk66 = gData_02002fc0[0].mode;
  BYTE_AT(object, 98) = gData_02002fc0[0].byte98;
  BYTE_AT(object, 99) = gData_02002fc0[0].byte99;
  object = Actor_GetById(10);
  object->action = gData_02002fc0[1].action;
  object->unk66 = gData_02002fc0[1].mode;
  BYTE_AT(object, 98) = gData_02002fc0[1].byte98;
  BYTE_AT(object, 99) = gData_02002fc0[1].byte99;
  object = Actor_GetById(9);
  object->action = gData_02002fc0[2].action;
  object->unk66 = gData_02002fc0[2].mode;
  BYTE_AT(object, 98) = gData_02002fc0[2].byte98;
  BYTE_AT(object, 99) = gData_02002fc0[2].byte99;
  object = Actor_GetById(11);
  object->action = gData_02002fc0[3].action;
  object->unk66 = gData_02002fc0[3].mode;
  BYTE_AT(object, 98) = gData_02002fc0[3].byte98;
  BYTE_AT(object, 99) = gData_02002fc0[3].byte99;
  object = Actor_GetById(gSequenceParentWork.currentOwner);
  object->action = gData_02002fc0[4].action;
  object->unk66 = gData_02002fc0[4].mode;
  BYTE_AT(object, 98) = gData_02002fc0[4].byte98;
  BYTE_AT(object, 99) = gData_02002fc0[4].byte99;
  Task_Add(Func_02001688, TASK_PASS_VBLANK);
}
