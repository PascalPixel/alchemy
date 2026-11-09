#include "caverns.h"
#include "event_inline.h"

void Func_02000f50(void) {
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    MapGrid_CopyCellAttributes(28, 0, 1, 1, 28, 5);
    Map_CopyCellAttributeRect(36, 0, 1, 1, 36, 5);
  }
}

void Func_02000f98(void) {
  Func_02000454(gData_02001220);
  ((struct FieldActor *)Actor_GetById(8))->priorityFlags |= 0x20;
  ((struct FieldActor *)Actor_GetById(9))->priorityFlags |= 0x20;
  ((struct FieldActor *)Actor_GetById(10))->collisionFlags |= 8;
  ((struct FieldActor *)Actor_GetById(11))->collisionFlags |= 8;
}

void Func_02000fe4(void) {
  Func_020010dc();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  Event_Wait(10);
  MapGrid_CopyCellsTo(7, 94, 6, 94, 1, 2);
  Event_Wait(16);
  Sound_Play(123);
  Import_EventSetExitRequest(8);
  Func_020010e4();
}
