#include "trial_road.h"

void Func_020008a4(void)
{
    if (GameFlag_Test(Func_020059b8(gPartyState.currentOwner) + 0x203))
        Func_020059c0();
}

void Func_020008d0(s32 a, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    Func_0200070c(id, 16);
    GameFlag_ClearBit(id + 0x203);
    if (object != NULL)
        object->priorityFlags &= 0xfd;
}

void Func_02000904(s32 a, s32 id)
{
    OBJECT_BYTE(Object_GetById(id - 5), 98) = 0;
    GameFlag_SetBit(id + 0x313);
}

void Func_02000924(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL)
    {
        Func_0200070c(id, -16);
        object->collisionFlags = 0;
        /* FAKEMATCH: stores collisionFlags before it copies the object for the priority flags (differ 2 without it). */
        do
        {
        } while (0);
        object->priorityFlags |= 2;
        Func_020057c8(object, 6);
    }
}

void Func_0200095c(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL)
    {
        Func_0200070c(id, 16);
        object->collisionFlags = 1;
        /* FAKEMATCH: stores collisionFlags before it copies the object for the priority flags (differ 2 without it). */
        do
        {
        } while (0);
        object->priorityFlags &= 0xfd;
        Func_020057c8(object, 5);
    }
}
