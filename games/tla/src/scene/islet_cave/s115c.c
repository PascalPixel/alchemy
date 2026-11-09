#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "flag_call.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_0200115c(void) {
  if (!Flag_IsSet(0x230)) {
    struct Actor *object = Actor_GetById(9);
    struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);

    Flag_Set(0x230);
    Actor_SetPosition(9, PIXELS(296), PIXELS(160));
    object->speedLimit = 0x18000;
    object->velocityY = PIXELS(15);
    Actor_MoveToAndWait(9, 344, ((owner->z >> 19) << 3) + 24);
    Actor_Stop(8);
    object->speedLimit = 0x10000;
    Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 9, 0);
  }
}
