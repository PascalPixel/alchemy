#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "scene_ids.h"
#include "event_inline.h"

void Func_02000454(void);

s32 Func_02000340(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom1) {
    if (gMainRoutineWork.entrance == 5)
      Func_02000454();
    else if (!GameFlag_Test(0x821) ||
             (GameFlag_Test(0xa39) && !GameFlag_Test(0x341)) ||
             GameFlag_Test(34))
      Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    else
      Actor_SetPartAttribute(Actor_GetById(18), 4);
    return 0;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom2) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 1) {
      Actor_GetById(8)->collisionFlags |= 4;
      Actor_SetSpritePriority(8, 2);
    } else if (entrance == 5) {
      GameFlag_ClearBit(0x12f);
      Actor_GetById(9)->collisionFlags |= 4;
      Actor_SetSpritePriority(9, 2);
    } else if (entrance == 4) {
      GameFlag_ClearBit(0x12f);
    }
  }
  return 0;
}
