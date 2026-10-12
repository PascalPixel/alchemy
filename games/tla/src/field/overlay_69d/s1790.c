#include "types.h"
#include "party_state.h"
#include "field_event.h"

void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(u32 object_id, s32 x, s32 z);

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct FieldActor *Func_02001ea8(void);

void Func_02001790(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x = object->x;
    s32 z = object->z;

    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x40000, 0x20000);
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native loads x and z before it copies the object to r10 (one reordered row). */
    x >>= 20;
    z >>= 20;
    ObjectMotion_SetPositionAndCommit(gPartyState.current_owner, (x << 4) + 8, (z << 4) + 8);
    WaitFrames(1);
    Object_SetPositionAndResetMotion(object, (x << 20) + 0x80000, object->y, (z << 20) + 0x80000);
    Battle_WaitMode0(4);
    Func_02001ea8()->y += 0xfff80000;
}
