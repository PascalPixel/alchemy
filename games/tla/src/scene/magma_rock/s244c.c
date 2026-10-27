#include "types.h"
#include "party_state.h"
#include "object_runtime.h"
#include "flag_call.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 WaitTicks(s32 ticks);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
s32 Camera_FollowActor(s32 id, s32 a);
s32 Func_02003a7c(void);

void Func_0200244c(s32 height) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);

  if (!Flag_IsSet(0x109)) {
    object->motionFlags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->y += height;
    object->speedLimit = 0x10000;
    object->velocityY = 0;
    gPartyState.renderMode = 2;
    object->actionFlags = 1;
    Camera_FollowActor(gPartyState.currentOwner, 0);
    Func_02003a7c();
    WaitTicks(1);
  }
}
