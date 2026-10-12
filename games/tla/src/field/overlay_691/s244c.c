#include "types.h"
#include "party_state.h"
#include "object_runtime.h"
#include "flag_call.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 WaitFrames(s32 frames);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
s32 Object_AttachWorkTargetToObject(s32 id, s32 a);
s32 Func_02003a7c(void);

void Func_0200244c(s32 height)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);

    if (!Flag_IsSet(0x109)) {
        object->motion_flags = 0;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        object->y += height;
        object->speed_limit = 0x10000;
        object->velocity_y = 0;
        gPartyState.render_mode = 2;
        object->action_flags = 1;
        Object_AttachWorkTargetToObject(gPartyState.current_owner, 0);
        Func_02003a7c();
        WaitFrames(1);
    }
}
