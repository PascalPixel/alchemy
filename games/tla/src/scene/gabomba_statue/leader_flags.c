#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "heap_release.h"
#include "resource.h"
#include "event_inline.h"

extern u8 gSceneState[];
extern const u8 gField_CachedTilesA[];
void Func_020055cc(const u8 *data, void *buffer);

void Func_02003868(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (gMainRoutineWork.entrance == 1) {
    Event_SetFlag(0xa27);
    Event_ClearFlag(0xa28);
  }
}
