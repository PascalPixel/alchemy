#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_020000f4(void);

/// The room's setup: it schedules the palette cycle Func_020000f4.
s32 Func_02000158(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Task_AddFrame(Func_020000f4, 0xc80);
  Actor_SetSpritePriority(8, 3);
  if (gMainRoutineWork.entrance == 6)
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (GameFlag_Test(0x96a)) {
    Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
