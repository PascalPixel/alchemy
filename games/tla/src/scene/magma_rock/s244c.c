#include "types.h"
#include "adventure.h"
#include "actor.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);
s32 wait(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 Camera_FollowActor(s32 id, s32 a);
s32 Func_02003a7c(void);

void Func_0200244c(s32 height) {
  struct Actor *object = Actor_GetById(gAdventure.currentOwner);

  if (!Flag_IsSet(0x109)) {
    object->motionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    object->y += height;
    object->speedLimit = 0x10000;
    object->velocityY = 0;
    gAdventure.renderMode = 2;
    object->actionFlags = 1;
    Camera_FollowActor(gAdventure.currentOwner, 0);
    Func_02003a7c();
    wait(1);
  }
}
