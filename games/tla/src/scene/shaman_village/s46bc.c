#include "shaman_village.h"

void Func_020046bc(void)
{
    s32 variant = Object_GetById(gPartyState.currentOwner)->sprite->priority;

    ObjectMotion_SetActionVariant(9, variant);
    ObjectMotion_SetActionVariant(10, variant);
}
