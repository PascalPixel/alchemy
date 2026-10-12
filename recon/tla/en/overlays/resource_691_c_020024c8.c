/* Near miss, differ 2 of 32: after the target_y store the native clears the flags argument (movs r1, #0) before it loads the owner (ldr r0); this form loads first. Seen in several near misses of this queue: a load right after a store waits one more step in the native. A void or valued callee measured the same, and a forced zero is shared with the velocity store (differ 11). */
#include "types.h"
#include "party_state.h"
#include "object_runtime.h"
#include "flag_call.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 WaitFrames(s32 frames);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Object_AttachWorkTargetToObject(s32 id, s32 a);

void Func_020024c8(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);

    if (Flag_IsSet(0x109)) {
        s32 height;

        WaitFrames(1);
        height = Map_GetTerrainHeight(object->terrain_id, object->x, object->z);
        object->velocity_y = 0;
        object->terrain_height = height;
        object->y = height;
        object->target_y = 0x80000000;
        Object_AttachWorkTargetToObject(gPartyState.current_owner, 0);
    }
}
