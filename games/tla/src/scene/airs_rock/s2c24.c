#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "flag_call.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 Func_02003b50(void);

void Func_02002c24(s32 height) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);

  if (!Flag_IsSet(0x109)) {
    object->motionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    object->y += height;
    object->speedLimit = 0x10000;
    object->velocityY = 0;
    gMainRoutineWork.renderMode = 2;
    object->actionFlags = 1;
    Camera_FollowActor(gMainRoutineWork.currentOwner, 0);
    Func_02003b50();
    wait(1);
  }
}
