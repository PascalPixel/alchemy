#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
struct ObjectRuntime *Func_02003440(void);

void Func_02002cb8(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x = object->x;
    s32 z = object->z;

    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x40000, 0x20000);
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads x and z before it copies the object to r10 (one reordered row), as overlay 69C's twin. */
    x >>= 20;
    z >>= 20;
    ObjectMotion_SetPositionAndCommit(gPartyState.current_owner, (x << 4) + 8, (z << 4) + 8);
    WaitFrames(1);
    Object_SetPositionAndResetMotion(object, (x << 20) + 0x80000, object->y, (z << 20) + 0x80000);
    Battle_WaitMode0(4);
    Func_02003440()->y -= 0x80000;
}
