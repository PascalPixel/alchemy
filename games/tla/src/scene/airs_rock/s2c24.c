#include "types.h"
#include "sequence_parent_work.h"
#include "actor.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);
s32 wait(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 Camera_FollowActor(s32 id, s32 a);
s32 Func_02003b50(void);

void Func_02002c24(s32 height) {
  struct Actor *object = Actor_GetById(gSequenceParentWork.currentOwner);

  if (!Flag_IsSet(0x109)) {
    object->motionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    object->y += height;
    object->speedLimit = 0x10000;
    object->velocityY = 0;
    gSequenceParentWork.renderMode = 2;
    object->actionFlags = 1;
    Camera_FollowActor(gSequenceParentWork.currentOwner, 0);
    Func_02003b50();
    wait(1);
  }
}
