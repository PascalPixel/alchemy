#include "local.h"

void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
struct ObjectRuntime *Func_020023f0(void);

void Func_02001cc0(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
    s32 x = object->x;
    s32 z = object->z;

    ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x40000, 0x20000);
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads x and z before it copies the object to r10 (one reordered row). */
    x >>= 20;
    z >>= 20;
    ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, (x << 4) + 8, (z << 4) + 8);
    WaitFrames(1);
    Object_SetPositionAndResetMotion(object, (x << 20) + 0x80000, object->y, (z << 20) + 0x80000);
    Battle_WaitMode0(4);
    Func_020023f0()->y -= 0x80000;
}
