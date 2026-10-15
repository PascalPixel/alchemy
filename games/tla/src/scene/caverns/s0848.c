#include "caverns.h"

void Func_02000848(struct FieldActor *object)
{
    struct FieldActor *leader = Object_GetById(gPartyState.currentOwner);
    s32                leaderZ = leader->z;
    s32                z = object->z;
    u8                *flags = &leader->priorityFlags;

    if (leaderZ > z)
    {
        /* FAKEMATCH: the mask is set first so that it takes r3 and the flags byte r2 (differ 4 with *flags |= 4). */
        s32 value = 4;

        value |= *flags;
        *flags = value;
    }
    else
    {
        *flags &= ~6;
    }
}
