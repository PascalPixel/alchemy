#include "local.h"

void Func_02002e04(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != 0)
    {
        u8 *meta = ResourceMetadata_Register(object->animation, 0x17d);

        meta[5] = 0;
        meta[6] = 0;
        Func_02003cc4(object, 0);
        Func_02003cc4(object, 2);
    }
}

void Func_02002e34(s32 entrance)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    s32                   i;

    Func_02003d7c();
    Func_02003eac(0);
    ObjectMotion_SetActionVariant(gPartyState.currentOwner, 1);
    leader->motionFlags = 0;
    /* FAKEMATCH: the native clears the motion flags before it addresses the priority flags (differ 2 without the barrier). */
    __asm__("" : "+r"(leader), "+m"(leader->motionFlags));
    leader->priorityFlags |= 0x20;
    Func_02003ef4(215);
    Func_02003cc4(leader, 18);
    Func_02003ef4(612);
    for (i = 0; i < 60; i++)
    {
        s32 angle;

        if (i == 30)
            Event_FinishScreenTransition();
        leader->y -= 0x4000;
        angle = leader->angle;
        /* FAKEMATCH: the native loads the angle before it builds the turn (differ 2 without the barrier). */
        __asm__ volatile("" : "+r"(angle));
        leader->angle = angle + 0x1000;
        if ((i & 7) == 0)
            Func_02002bd4(Object_GetById(15));
        WaitFrames(1);
    }
    Func_02003d84();
    Func_02003e5c(entrance);
}
