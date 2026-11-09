#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 Func_02003a7c(void);

void Func_0200244c(s32 height) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);

  if (!Event_GetFlag(0x109)) {
    object->motionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    object->y += height;
    object->speedLimit = 0x10000;
    object->velocityY = 0;
    gMainRoutineWork.renderMode = 2;
    object->actionFlags = 1;
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
    Func_02003a7c();
    System_Wait(1);
  }
}
