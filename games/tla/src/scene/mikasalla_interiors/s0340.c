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
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom1) {
    if (gMainRoutineWork.entrance == 5)
      Func_02000454();
    else if (!Event_GetFlag(0x821) ||
             (Event_GetFlag(0xa39) && !Event_GetFlag(0x341)) ||
             Event_GetFlag(34))
      Actor_SetPosition(18, 0, 0);
    else
      Actor_SetSpriteColor(Actor_Get(18), 4);
    return 0;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom2) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 1) {
      Actor_Get(8)->collisionFlags |= 4;
      FieldActor_SetSpritePriority(8, 2);
    } else if (entrance == 5) {
      Event_ClearFlag(0x12f);
      Actor_Get(9)->collisionFlags |= 4;
      FieldActor_SetSpritePriority(9, 2);
    } else if (entrance == 4) {
      Event_ClearFlag(0x12f);
    }
  }
  return 0;
}
