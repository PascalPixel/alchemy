#include "scene_tables.h"
#include "caverns.h"
#include "event_inline.h"

void Func_02000d68(void) {
  s32 value = HALF_AT(Ram_HeapSlots->eventWork, 340);

  if (value == 2)
    Actor_SetPosition(8, PIXELS(144), PIXELS(224));
  else
    Actor_SetPosition(8, PIXELS(224), PIXELS(224));
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  BYTE_AT(((struct FieldActor *)Actor_GetById(4)), 90) &= ~1;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(((struct FieldActor *)Actor_GetById(4)), 0);
  Actor_OffsetPositionAndResetMotion(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 13);
  Sound_Play(123);
  Event_Wait(14);
  Import_EventSetExitRequest(value);
}
