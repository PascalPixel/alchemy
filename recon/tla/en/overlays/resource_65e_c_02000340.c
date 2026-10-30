// Near miss, differ 2 of 114 (tla-en, 2026-10-30): one reordered row, the load
// of gMainRoutineWork's address into r5. In sched2 that load has priority 47:
// its latency of 2, plus the cycle -mthumb-slow-loads adds to a load on the
// critical path, above the currentOwner read (44). lsls r2 (the 0x209 store's
// value) also has 47 and the add into the event work 46, so the load wins the
// tie on its four dependents. The native order needs it at 46, the plain
// latency: after both, before adds r2, #255. Measured: the store through struct
// EventWork's screenEffect (2), gMainRoutineWork through a local pointer (2),
// currentOwner read before the store (21), the barrier form of
// champa_interiors/scene_tables.c (2: the load moves after adds r2, #255 and
// the barrier counts 4 pool bytes), alchemy permute 5 minutes (nothing better).
// Draft, differ 2 of 114 under -mthumb-slow-loads: the compiler issues
// gMainRoutineWork's load (ldr r5) before lsls r2, r2, #1 and the add into the event
// work; the native issues it after them. Adopted in 573088529f, when it
// matched without the option; back on the listing since.
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "scene_ids.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit();
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02000454(void);
void Func_020005e4(s32 id, s32 a, s32 b);

s32 Func_02000340(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom1) {
    if (gMainRoutineWork.entrance == 5)
      Func_02000454();
    else if (!GameFlag_Test(0x821) ||
             (GameFlag_Test(0xa39) && !GameFlag_Test(0x341)) ||
             GameFlag_Test(34))
      Func_020005e4(18, 0, 0);
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
