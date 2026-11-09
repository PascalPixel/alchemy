#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004fdc(void);
void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

/// Unless MapGrid_GetCellBehaviorAt(2, ...) holds, copies map cell (36, 23) to (38,
/// 25), moves a leader standing on that cell aside, and sets flag 0x30a, or
/// 0x9bf once flag 0x927 is set.
void Func_02000b6c(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Call7(Func_02004ff4, 37, 26, 0, 3, 3, 61, 0);
  if (MapGrid_GetCellBehaviorAt(2, 152 << 18, 200 << 17) == 0) {
    MapGrid_CopyCellAttributes(36, 23, 1, 1, 38, 25);
    Func_02004e5c();
    FieldEvent_ResolvePendingActions(0);
    if (leader->x >> 20 == 38 && leader->z >> 20 == 25) {
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = PIXELS(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
    }
    if (!Event_GetFlag(0x927)) {
      Event_SetFlag(0x30a);
      FieldEvent_Wait(30);
      Func_02004fdc();
      FieldEvent_SetExitRequest(20);
    } else {
      Event_SetFlag(0x9bf);
      Func_02004e64();
    }
  }
}
