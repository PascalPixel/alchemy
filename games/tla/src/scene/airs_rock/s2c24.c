#include "types.h"
#include "party_state.h"
#include "object_runtime.h"
#include "flag_call.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 WaitFrames(s32 frames);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
s32 Object_AttachWorkTargetToObject(s32 id, s32 a);
s32 Func_02003b50(void);

void Func_02002c24(s32 height) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);

  if (!Flag_IsSet(0x109)) {
    object->motionFlags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->y += height;
    object->speedLimit = 0x10000;
    object->velocityY = 0;
    gPartyState.renderMode = 2;
    object->actionFlags = 1;
    Object_AttachWorkTargetToObject(gPartyState.currentOwner, 0);
    Func_02003b50();
    WaitFrames(1);
  }
}
